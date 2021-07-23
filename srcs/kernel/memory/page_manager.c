/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 12:13:40 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/23 15:17:59 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "memory.h"
#include "../../lib/memlib.h"

page_t *get_page(uint32_t address, int make, page_directory_t *dir) {
	// Turn the address into an index.
	address /= 0x1000;
	// Find the page table containing this address.
	uint32_t table_idx = address / 1024;
	if (dir->tables[table_idx]) // If this table is already assigned
		return &dir->tables[table_idx]->pages[address%1024];
	else if (make) {
		uint32_t tmp;
		dir->tables[table_idx] = (page_table_t*)e_kmalloc(sizeof(page_table_t), 1, &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
		return &dir->tables[table_idx]->pages[address%1024];
	}
	else
		return 0;
}
