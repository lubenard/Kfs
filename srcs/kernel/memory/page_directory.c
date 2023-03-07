/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_directory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:47:16 by lubenard          #+#    #+#             */
/*   Updated: 2022/07/19 13:29:00 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/bitwiselib.h"
#include "vmm/malloc/malloc.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
void *page_directory_start;

static inline void flush_tlb_addr(void *addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void unmap_page(void *addr, void *custom_page_directory) {
	int k;
    (void)custom_page_directory;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_WARNING, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	//printk(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;
	if (checkBit(page_directory[pdindex], PD_PRESENT)) {
		//printk(KERN_INFO, "pdindex present");
		uint32_t *page_table_addr = (void*)((page_directory[pdindex] >> 8) << 8);
		//printk(KERN_INFO, "page_addr is %x, origninaly %x", page_table_addr, page_directory[pdindex]);
		if (checkBit(page_table_addr[ptindex], PT_PRESENT)) {
			printd(KERN_INFO, "ptindex present");
			flush_tlb_addr((void*)((page_table_addr[ptindex] >> 8) << 8));
			// Reset given ptindex in Page Directory
			page_table_addr[ptindex] = 0x00000000;
			//printk(KERN_INFO, "page_table_addr[%d] = 0x00000000", ptindex);
		} else
			printd(KERN_WARNING, "ptindex not present for %x", (k + ptindex) * 0x1000);
	}
	else
		printd(KERN_WARNING, "pdindex NOT present");
}

void displayPD() {
    int i = 0;
    //for (i = 0; i < 1024; i++) {
        if (page_directory[i] != 0 && page_directory[i] != 0x2) {
            printd(KERN_INFO, "page_directory[%d] = %p", i, page_directory[i]);
            printd(KERN_INFO, "Page table (located at %p), contains as first value", ((uint32_t *)page_directory[i]));
            printd(KERN_INFO, "Page table first element : %p", page_directory[i]);

            /*for (int j = 0; j < 1024; j++) {
                if (((uint32_t *)page_directory[i])[j] != 0 && ((uint32_t *)page_directory[i])[j] != 0x2) { printd(KERN_INFO, "Page table [%d] = %p", j ,((uint32_t *)page_directory[i])[j]); }
            }*/
        }
    //}
}

void map_page(void *addr, void *custom_page_directory) {
	int k;
	uint32_t *mapping_page_directory;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_ERROR, "Pointer is not aligned !");
		return ;
	}

    mapping_page_directory = (custom_page_directory != 0) ? custom_page_directory : page_directory;

	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printd(KERN_INFO, "%p -> PD : %d/1024, PT : %d/1024", addr, pdindex, ptindex);

	k = pdindex * 1024;
	uint32_t *page_table = (uint32_t*)page_directory_start + k;

	if (checkBit(mapping_page_directory[pdindex], PD_PRESENT)) {
		printd(KERN_INFO, "Page directory index present");
		printd(KERN_INFO, "Page table should be located at %p, originally %p", page_table, mapping_page_directory[pdindex]);
		if (page_table[ptindex] & (1 << 0)) {
			printd(KERN_WARNING, "Map page : ptindex present, page already mapped");
			return ;
		} else {
			printd(KERN_INFO, "Page table at %d not present, mapping from %p to %p", ptindex, (k + ptindex) * 0x1000, ((k + ptindex) * 0x1000) + 0x1000);
			page_table[ptindex] = (k + ptindex) * 0x1000 | 3;
			flush_tlb_addr((void *)((k + ptindex) * 0x1000));
		}
	} else {
		printd(KERN_INFO, "Page directory index NOT present, k = %d", k);
		flush_tlb_addr((void*)((k + ptindex) * 0x1000));
		for (unsigned int i = 0; i < 1024; i++) {
			page_table[i] = 0x00000000;
		}
		page_table[ptindex] = ((k + ptindex) * 0x1000) | 3;
		printd(KERN_INFO, "added page_directory[%d] + mapped page table at addr %p", pdindex, page_table);
		flush_tlb_addr(page_table);
		page_directory[pdindex] = ((unsigned int)page_table) | 3;
	}
    //displayPD();
}

int check_mapping(void *addr) {
	int k;
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printd(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;
	uint32_t *page_table = (uint32_t*)page_directory_start + k;

	if (checkBit(page_directory[pdindex], PD_PRESENT)) {
		if (page_table[ptindex] & (1 << 0))
			return 1;
		else
			return 0;
	} else
		return 0;
}

/*
 * Copy a page directory
 * */
uint32_t* clone_table(uint32_t *src_page_dir, uint32_t *dst_page_dir, int page_dir_idx) {
    uint32_t* table = (uint32_t *)malloc(sizeof(uint32_t) * 1024);
    (void)dst_page_dir;
    (void)page_dir_idx;
    for (int i = 0; i < 1024; i++) {
        //if(!extractBit(table[i], PT_ADDR, 20))
        //    continue;
        // Source frame's virtual address
        //uint32_t src_virtual_address = (page_dir_idx << 22) | (i << 12) | (0);
        // Destination frame's virtual address
        //uint32_t dst_virtual_address = src_virtual_address;
        // Temporary virtual address in current virtual address space
        //uint32_t tmp_virtual_address = 0;
        //printd(KERN_INFO, "Source virtual address for index %d is %p", page_dir_idx, src_virtual_address);

        // Allocate a frame in destination page table
        //allocate_page(dst_page_dir, dst_virtual_address, 0, 0, 1);
        // Now I want tmp_virtual_address and dst_virtual_address both points to the same frame
        //allocate_page(src_page_dir, tmp_virtual_address, dst_page_dir, 0, 1);
        if (checkBit(src_page_dir[i], PT_PRESENT))  SETBIT(table[i], PT_PRESENT);
        if (checkBit(src_page_dir[i], PT_RW))       SETBIT(table[i], PT_RW);
        if (checkBit(src_page_dir[i], PT_USER))     SETBIT(table[i], PT_USER);
        if (checkBit(src_page_dir[i], PT_ACCESSED)) SETBIT(table[i], PT_ACCESSED);
        if (checkBit(src_page_dir[i], PT_DIRTY))    SETBIT(table[i], PT_DIRTY);
        //memcpy((void*)tmp_virtual_address, (void*)src_virtual_address, PAGE_SIZE);
        // Unlink frame
        //free_page(src_page_dir, tmp_virtual_address, 0);
        //printd(KERN_INFO, "Copied table at index %d is %p", page_dir_idx, table[i]);
    }
    return table;
}

/**
 * Similar to init_pd_and_map_kernel, but used for process creation
 */
void copy_kernel_to_process_page_directory(t_page_directory *process_page_directory) {
   	int i = 0;
   	while (i != 1024) {
		printd(KERN_INFO, "Kernel Page directory is %p, process page directory is %p", page_directory[i], process_page_directory->page_directory[i]);
        if (page_directory[i] == process_page_directory->page_directory[i]) {
		//if (checkBit(page_directprocess_page_directoryory[i], 2) == 0) {
            printd(KERN_INFO, "Linking for page %d", i);
            process_page_directory->page_directory[i] = page_directory[i];
        } else {
            printd(KERN_INFO, "Copying page %d", i);
            //uint32_t phys;
            /*process_page_directory->page_directory[i] = */clone_table(page_directory, process_page_directory->page_directory, i);
            //process_page_directory->page_directory[i] = phys | 0x07;
        }
       	i++;
   	}
}

void init_pd_and_map_kernel(void *start_addr, uint32_t nframes) {
	int i;
	uint32_t *page_table = start_addr;
	page_directory_start = start_addr;

	// Check if pointer is aligned on 4096
	if (((uintptr_t)start_addr % 4096) != 0) {
		printk(KERN_WARNING, "Pointer %p is not aligned !", start_addr);
		return;
	}

	for (i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		//   2 is in binary 10
		page_directory[i] = 0x00000002;
	}

	printd(KERN_INFO, "Page directory[1023] %p", page_directory[1023]);

	/*
	 * Mapping 4 first mb (including kernel)
	 * Kernel is very probably starting at 0x100000
	 * We map from 0x0 to 0x400000
	 */
	for (i = 0; i < 1024; i++) {
		if (i == 0 || i == 1023) { printd(KERN_INFO, "First page table: i = %d, addr = %p, addr is %p", i, &(page_table[i]), (i * 0x1000)); }
		page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present. 3 is 11 is binary
	}

	page_directory[0] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present
	printd(KERN_INFO, "added page_directory[0] %p as %p", page_table, page_directory[0]);

	printd(KERN_INFO, "Page directory is at %p", &page_directory);
	printd(KERN_INFO, "Mapped until %p", i * 0x1000);

	printd(KERN_INFO, "JUST BEFORE page_directory[0] %p", page_directory[0]);
	printd(KERN_INFO, "Page directory is %p", page_directory);

	enable_paging(page_directory);

	printd(KERN_INFO, "Page directory is %p", page_directory);


	printd(KERN_INFO, "JUST AFTER page_directory[0] %p", page_directory[0]);

    // If the Page Directory is bigger than what was previously mapped
	if ((uint32_t *)start_addr + nframes >= (uint32_t*)0x400000) {
		// Number of pages i need to allocate for page directory.
		printd(KERN_INFO, "We need to map %d pages for Page Directory", (nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr)) / 1024);

		// How can we know how much we need to map for Page Directory ?
		// We are talking about the type of datas to STORE the pages (uint32_t),
		//   not the pages themselves.
		// We know memory is mapped from 0x0 to 0x400000.
		// ((uint32_t*)0x400000 - (uint32_t*)start_addr))
		//    Allow us to know how much pages we can STORE before going into unmapped memory.
		// (nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr))
		//    Substracting it to nframes (the total of all frames) to know how much pages we CANNOT store in the mapped memory
		// ((nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr)) / 1024)
		//    Since uint32_t is 4 octets, and 1 page is 4096, we simply need to divide this number by 1024
		//    (because 1024 * 4 = 4096)
		for (i = 0; i < (int)((nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr)) / 1024) + 1; i++)
			map_page((char *)0x400000 + 0x1000 * i, 0);
	}
	printd(KERN_INFO, "page_directory[0] %p", page_directory[0]);
}
