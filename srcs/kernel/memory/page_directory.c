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
#include "../../lib/memlib.h"
#include "../kernel.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));

static inline void flush_tlb_addr(void *addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

/**
 * unmap a page on a given page directory
 * @param addr address to unmap
 * @param custom_page_directory (optionnal) if not null, this page directory will be choosen instead of kernel one
 */
void unmap_page(void *addr, void *custom_page_directory) {
	int k;
    uint32_t *mapping_page_directory;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_WARNING, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	//printk(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

    mapping_page_directory = (custom_page_directory != 0) ? custom_page_directory : page_directory;

	k = pdindex * 1024;
	if (checkBit(mapping_page_directory[pdindex], PD_PRESENT)) {
		//printk(KERN_INFO, "pdindex present");

        // Extract 'addr' field from the page_directory
		uint32_t *page_table_addr = (void*)((mapping_page_directory[pdindex] >> 8) << 8);
		//printk(KERN_INFO, "page_addr is %x, origninaly %x", page_table_addr, mapping_page_directory[pdindex]);
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

/**
 * Used for Debug - Display the Page directory and Pages tables if existing
 */
void displayPD(uint32_t *page_dir) {
    printd(KERN_INFO, "------------ Displaying page directory stored at %p ------------", &(page_dir[0]));
    for (int i = 0; i < 1024; i++) {
        printd(KERN_INFO, "page_directory[%d] = %p", i, page_directory[i]);
        if (page_dir[i] != 0 && page_dir[i] != 0x2) {

            printd(KERN_INFO, "Page table is contained at %p", (page_dir[i] >> 8) << 8);

            uint32_t *page_table = (uint32_t*)((page_dir[i] >> 8) << 8);

            for (int j = 0; j < 1024; j++) {
                printd(KERN_INFO, "page_table[%d] = %p", j , page_table[j]);
            }
        }
    }
    printd(KERN_INFO, "------------ End page directory stored at %p ------------", &(page_dir[0]));
}

/**
 * map a page on a given page directory
 * @param addr address to map
 * @param custom_page_directory (optionnal) if not null, this page directory will be choosen instead of kernel one
 */
void map_page(void *addr, void *custom_page_directory) {
	int k;
	uint32_t *mapping_page_directory;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_ERROR, "Pointer is not aligned !");
		return ;
	}
    if (custom_page_directory != 0)
        printd(KERN_WARNING, "Allocating on pd %p", custom_page_directory);
    mapping_page_directory = (custom_page_directory != 0) ? custom_page_directory : page_directory;

	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printd(KERN_INFO, "%p -> PD : %d/1024, PT : %d/1024", addr, pdindex, ptindex);

	k = pdindex * 1024;
	uint32_t *page_table = (uint32_t*) (((mapping_page_directory[0]) >> 8) << 8) + k;

	if (checkBit(mapping_page_directory[pdindex], PD_PRESENT)) {
		printd(KERN_INFO, "Page directory index present");
		printd(KERN_INFO, "Page table should be located at %p, originally %p", page_table, mapping_page_directory[pdindex]);
		if (page_table[ptindex] & (1 << 0)) {
			printd(KERN_WARNING, "Map page : ptindex present, page already mapped");
			return ;
		} else {
			printd(KERN_INFO, "Page table at %d not present, mapping from %p to %p", ptindex, (k + ptindex) * 0x1000, ((k + ptindex) * 0x1000) + 0x1000);
            if (custom_page_directory != 0) {
                printd(KERN_INFO, "Setting page as user page");
                page_table[ptindex] = (k + ptindex) * 0x1000 | 7;
            } else
                page_table[ptindex] = (k + ptindex) * 0x1000 | 3;
			flush_tlb_addr((void *)((k + ptindex) * 0x1000));
		}
	} else {
		printd(KERN_INFO, "Page directory index NOT present, k = %d, flushing tlb at %p", k, ((k + ptindex) * 0x1000));
		flush_tlb_addr((void*)((k + ptindex) * 0x1000));
		for (unsigned int i = 0; i < 1024; i++) {
			page_table[i] = 0x00000000;
		}
		page_table[ptindex] = ((k + ptindex) * 0x1000) | 7;
		printd(KERN_INFO, "added page_directory[%d] + mapped page table at addr %p", pdindex, page_table);
		flush_tlb_addr(page_table);
		page_directory[pdindex] = ((unsigned int)page_table) | 7;
	}
    //displayPD();
}

/*
 * Copy a page directory
 * */
uint32_t clone_table(uint32_t *src_page_dir, t_page_directory *process_page_dir, int page_dir_idx) {
    uint32_t* table = (uint32_t *)malloc(sizeof(uint32_t) * 1024);
    //(void)page_dir_idx;
    printd(KERN_INFO, "------- start clone table --------");
    for (int i = 0; i < 1024; i++) {
        if (!extractBit(src_page_dir[i], PT_ADDR, 20)) {
            printd(KERN_INFO, "src_page_dir[%d] is %d - No addr field - Skipping", i, src_page_dir[i]);
            continue;
        }
        // Source frame's virtual address
        uint32_t src_virtual_address = (page_dir_idx << 22) | (i << 12) | (0);
        // Destination frame's virtual address
        //uint32_t dst_virtual_address = src_virtual_address;
        // Temporary virtual address in current virtual address space
        printd(KERN_INFO, "page_dir_idx is %d (i = %d)", page_dir_idx, i);

        // Allocate a frame in destination page table
        printd(KERN_INFO, "Mapping %p for process page table", src_virtual_address);
        map_page((void*)src_virtual_address, process_page_dir->page_directory);
        if (checkBit(src_page_dir[i], PT_PRESENT))  SETBIT(table[i], PT_PRESENT);
        if (checkBit(src_page_dir[i], PT_RW))       SETBIT(table[i], PT_RW);
        if (checkBit(src_page_dir[i], PT_USER))     SETBIT(table[i], PT_USER);
        if (checkBit(src_page_dir[i], PT_ACCESSED)) SETBIT(table[i], PT_ACCESSED);
        if (checkBit(src_page_dir[i], PT_DIRTY))    SETBIT(table[i], PT_DIRTY);
        printd(KERN_INFO, "In theory, we should copy from %p to %p", src_virtual_address, src_virtual_address + 0x1000);
        //memcpy((void*)tmp_virtual_address, (void*)src_virtual_address, 16384);
        // Unlink frame
        //unmap_page(src_page_dir, tmp_virtual_address, 0);
        printd(KERN_INFO, "Copied table at index %d is %p, src is %p", page_dir_idx, table[i], src_page_dir[i]);
    }
    printd(KERN_INFO, "----- end clone table --------");
    return (uint32_t)table;
}

/**
 * Similar to init_pd_and_map_kernel, but used for process creation
 */
void copy_kernel_to_process_page_directory(uint32_t *src_page_directory, t_page_directory *process_page_directory) {
    printd(KERN_INFO, "src_page_directory[0] is %p (stored at %p)", src_page_directory[0], &src_page_directory);
    printd(KERN_INFO, "process_page_directory[0] is %p (stored at %p)", process_page_directory->page_directory[0], &(process_page_directory->page_directory));
   	for (int i = 0; i < 1024; i++) {
        if (!extractBit(page_directory[i], PT_ADDR, 20)) {
            printd(KERN_INFO, "page_directory[%d] is %d - No addr field - Skipping", i, page_directory[i]);
            continue;
        }
        printd(KERN_INFO, "Kernel Page directory is %p, process page directory is %p", page_directory[i], src_page_directory[i]);
        if (page_directory[i] == src_page_directory[i]) {
            printd(KERN_INFO, "Linking for page %d", i);
            process_page_directory->page_directory[i] = (uint32_t)page_directory[i];
        } else {
            printd(KERN_INFO, "Copying page %d", i);
            process_page_directory->page_directory[i] = clone_table(page_directory, process_page_directory, i);
            process_page_directory->page_directory[i] = process_page_directory->page_directory[i] | 0x07;
            printd(KERN_INFO, "After copy, page_directory[%d] is %p", i, process_page_directory->page_directory[i]);
        }
   	}
    //displayPD(src_page_directory);
    //displayPD(process_page_directory->page_directory);
}


void set_page_dir_into_kernel_struct() {
    get_kernel_struct()->kernel_page_directory = page_directory;
    printd(KERN_INFO, "Registered as %p in kernel struct", get_kernel_struct()->kernel_page_directory[0]);
}

void init_pd_and_map_kernel(void *start_addr, uint32_t nframes) {
	int i;
    printd(KERN_INFO, "page_table = %p, page_directory is %p");
	uint32_t *page_table = start_addr;

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
		if (i == 0 || i == 1023) { printd(KERN_INFO, "Page table: i = %d, addr = %p, addr is %p", i, &(page_table[i]), (i * 0x1000)); }
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
