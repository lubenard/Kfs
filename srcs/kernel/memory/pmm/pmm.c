/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 17:23:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/25 15:23:15 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/iolib.h"
#include "pmm.h"
#include <stddef.h>
#include "../memory.h"

char *pmm_array = 0;
t_pmm *pmm_infos;

// Free block '.', occupied block 'x', shared block '#'

size_t roundUp2(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

void create_pmm_array(void *start_addr, unsigned int page_number) {
	pmm_infos = start_addr;
	pmm_infos->pmm_page_number = page_number;
	pmm_infos->pmm_last_index = 0;
	pmm_array = (char*)start_addr + sizeof(t_pmm) + 1;
	pmm_infos->pmm_memory_start = (void*)roundUp2((char *)pmm_array + page_number + 1, 4096);
	printk(KERN_INFO, "Initialised pmm at %p with size %d, end at %p", pmm_array, page_number, (char*)pmm_array + page_number);
	// Set all blocks to free.
	for (unsigned int i = 0; i < page_number; i++)
		pmm_array[i] = PMM_BLOCK_FREE;
	printk(KERN_INFO, "Pmm Initialised, memory start at %p", pmm_infos->pmm_memory_start);
}

void set_block_status(unsigned int index, char new_block_status) {
	printk(KERN_INFO, "Set block located at index %d new status %c (addr %p)", index, new_block_status, &pmm_array[index]);
	pmm_array[index] = new_block_status;
}

char get_block_status(unsigned int index) {
	return pmm_array[index];
}

void *pmm_next_fit(unsigned int size, int flags) {
	(void)flags;
	unsigned wanted_page_number = (size / 4097) + 1;
	unsigned int available_pages = 0;
	printk(KERN_INFO, "Wanted page_number is %d", wanted_page_number);
	while (pmm_infos->pmm_last_index != pmm_infos->pmm_page_number) {
		if (pmm_array[pmm_infos->pmm_last_index + available_pages] == PMM_BLOCK_FREE) {
			printk(KERN_INFO, "Bloc a index %d is free, available_pages %d / %d", pmm_infos->pmm_last_index + available_pages, available_pages + 1, wanted_page_number);
			available_pages++;
		} else {
			available_pages = 0;
			pmm_infos->pmm_last_index++;
			printk(KERN_INFO, "Reset infos");
		}

		if (available_pages == wanted_page_number) {
			for (unsigned int j = 0; j < wanted_page_number; j++) {
				map_page((char*)pmm_infos->pmm_memory_start + (pmm_infos->pmm_last_index * 0x1000));
				set_block_status(pmm_infos->pmm_last_index + j, PMM_BLOCK_OCCUPIED);
			}
			printk(KERN_INFO, "Returning %p with size %d", (char*)pmm_infos->pmm_memory_start + (pmm_infos->pmm_last_index * 0x1000), size);
			return (char*)pmm_infos->pmm_memory_start + (pmm_infos->pmm_last_index * 0x1000);
		}
	}
	return 0;
}

void pmm_unset_pages(void *ptr, unsigned int size) {
	(void)ptr;
	(void)size;
	unsigned int pages = (size / 4097) + 1;
	unsigned int index = ((char *)ptr - (char *)pmm_infos->pmm_memory_start) / 4096;
	printk(KERN_INFO, "Index should be %d", index);
	for (unsigned int i = 0; i < pages; i++) {
		printk(KERN_INFO, "Releasing page at index %d", index + i);
		pmm_array[index + i] = PMM_BLOCK_FREE;
	}
}
