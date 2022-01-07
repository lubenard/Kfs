/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2022/01/07 15:36:12 by lubenard         ###   ########.fr       */
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

size_t roundUp3(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

uint32_t get_kernel_size() {
	return end_kernel_addr - start_kernel_addr;
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t nframes;

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	// nframes is the number of frames (aka blocks of 4096)
	nframes = (map_entry->len_low / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)

	printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);

	printk(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);

	void *start_pd = (void *)roundUp3((char *)end_kernel_addr + 1, 4096);

	printk(KERN_INFO, "Page directory should begin at %p and end at %p", start_pd, (char *)start_pd + (nframes * sizeof(uint32_t)));

	/*uint32_t *end_pt = start_pd;

	int test_block = 0;
	for (uint32_t j = 0; j < nframes; j++){
		if (end_pt >= (uint32_t *)0x400000 && test_block == 0) {
			printk(KERN_INFO, "Superior from %d", j);
			test_block = 1;
		}
		end_pt++;
	}
	printk(KERN_INFO, "Check Page tables end at %p", end_pt );*/

	//printk(KERN_INFO, "Pmm should take %d pages, aka %d page table and start at %p and end at %p", nframes / 4096, (nframes / 4096) / 1024, (void *)roundUp3((char *)end_kernel_addr + 1 + ((nframes / 1024) * 4096) + 1, 4096), (char *)roundUp3((char *)end_kernel_addr + 1 + ((nframes / 1024) * 4096) + 1, 4096) + nframes);

	init_pd_and_map_kernel((void*)roundUp3((char *)end_kernel_addr + 1, 4096), nframes);

	printk(KERN_INFO, "Page directory should begin at %p and end at %p", start_pd, (char *)start_pd + (nframes * sizeof(uint32_t)));

	/* Init physical memory manager */
	create_pmm_array((void *)start_pd + (nframes * sizeof(uint32_t)) + 1, nframes);

	// USED FOR TEST
	/*map_page((void*)0x400000);
	// Do not cause page fault
	uint32_t *ptr1 = (uint32_t*)0x400000;
	uint32_t not_page_fault = *ptr1;
	(void)not_page_fault;*/

	//unmap_page((void*)0x400000);
}
