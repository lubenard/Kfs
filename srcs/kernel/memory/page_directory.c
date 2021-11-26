/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_directory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 17:47:16 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/26 19:42:21 by lubenard         ###   ########.fr       */
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
		printk(KERN_INFO, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printk(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;

	if (page_directory[pdindex] & (1 << 0)) {
		printk(KERN_INFO, "pdindex present");
		uint32_t *page_table_addr = (void*)((page_directory[pdindex] >> 8) << 8);
		printk(KERN_INFO, "page_addr is %x, origninaly %x", page_table_addr, page_directory[pdindex]);
		if (page_table_addr[ptindex] & (1 << 0)) {
			flush_tlb_addr((void*)((page_table_addr[ptindex] >> 8) << 8));
			printk(KERN_INFO, "ptindex present");
			page_table_addr[ptindex] = 0x00000000;
		} else
			printk(KERN_INFO, "ptindex not present, unmapping for %x", (k + ptindex) * 0x1000);
	}
	else
		printk(KERN_INFO, "pdindex NOT present");
}

void map_page(void *addr) {
	int k;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_INFO, "Pointer is not aligned !");
		return ;
	}

	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printk(KERN_INFO, "%p -> PD : %d, PT : %d", addr, pdindex, ptindex);

	k = pdindex * 1024;
	uint32_t *page_table = 0;

	page_table = (uint32_t*)page_directory_start + k;
	if (page_directory[pdindex] & (1 << 0)) {
		printk(KERN_INFO, "pdindex present");
		printk(KERN_INFO, "page_addr is %p, origninaly %p", page_table, page_directory[pdindex]);
		if (page_table[ptindex] & (1 << 0)) {
			printk(KERN_INFO, "ptindex present");
			return ;
		} else {
			printk(KERN_INFO, "ptindex not present, mapping for %p at page_table[%d]", (k + ptindex) * 0x1000, ptindex);
			page_table[ptindex] = (k + ptindex) * 0x1000 | 3;
			flush_tlb_addr((void *)((k + ptindex) * 0x1000));
		}
	}
	else {
		printk(KERN_INFO, "pdindex NOT present, k = %d", k);
		flush_tlb_addr((void*)((k + ptindex) * 0x1000));
		page_table[ptindex] = ((k + ptindex) * 0x1000) | 3;
		printk(KERN_INFO, "Before loop, page table is %p", page_table);
		for (unsigned int i = 0; i < 1024; i++) {
			if (i != ptindex)
				page_table[i] = 0x00000000;
		}
		printk(KERN_INFO, "After loop");
		printk(KERN_INFO, "added page_directory[%d] + mapped page table at addr %p", pdindex, page_table);
		flush_tlb_addr(page_table);
		page_directory[pdindex] = ((unsigned int)page_table) | 3;
	}
}

void init_pd_and_map_kernel(void *start_addr) {
	int i;
	uint32_t *page_table = start_addr;
	page_directory_start = start_addr;

	// Check if pointer is aligned on 4096
	if (((uintptr_t)start_addr % 4096) != 0) {
		printk(KERN_INFO, "Pointer is not aligned !");
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

	/* Mapping 4 first mb (including kernel) */
	for (i = 0; i < 1024; i++) {
		page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
	page_directory[0] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present
	printk(KERN_INFO, "First page table is located after completion at %p", page_table);

	printk(KERN_INFO, "Page directory is at %p", &page_directory);
	printk(KERN_INFO, "Mapped until %p", (i - 1) * 0x1000);
	//printk(KERN_INFO, "Address of page_table %p and Address of sec_page_table %p, third %p", page_table, sec_page_table, third_page_table);

	enable_paging(page_directory);
}
