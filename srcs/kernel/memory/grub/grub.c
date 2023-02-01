/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grub.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 19:38:31 by lubenard          #+#    #+#             */
/*   Updated: 2022/10/13 14:35:21 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grub.h"
#include "../../../lib/iolib.h"
#include "../../isr/isr.h"

/*multiboot_memory_map_t *get_memory_map_from_grub(multiboot_info_t *mb_mmap) {
	if (!(mb_mmap->flags & (1 << 6))) {
		//PANIC("Couldn't get a Memory map !");
		printk(KERN_ERROR, "Couldn't get a Memory map !");
		return 0;
	}

    printk(KERN_INFO, "symbols from multiboot infos are 0x%x, 0x%x", mb_mmap->u.aout_sym, mb_mmap->u.elf_sec);

	multiboot_memory_map_t* entry = (multiboot_memory_map_t*)mb_mmap->mmap_addr;
	multiboot_memory_map_t* ret_entry = 0;
	while (entry < ((multiboot_memory_map_t*)mb_mmap->mmap_addr + mb_mmap->mmap_length)) {
		printd(KERN_INFO, "Ram detected @ entry %p : addr_low 0x%x addr_high 0x%x | Length: %u - %u | Size: %x | Type: %d", entry,
				entry->addr_low, entry->addr_high, entry->len_low, entry->len_high, entry->size, entry->type);
		// We do not want to detect 'Low Memory', cause it is there that are used vga buffers, etc
		if (entry->type == 1 && (entry->addr_low != 0 || entry->addr_high != 0)) {
			printd(KERN_INFO, "Ram ok @ addr_low 0x%x addr_high 0x%x | Length: %u - %u | Size: %x | Type: %d",
				entry->addr_low, entry->addr_high, entry->len_low, entry->len_high, entry->size, entry->type);

			if (!ret_entry && entry->type == MULTIBOOT_MEMORY_AVAILABLE && (entry->addr_low != 0 || entry->addr_high != 0))
				ret_entry = entry;
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}
	printd(KERN_INFO, "Ram returned for memory map is [addr_low 0x%x addr_high 0x%x | Length: %d - %d | Size: %x | Type: %d]",
				ret_entry->addr_low, ret_entry->addr_high, ret_entry->len_low, ret_entry->len_high, ret_entry->size, ret_entry->type);
	return ret_entry;
}*/