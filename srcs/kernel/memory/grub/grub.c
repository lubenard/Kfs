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

static const char *memory_status_str[] = {
        "Available", "Reserved", "ACPI Reclaimable", "NVS", "Bad Ram",
};

multiboot2_memory_map_t *get_memory_map_from_grub(struct multiboot_tag *tag) {
    multiboot2_memory_map_t *mmap;
    multiboot2_memory_map_t *ret_entry = 0;

    printd(KERN_INFO, "Getting memory map:");

    for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
        (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
        mmap = (multiboot2_memory_map_t *) ((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag)->entry_size)) {

        printd(KERN_NORMAL, " Ram detected @ entry %p : addr_low 0x%x addr_high 0x%x | Length: %u - %u | Type: %s | ",
               mmap,
               (unsigned) (mmap->addr_low & 0xffffffff),
               (unsigned) (mmap->addr_high),
               (unsigned) (mmap->len_low & 0xffffffff),
               (unsigned) (mmap->len_high),
               memory_status_str[(unsigned) (mmap->type) - 1]);

        // We do not want to detect 'Low Memory', because it is there that are used vga buffers, etc
        if ((unsigned) (mmap->type) == MULTIBOOT_MEMORY_AVAILABLE && ((unsigned) (mmap->addr_low & 0xffffffff) != 0 || (unsigned) (mmap->addr_high) != 0)) {
            printd(KERN_NORMAL, "Ram OK\n");

            if (!ret_entry)
                ret_entry = mmap;

        } else {
            printd(KERN_NORMAL, "Ram Not OK\n");
        }

    }
    if (ret_entry != 0)
        printd(KERN_INFO, "Ram returned for memory map is [addr_low 0x%x addr_high 0x%x | Length: %u - %u | Type: %s]",
           (unsigned) (ret_entry->addr_low & 0xffffffff), ret_entry->addr_high, (unsigned) (ret_entry->len_low & 0xffffffff), ret_entry->len_high, memory_status_str[(unsigned) (ret_entry->type) - 1]);
    else
        printk(KERN_ERROR, "COULD NOT GET MEMORY SPACE, ABORT");
    return ret_entry;
}