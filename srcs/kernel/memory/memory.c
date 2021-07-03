/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/03 23:09:51 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"

extern uint32_t endKernel;
uint32_t placement_address = (uint32_t) &endKernel;

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
	placement_address += size;
	return tmp;
}

void init_memory(multiboot_info_t *mb_mmap) {
	page_directory_t *page_directory;
	uint32_t nframes;

	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	// Getting the size of the memory via grub.
	nframes = map_entry->size / 0x1000; // 0x1000 -> 4096 (size of 1 page)
	setFrames((uint32_t *)e_kmalloc(INDEX_FROM_BIT(nframes), 0, 0), nframes);

	// Let's make a page directory.
	page_directory = (page_directory_t*)e_kmalloc(sizeof(page_directory_t), 1, 0);
	memset(page_directory, 0, sizeof(page_directory_t));

	unsigned int i = 0;
	while (i < (uint32_t)placement_address) {
		// Kernel code is readable but not writeable from userspace.
		alloc_frame(get_page(i, 1, page_directory), 0, 0);
		i += 0x1000;
	}
	enable_paging(&page_directory->tablesPhysical[0]);
}
