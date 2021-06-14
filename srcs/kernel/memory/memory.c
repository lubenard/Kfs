/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/06/14 18:04:34 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

void map_page(void * physaddr, void * virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
 
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    //unsigned long * pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    unsigned long * pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
	flush_tlb();
}

void init_memory() {
	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	uint32_t page_table[1024] __attribute__((aligned(4096)));

	//set each entry to not present
	/*for (int i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}*/

	// We are mapping 4Gb into the Page directory.
	// 4Kb (One page) * 1024 (Nbr of pages inside page_entry) * 1024 (Nbr of pages_entry inside page directory) = 4Gb
	for (unsigned int k = 0; k < 1024; k++) {
		//we will fill all 1024 entries in the table, mapping 4 megabytes
		for (unsigned int j = 0; j < 1024; j++) {
			// As the address is page aligned, it will always leave 12 bits zeroed.
			// Those bits are used by the attributes ;)
			// attributes: supervisor level, read/write, present.
			page_table[j] = (j * 0x1000) | 3;
		}
		// attributes: supervisor level, read/write, present
		page_directory[k] = ((unsigned int)page_table) | 3;
	}
	enable_paging(page_directory);
}
