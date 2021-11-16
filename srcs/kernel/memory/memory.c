/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/16 18:24:26 by lubenard         ###   ########.fr       */
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
uint32_t static placement_address = (uint32_t) &endKernel;

uint32_t *frames;

void set_placement_addr(uint32_t new_placement_address) {
	placement_address += new_placement_address;
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	uint32_t first_page_table[1024] __attribute__((aligned(4096)));

	//page_directory_t *page_directory;
	uint32_t nframes;

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	uint32_t diff = (void*)placement_address - (void *)map_entry->addr_low;

	printk(KERN_INFO, "Diff is %d", diff);
	printk(KERN_INFO, "placement_address is %p", placement_address);
	printk(KERN_INFO, "Original space is %d", map_entry->len_low);
	printk(KERN_INFO, "Real size 2 should be %d bytes", map_entry->len_low - diff);

	printk(KERN_INFO, "Diff is %x (%d in decimal) (%x - %x)", diff, diff, (void*)placement_address, (void*)map_entry->addr_low);

	printk(KERN_INFO, "From endKernel to end, there is %d bytes available", map_entry->len_low - diff);

	// Getting the size of the memory via grub.
	nframes = (map_entry->len_low - diff) / 0x1000; // 0x1000 -> 4096 (size of 1 page)

	printk(KERN_NORMAL, "Should require no more than %d pages, starting at %p\n", nframes, placement_address);

	create_pmm_array((void *)placement_address, nframes);
	set_block_status(0, PMM_BLOCK_OCCUPIED);
	set_block_status(1, PMM_BLOCK_SHARED);
	set_block_status(2, PMM_BLOCK_SHARED);
	set_block_status(5, PMM_BLOCK_FREE);
	unsigned int i;

	for(i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(i = 0; i < 1024; i++) {
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		//printk(KERN_INFO, "Filling first_page_table at %p (i = %d)", &first_page_table[i], i);
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	page_directory[0] = ((unsigned int)first_page_table) | 3;

	//switch_page_directory(page_directory);
	enable_paging(page_directory);
	//enable_paging(&page_directory->tablesPhysical[0]);
	//printk(KERN_INFO, "Paging enabled and working, can begin ram at %x for %d bytes", map_entry->addr_low, map_entry->len_low);
	//printk(KERN_INFO, "EndKenel says %x, addr_low says %d", placement_address, map_entry->addr_low);
	//printk(KERN_INFO, "ram is now at %x, with a size of %d", placement_address, map_entry->len_low - (placement_address - map_entry->addr_low));
	//mem_page_tracking_t *head = first_fit_memory(0, 10);
	//first_fit_memory(head, 4);
	//char *test = kmalloc(10);
	//(void)test;
	//test[0] = 'a';
	//test[1] = '\0';
	//printk(KERN_NORMAL, "TEST STRING = '%s'\n", test);
	//kmalloc(10);
}
