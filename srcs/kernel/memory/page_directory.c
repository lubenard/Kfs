/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_directory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:47:16 by lubenard          #+#    #+#             */
/*   Updated: 2022/06/23 23:05:22 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
void *page_directory_start;

static inline void flush_tlb_addr(void *addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void unmap_page(void *addr) {
	int k;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_WARNING, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	//printk(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;
	if (page_directory[pdindex] & (1 << 0)) {
		//printk(KERN_INFO, "pdindex present");
		uint32_t *page_table_addr = (void*)((page_directory[pdindex] >> 8) << 8);
		//printk(KERN_INFO, "page_addr is %x, origninaly %x", page_table_addr, page_directory[pdindex]);
		if (page_table_addr[ptindex] & (1 << 0)) {
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

void map_page(void *addr) {
	int k;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_ERROR, "Pointer is not aligned !");
		return ;
	}

	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printk(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;
	uint32_t *page_table = (uint32_t*)page_directory_start + k;

	if (page_directory[pdindex] & (1 << 0)) {
		printk(KERN_INFO, "pdindex present");
		printk(KERN_INFO, "Page table should be located at %p, origninaly %p", page_table, page_directory[pdindex]);
		if (page_table[ptindex] & (1 << 0)) {
			printd(KERN_WARNING, "Map page : ptindex present, page already mapped");
			return ;
		} else {
			printk(KERN_INFO, "ptindex not present, mapping from %p to %p at page_table[%d]", (k + ptindex) * 0x1000, ((k + ptindex) * 0x1000) + 0x1000, ptindex);
			page_table[ptindex] = (k + ptindex) * 0x1000 | 3;
			flush_tlb_addr((void *)((k + ptindex) * 0x1000));
		}
	} else {
		printk(KERN_INFO, "pdindex NOT present, k = %d", k);
		flush_tlb_addr((void*)((k + ptindex) * 0x1000));
		for (unsigned int i = 0; i < 1024; i++) {
			page_table[i] = 0x00000000;
		}
		page_table[ptindex] = ((k + ptindex) * 0x1000) | 3;
		printk(KERN_INFO, "added page_directory[%d] + mapped page table at addr %p", pdindex, page_table);
		flush_tlb_addr(page_table);
		page_directory[pdindex] = ((unsigned int)page_table) | 3;
	}
    displayPD();
}

int check_mapping(void *addr) {
	int k;
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printd(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;
	uint32_t *page_table = (uint32_t*)page_directory_start + k;

	if (page_directory[pdindex] & (1 << 0)) {
		if (page_table[ptindex] & (1 << 0))
			return 1;
		else
			return 0;
	} else
		return 0;
}

void init_pd_and_map_kernel(void *start_addr, uint32_t nframes) {
	int i;
	uint32_t *page_table = start_addr;
	page_directory_start = start_addr;

	// Check if pointer is aligned on 4096
	if (((uintptr_t)start_addr % 4096) != 0) {
		printk(KERN_WARNING, "Pointer is not aligned !");
		return;
	}

	for(i = 0; i < 1024; i++) {
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
		page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
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

    // If the Page Directory is bigger than what was proviously mapped
	if ((uint32_t *)start_addr + nframes >= (uint32_t*)0x400000) {
		// Number of pages i need to allocate for page directory.
		printd(KERN_INFO, "We need to map %d pages for Page Directory", (nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr)) / 1024);

		// How can we know how much we need to map for Page Directory ?
		// We are talking about the type of datas to STORE the pages (uint32_t),
		//   not the pages themselves.
		// We know memory is mapped to 0x400000.
		// ((uint32_t*)0x400000 - (uint32_t*)start_addr))
		//    Allow us to know how much pages we can STORE before going into unmapped memory.
		// (nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr))
		//    Substracting it to nframes (the total of all frames) to know how much pages we CANNOT store in the mapped memory
		// ((nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr)) / 1024)
		//    Since uint32_t is 4 octets, and 1 page is 4096, we simply need to divide this number by 1024
		//    (because 1024 * 4 = 4096)
		for (i = 0; i < (int)((nframes - ((uint32_t*)0x400000 - (uint32_t*)start_addr)) / 1024) + 1; i++)
			map_page((char *)0x400000 + 0x1000 * i);
	}
    printd(KERN_INFO, "page_directory[0] %p", page_directory[0]);
}
