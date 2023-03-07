/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 17:23:38 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/18 01:47:36 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/iolib.h"
#include "pmm.h"
#include "../memory.h"
#include "../../../lib/memlib.h"

char *pmm_array = 0;
t_pmm *pmm_infos;

// Free block '.', occupied block 'x', shared block '#'

void check_mapping_pmm(void *pmm_array, unsigned int page_number) {
	if ((char *)pmm_array + page_number > (char*)0x3FF000) {
		printd(KERN_INFO, "PMM is not in a mapped area (%p), should map it first.", pmm_array);
		unsigned int unmapped_pmm = page_number - roundUpDiff(pmm_array, 4096);
		printd(KERN_INFO, "We need to map %d pages for PMM", unmapped_pmm / 4096);
		void *start_mapping = (void*)roundUp(pmm_array, 4096);
		for (unsigned int j = 0; j < (unmapped_pmm / 4096) + 1; j++) {
			printd(KERN_INFO, "Mapping %p", start_mapping + (4096 * j));
			map_page(start_mapping + (4096 * j), 0);
		}
	} else
		printd(KERN_INFO, "PMM is contained in a mapped area (before 0x3FF000).\nStart at %p, end at %p", pmm_array, (char*)pmm_array + page_number);
}

void *create_pmm_array(void *start_addr, unsigned int page_number) {
	printk(KERN_INFO, "Pmm infos is stored at %p", start_addr);
	pmm_infos = start_addr;
	pmm_infos->pmm_page_number = page_number;
	pmm_infos->pmm_last_index = 0;
	pmm_infos->available_pages_number = page_number;
	pmm_array = (char*)start_addr + sizeof(t_pmm) + 1;
	pmm_infos->pmm_memory_start = (void*)roundUp((char *)pmm_array + page_number + 1, 4096);
	printd(KERN_INFO, "Pmm will be init at %p with size %d, end at %p", pmm_array, page_number, (char*)pmm_array + page_number);
	check_mapping_pmm(pmm_array, page_number);
	// Set all blocks to free.
	for (unsigned int i = 0; i < page_number; i++)
		pmm_array[i] = PMM_BLOCK_FREE;
	printd(KERN_INFO, "Pmm Initialised, at %p memory start at %p", pmm_array, pmm_infos->pmm_memory_start);
	return pmm_infos->pmm_memory_start;
}

void set_block_status(unsigned int index, char new_block_status) {
	printd(KERN_INFO, "Set block located at index %d new status %c (addr %p)", index, new_block_status, &pmm_array[index]);
	pmm_array[index] = new_block_status;
}

char get_block_status(unsigned int index) {
	return pmm_array[index];
}

void *pmm_next_fit(unsigned int size, int flags) {
	(void)flags;
	unsigned wanted_page_number = (size / 4096);
	unsigned int available_pages = 0;
	printd(KERN_INFO, "Wanted page_number is %d", wanted_page_number);
	if (wanted_page_number > pmm_infos->available_pages_number) {
		printk(KERN_ERROR, "Out of memory !");
		return 0;
	}
	while (pmm_infos->pmm_last_index != pmm_infos->pmm_page_number) {
		if (pmm_array[pmm_infos->pmm_last_index + available_pages] == PMM_BLOCK_FREE) {
			printd(KERN_INFO, "Bloc a index (%d + %d)=%d/%d is free, available_pages %d / %d", pmm_infos->pmm_last_index, available_pages, pmm_infos->pmm_last_index + available_pages, pmm_infos->pmm_page_number, available_pages + 1, wanted_page_number);
			available_pages++;
		} else {
            printd(KERN_INFO, "Bloc a index %d/%d is occupied, reset research", pmm_infos->pmm_last_index + available_pages, pmm_infos->pmm_page_number);
            available_pages = 0;
            pmm_infos->pmm_last_index++;
            if (pmm_infos->pmm_last_index > pmm_infos->pmm_page_number) {
                pmm_infos->pmm_last_index = 0;
            }
		}

		if (available_pages == wanted_page_number) {
			for (unsigned int j = 0; j < wanted_page_number; j++) {
				printd(KERN_INFO, "Mapping page %d/%d addr: %p", j + 1, wanted_page_number, (char*)pmm_infos->pmm_memory_start + (0x1000 * (j + pmm_infos->pmm_last_index)));
				printd(KERN_INFO, "%p + (4096 * %d + %d) = %p", pmm_infos->pmm_memory_start, j, pmm_infos->pmm_last_index, (char*)pmm_infos->pmm_memory_start + (pmm_infos->pmm_last_index + (0x1000 * j)));
				map_page((char*)pmm_infos->pmm_memory_start + (0x1000 * (j + pmm_infos->pmm_last_index)), 0);
				set_block_status(pmm_infos->pmm_last_index + j, PMM_BLOCK_OCCUPIED);
			}
			pmm_infos->available_pages_number -= wanted_page_number;
			printd(KERN_INFO, "Returning %p with size %d (%d pages)", (char*)pmm_infos->pmm_memory_start + (pmm_infos->pmm_last_index * 0x1000), size, wanted_page_number);
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
	printd(KERN_INFO, "Index should be %d", index);
	for (unsigned int i = 0; i < pages; i++) {
		printd(KERN_INFO, "Releasing page at index %d", index + i);
		pmm_array[index + i] = PMM_BLOCK_FREE;
		unmap_page((char*)pmm_infos->pmm_memory_start + (pmm_infos->pmm_last_index * 0x1000), 0);
	}
	pmm_infos->available_pages_number += pages;
}
