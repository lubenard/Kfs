/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/30 18:16:10 by lubenard         ###   ########.fr       */
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
	nframes = ((map_entry->len_low - get_kernel_size()) / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)

	printk(KERN_NORMAL, "Should require no more than %d pages without kernel, origninaly %d\n", nframes, (map_entry->len_low / 0x1000));
	printk(KERN_NORMAL, "%d pages = %d tables\n", (map_entry->len_low / 0x1000), (map_entry->len_low / 0x1000) / 1024);

	printk(KERN_INFO, "Memory should begin at %p, end_kernel_addr at %p", roundUp3((char *)end_kernel_addr + 1, 4096), end_kernel_addr);

	printk(KERN_INFO, "%d page directory needed + %d pages table entry needed + %d frames", nframes / 1024, nframes, nframes % 4096);
	printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);
	printk(KERN_INFO, "Pmm should take %d pages, aka %d page table", nframes / 4096, (nframes / 4096) / 1024);

	init_pd_and_map_kernel((void*)roundUp3((char *)end_kernel_addr + 1, 4096));

	/* Init physical memory manager */
	create_pmm_array((void *)roundUp3((char *)end_kernel_addr + 1 + ((nframes / 1024) * 4096) + 1, 4096), nframes);

	/*map_page((void*)0x400000);
	// Do not cause page fault
	uint32_t *ptr1 = (uint32_t*)0x400000;
	uint32_t not_page_fault = *ptr1;
	(void)not_page_fault;*/

	//unmap_page((void*)0x400000);

	//map_page((void*)0x401000);
	// Cause page fault
	/*uint32_t *ptr2 = (uint32_t*)0x401000;
	uint32_t do_page_fault = *ptr2;
	(void)do_page_fault;*/

	//map_page((void*)0x402000);
	//unmap_page((void*)0x401000);
	// Cause page fault
	/*uint32_t *ptr1 = (uint32_t*)0x402000;
	uint32_t do_page_fault2 = *ptr1;
	(void)do_page_fault2;*/

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
