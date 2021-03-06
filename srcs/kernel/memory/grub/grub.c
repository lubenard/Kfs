/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grub.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 19:38:31 by lubenard          #+#    #+#             */
/*   Updated: 2021/12/02 18:04:39 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grub.h"
#include "../../../lib/iolib.h"
#include "../../isr/isr.h"

multiboot_memory_map_t *get_memory_map_from_grub(multiboot_info_t *mb_mmap) {
	if (!(mb_mmap->flags & (1 << 6))) {
		PANIC("Couldn't get a Memory map !");
	}
	multiboot_memory_map_t* entry = (multiboot_memory_map_t*)mb_mmap->mmap_addr;
	multiboot_memory_map_t* ret_entry = 0;
	while (entry < ((multiboot_memory_map_t*)mb_mmap->mmap_addr + mb_mmap->mmap_length)) {
		// We do not want to detect 'Low Memory', cause it is there that are used vga buffers, etc
		//if (entry->type == 1 && (entry->addr_low != 0 || entry->addr_high != 0)) {
			//printk(KERN_INFO, "Ram ok @ addr_low 0x%x addr_high 0x%x, size %d %d", entry->addr_low, entry->addr_high, entry->len_low, entry->len_high);
			printk(KERN_INFO, "Start Addr: %x - %x | Length: %d - %d | Size: %x | Type: %d", entry->addr_low, entry->addr_high, entry->len_low, entry->len_high, entry->size, entry->type);
			// Temp ret ? only returning after 1rst free section found
			//return entry;
			if (!ret_entry && entry->type == MULTIBOOT_MEMORY_AVAILABLE && (entry->addr_low != 0 || entry->addr_high != 0))
				ret_entry = entry;
		//}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
	return ret_entry;
}

