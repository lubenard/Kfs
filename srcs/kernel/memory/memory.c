/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/23 04:41:05 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"
#include "heap/heap.h"

extern uint32_t endKernel;
uint32_t static placement_address = (uint32_t) &endKernel;

void set_placement_addr(uint32_t new_placement_address) {
	placement_address += new_placement_address;
}

/*
 * Early malloc. Used to create the linked list for memory management
 */
uint32_t e_kmalloc(uint32_t size, int align, uint32_t *phys) {
	if (align == 1 && (placement_address & 0xFFFFF000)) {
		// Align address with physical addr if needed.
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys)
		*phys = placement_address;
	uint32_t tmp = placement_address;
	printk(KERN_INFO, "Placement addr is %d", placement_address);
	placement_address += size;
	return tmp;
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	uint32_t first_page_table[1024] __attribute__((aligned(4096)));

	//page_directory_t *page_directory;
	//uint32_t nframes;

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);
	(void)map_entry;

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
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}

	page_directory[0] = ((unsigned int)first_page_table) | 3;

	// Getting the size of the memory via grub.
	/*nframes = map_entry->len_low / 0x1000; // 0x1000 -> 4096 (size of 1 page)
	setFrames((uint32_t *)e_kmalloc(INDEX_FROM_BIT(nframes), 0, 0), nframes);

	// Let's make a page directory.
	page_directory = (page_directory_t*)e_kmalloc(sizeof(page_directory_t), 1, 0);
	memset(page_directory, 0, sizeof(page_directory_t));

	for (i = KHEAP_START; i < KHEAP_START+KHEAP_MIN_SIZE; i += 0x1000)
		get_page(i, 1, page_directory);

	i = 0;
	while (i < (uint32_t)placement_address) {
		// Kernel code is readable but not writeable from userspace.
		alloc_frame(get_page(i, 1, page_directory), 0, 0);
		i += 0x1000;
	}

	for (i = KHEAP_START; i < KHEAP_START+KHEAP_MIN_SIZE; i += 0x1000)
		alloc_frame(get_page(i, 1, page_directory), 0, 0);

	switch_page_directory(page_directory);*/
	enable_paging(page_directory);
	//enable_paging(&page_directory->tablesPhysical[0]);
	//printk(KERN_INFO, "Paging enabled and working, can begin ram at %x for %d bytes", map_entry->addr_low, map_entry->len_low);
	printk(KERN_INFO, "EndKenel says %x, addr_low says %d", placement_address, map_entry->addr_low);
	//printk(KERN_INFO, "ram is now at %x, with a size of %d", placement_address, map_entry->len_low - (placement_address - map_entry->addr_low));
	//mem_page_tracking_t *head = first_fit_memory(0, 10);
	//first_fit_memory(head, 4);
	char *test = kmalloc(10);
	(void)test;
	test[0] = 'a';
	//test[1] = '\0';
	//printk(KERN_NORMAL, "TEST STRING = %s\n", test);
	//kmalloc(10);
}
