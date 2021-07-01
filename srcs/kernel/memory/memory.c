/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/01 23:58:37 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"

extern void* endKernel;

multiboot_memory_map_t *get_memory_map_from_grub(multiboot_info_t *mb_mmap) {
	if (!(mb_mmap->flags & (1<<6))) {
		printk(KERN_ERROR, "Couldn't get memory map!");
		//PANIC("Couldn't get a Memory map !");
	}

	multiboot_memory_map_t* entry = (multiboot_memory_map_t*)mb_mmap->mmap_addr;
	multiboot_memory_map_t* ret_entry = entry;
	while (entry < ((multiboot_memory_map_t*)mb_mmap->mmap_addr + mb_mmap->mmap_length)) {
		// We do not want to detect 'Low Memory', cause it is there that are used vga buffers, etc
		if (entry->type == 1 && (entry->addr_low != 0 || entry->addr_high != 0)) {
			printk(KERN_INFO, "Ram ok @ addr_low 0x%x addr_high 0x%x, size %d %d", entry->addr_low, entry->addr_high, entry->len_low, entry->len_high);
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
	return ret_entry;
}

mem_page_tracking_t *create_new_node_memory(uint32_t addr_low, uint32_t size, uint32_t pid) {
	mem_page_tracking_t *node;
	// We can create a header element right now.
	if (!(node = (mem_page_tracking_t*)e_kmalloc(sizeof(mem_page_tracking_t), 1, NULL)))
		return NULL;
	node->addr_low = addr_low;
	//node->addr_high = entry->addr_high;
	node->len_low = size;
	//node->len_high = entry->len_high;
	node->is_allocated = 1;
	node->owner_pid = pid;
	return node;
}

void first_fit_memory(mem_page_tracking_t *lknd_lst, unsigned int size) {
	// If there is no element in the linked list, all memory is free.
	if (lknd_lst == NULL) {
		mem_page_tracking_t *head = create_new_node_memory((uint32_t) &endKernel, size, 0);
		printk(KERN_INFO, "First node is size %d containing addr %x", head->len_low, head->addr_low);
	} else {
		while (lknd_lst->next) {
			if (lknd_lst->addr_low + size < lknd_lst->next->addr_low) {
				// We can allocate here
				mem_page_tracking_t *node = create_new_node_memory(lknd_lst->addr_low + lknd_lst->len_low, size, 0);
				printk(KERN_INFO, "First node is size %d", node->len_low);
				return ;
			}
			lknd_lst = lknd_lst->next;
		}
	}
}

void init_memory(multiboot_info_t *mb_mmap) {
	uint32_t page_directory[1024] __attribute__((aligned(4096)));
	uint32_t page_table[1024] __attribute__((aligned(4096)));

	(void)mb_mmap;
	multiboot_memory_map_t *map_entry = get_memory_map_from_grub(mb_mmap);

	(void)map_entry;
	//set each entry to not present
	/*for (int i = 0; i < 1024; i++) {
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}*/

	// We are mapping 4Gb into the Page directory.
	// 4Kb (One page) * 1024 (Nbr of pages inside 1 page entry) * 1024 (Nbr of pages entrys inside page directory) = 4Gb
	for (unsigned int k = 0; k < 1024; k++) {
		//we will fill all 1024 entries in the table, mapping 4 megabytes
		for (unsigned int j = 0; j < 1024; j++) {
			// As the address is page aligned, it will always leave 12 bits zeroed.
			// Those bits are used by the attributes ;)
			// attributes: supervisor level, read/write, present.
			page_table[j] = (j * 0x1000) | 3;
			//if (k == 0)
			//	printk(KERN_INFO, "Page address (%d) is %d", j, page_table[j]);
		}
		// attributes: supervisor level, read/write, present
		page_directory[k] = ((unsigned int)page_table) | 3;
	}
	enable_paging(page_directory);

	//build_virtual_mem_lkd_list(map_entry, mb_mmap);
	first_fit_memory(NULL, 20);
}
