/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/22 16:11:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"
#include "heap/heap.h"
#include "pmm/pmm.h"

extern uint32_t endKernel;
extern uint32_t startKernel;

uint32_t static placement_address = (uint32_t) &endKernel;
uint32_t static start_kernel_addr = (uint32_t) &startKernel;

uint32_t *frames;

void set_placement_addr(uint32_t new_placement_address) {
	placement_address += new_placement_address;
}

size_t roundUp(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

uint32_t get_kernel_size() {
	return placement_address - start_kernel_addr;
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t nframes;
	unsigned int i;
	unsigned int k = 0;
	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	uint32_t page_table[1024] __attribute__((aligned(4096)));
	//uint32_t sec_page_table[1024] __attribute__((aligned(4096)));
	//uint32_t third_page_table[1024] __attribute__((aligned(4096)));

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	nframes = (map_entry->len_low / 0x1000) + 256; // Ox1000 is 4096 in hexa (Size of one page)

	printk(KERN_NORMAL, "Should require no more than %d pages\n", nframes);

	printk(KERN_INFO, "Memory should begin at %p", roundUp((void*)placement_address, 4096));

	/*create_pmm_array((void *)placement_address, nframes);
	set_block_status(0, PMM_BLOCK_OCCUPIED);
	set_block_status(1, PMM_BLOCK_SHARED);
	set_block_status(2, PMM_BLOCK_SHARED);
	set_block_status(5, PMM_BLOCK_FREE);*/

	printk(KERN_INFO, "Page directory is at %p", &page_directory);

	for(i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		//   2 is in binary 10
		page_directory[i] = 0x00000002;
	}

	printk(KERN_INFO, "%d pages directory entry needed + %d frames", nframes / 1024, nframes % 1024);
	printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);

	/* Mapping all memory */
	for (unsigned int j = 0; j < ((get_kernel_size() / 4096) / 1024) + 1; j++) {
		for (i = 0; i < 257 + get_kernel_size(); i++) {
			page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
			//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
			k++;
		}
		//printk(KERN_INFO, "J = %d", j);
		page_directory[j] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present
	}

	//printk(KERN_INFO, "Address of page_table %p and Address of sec_page_table %p, third %p", page_table, sec_page_table, third_page_table);

	/*for (i = 0; i < 1024; i++) {
		page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
		k++;
	}
	page_directory[0] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present

	for (i = 0; i < 1024; i++) {
		sec_page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
		k++;
	}
	page_directory[1] = ((unsigned int)sec_page_table) | 3; // attributes: supervisor level, read/write, present

	for (i = 0; i < 1024; i++) {
		third_page_table[i] = (k * 0x1000) | 3; // attributes: supervisor level, read/write, present.
		//printk(KERN_INFO, "%d * 0x1000 = %d, aka %p", k, k * 0x1000, k * 0x1000);
		k++;
	}
	page_directory[2] = ((unsigned int)third_page_table) | 3; // attributes: supervisor level, read/write, present
*/

	//printk(KERN_INFO, "Mapped %d pages", nframes / 1024);

	/*for (i = 0; i < nframes % 1024; i++) {
		page_table[i] = (k * 0x1000) | 3;
		k++;
	}*/
	//page_directory[++j] = ((unsigned int)page_table) | 3; // attributes: supervisor level, read/write, present

	//printk(KERN_INFO, "Mapped %d", nframes % 1024);

	enable_paging(page_directory);
	printk(KERN_INFO, "Paging enabled and working, mapped 1mb + kernel");
}
