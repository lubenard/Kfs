/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2022/04/11 12:29:56 by lubenard         ###   ########.fr       */
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
	printk(KERN_INFO, "Kernel start at %p, and end at %p", start_kernel_addr, end_kernel_addr);

	//printd(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);
	printd(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);

	void *start_pd = (void *)roundUp((char *)end_kernel_addr + 1, 4096);

	void *end_pd = (char *)start_pd + (nframes * sizeof(uint32_t));

	printk(KERN_INFO, "Page tables should begin at %p and end at %p", start_pd, end_pd);

	init_pd_and_map_kernel(start_pd, nframes);

	/* Init physical memory manager */
	create_pmm_array(end_pd + 1, nframes);

	// USED FOR TEST
	/*map_page((void*)0x400000);
	// Do not cause page fault
	uint32_t *ptr1 = (uint32_t*)0x400000;
	uint32_t not_page_fault = *ptr1;
	(void)not_page_fault;*/

	//unmap_page((void*)0x400000);
}
