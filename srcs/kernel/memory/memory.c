/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/25 19:33:32 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"
#include "pmm/pmm.h"
#include "vmm/vmm.h"

extern uint32_t endKernel;
extern uint32_t startKernel;

uint32_t static end_kernel_addr = (uint32_t) &endKernel;
uint32_t static start_kernel_addr = (uint32_t) &startKernel;

uint32_t page_directory[1024] __attribute__((aligned(4096)));

void *map_page_start;

void unmap_page(void *addr) {
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_INFO, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	printk(KERN_INFO, "PD : %d, PT : %d", pdindex, ptindex);
	if (page_directory[pdindex] & (1 << 0)) {
		printk(KERN_INFO, "pdindex present");
		printk(KERN_INFO, "Removed pdindex %d to page_directory", pdindex);
		page_directory[pdindex] = 0x00000000;
		flush_tlb();
		enable_paging(page_directory);
	} else {
		printk(KERN_INFO, "pdindex NOT present");
	}
}

void map_page(void *addr) {
	int k;
	// Check if pointer is aligned on 4096
	if (((uintptr_t)addr % 4096) != 0) {
		printk(KERN_INFO, "Pointer is not aligned !");
		return;
	}
	unsigned long pdindex = (unsigned long)addr >> 22;
	unsigned long ptindex = (unsigned long)addr >> 12 & 0x03FF;
	k = pdindex * 1024;
	printk(KERN_INFO, "PD : %d, PT : %d", pdindex, ptindex);
	if (page_directory[pdindex] & (1 << 0))
		printk(KERN_INFO, "pdindex present");
	else {
		printk(KERN_INFO, "pdindex NOT present, k = %d", k);
		uint32_t *page_table = map_page_start;
		for (int i = 0; i < 1024; i++) {
			page_table[i] = (k * 0x1000) | 3;
			k++;
		}
		printk(KERN_INFO, "Added pdindex %d to page_directory", pdindex);
		page_directory[pdindex] = ((unsigned int)page_table) | 3;
		flush_tlb();
		enable_paging(page_directory);
	}
}

void set_placement_addr(uint32_t new_end_kernel_addr) {
	end_kernel_addr += new_end_kernel_addr;
}

size_t roundUp3(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

uint32_t get_kernel_size() {
	return end_kernel_addr - start_kernel_addr;
}

void compute_space_for_page_directory(void *start_addr, uint32_t nframes) {
	start_addr = (void*) roundUp3(start_addr, 4096);
	map_page_start = start_addr;
	printk(KERN_NORMAL, "Should require no more than %d nframes, starting at %p\n", nframes / 1024, start_addr);
	printk(KERN_NORMAL, "Should take %d bytes\n", sizeof(uint32_t) * (nframes / 1024) * 1024);
	for (uint32_t i = 0; i < nframes / 1024; i++) {
		//map_page(start_addr);
		//printk(KERN_INFO, "Addr for pd is %p", start_addr);
		start_addr += 4096;
	}
	printk(KERN_NORMAL, "compute space for page directory: %p -> %p\n", map_page_start, start_addr);
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t nframes;
	unsigned int i;
	uint32_t page_table[1024] __attribute__((aligned(4096)));

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	nframes = ((map_entry->len_low - get_kernel_size()) / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)

	printk(KERN_NORMAL, "Should require no more than %d pages without kernel, origninaly %d\n", nframes, (map_entry->len_low / 0x1000));
	printk(KERN_NORMAL, "%d pages = %d tables\n", (map_entry->len_low / 0x1000), (map_entry->len_low / 0x1000) / 1024);

	printk(KERN_INFO, "Memory should begin at %p, end_kernel_addr at %p", roundUp3((char *)end_kernel_addr + 1, 4096), end_kernel_addr);

	printk(KERN_INFO, "Page directory is at %p", &page_directory);

	for(i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		//   2 is in binary 10
		page_directory[i] = 0x00000002;
	}

	printk(KERN_INFO, "%d page directory needed + %d pages table entry needed + %d frames", nframes / 1024, nframes, nframes % 4096);
	printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);
	printk(KERN_INFO, "Pmm should take %d pages, aka %d page table", nframes / 4096, (nframes / 4096) / 1024);

	/* Mapping 4 first mb (including kernel) */
	for (i = 0; i < 1024; i++) {
		page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "k = %d aka %p", k, k * 0x1000);
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
	}
	//printk(KERN_INFO, "J = %d, k = %d", j, k);
	page_directory[0] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present

	printk(KERN_INFO, "Mapped until %p", (i - 1) * 0x1000);
	//printk(KERN_INFO, "Address of page_table %p and Address of sec_page_table %p, third %p", page_table, sec_page_table, third_page_table);

	enable_paging(page_directory);

	compute_space_for_page_directory((char *)end_kernel_addr + 1, nframes);

	/* Init physical memory manager */
	create_pmm_array((void *)roundUp3((char *)end_kernel_addr + 1 + ((nframes / 1024) * 4096) + 1, 4096), nframes);

	map_page((void*)0x400000);

	// Do not cause page fault
	uint32_t *ptr1 = (uint32_t*)0x400000;
	uint32_t not_page_fault = *ptr1;
	(void)not_page_fault;

	unmap_page((void*)0x400000);

	// Cause page fault
	uint32_t *ptr = (uint32_t*)0x400000;
	uint32_t do_page_fault = *ptr;
	(void)do_page_fault;

	/*map_page((void*)0x400000);
	map_page((void*)0x800000);
	map_page((void*)0x1200000);
	unmap_page((void*)0x800000);
	map_page((void*)0x800000);*/

	// Should cause page fault
	/*uint32_t *ptr = (uint32_t*)0x400000;
	uint32_t do_page_fault = *ptr;
	(void)do_page_fault;*/
}
