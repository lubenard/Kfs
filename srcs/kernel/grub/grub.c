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
#include "../debug/debug.h"
#include "../memory/memory.h"

static const char *memory_status_str[] = {
        "Available", "Reserved", "ACPI Reclaimable", "NVS", "Bad Ram",
};

void get_grub_boot_info(unsigned long addr) {
    if (addr & 7) {
        printk(KERN_ERROR, "Unaligned mbi: 0x%x\n", addr);
        return;
    }

    struct multiboot_tag *tag;
    unsigned size;

    size = *(unsigned *) addr;
    printk (KERN_INFO, "Announced mbi size 0x%x", size);
    for (tag = (struct multiboot_tag *) (addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
    {
        printk (KERN_INFO,"Tag 0x%x, Size 0x%x", tag->type, tag->size);
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_CMDLINE:
                printd (KERN_INFO,"Command line = %s",
                        ((struct multiboot_tag_string *) tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                printd (KERN_INFO,"Boot loader name = %s",
                        ((struct multiboot_tag_string *) tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                printd (KERN_INFO, "Module at 0x%x-0x%x. Command line %s",
                        ((struct multiboot_tag_module *) tag)->mod_start,
                        ((struct multiboot_tag_module *) tag)->mod_end,
                        ((struct multiboot_tag_module *) tag)->cmdline);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                printd (KERN_INFO, "mem_lower = %uKB, mem_upper = %uKB",
                        ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
                        ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                printd (KERN_INFO, "Boot device 0x%x,%u,%u",
                        ((struct multiboot_tag_bootdev *) tag)->biosdev,
                        ((struct multiboot_tag_bootdev *) tag)->slice,
                        ((struct multiboot_tag_bootdev *) tag)->part);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP: {
                init_memory(get_memory_map_from_grub(tag));
                break;
            }
            case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
                init_debugging_infos(tag);
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
                /*multiboot_uint32_t color;
                unsigned i;
                struct multiboot_tag_framebuffer *tagfb
                        = (struct multiboot_tag_framebuffer *) tag;
                void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;

                switch (tagfb->common.framebuffer_type)
                {
                    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
                    {
                        unsigned best_distance, distance;
                        struct multiboot_color *palette;

                        palette = tagfb->framebuffer_palette;

                        color = 0;
                        best_distance = 4*256*256;

                        for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
                        {
                            distance = (0xff - palette[i].blue)
                                       * (0xff - palette[i].blue)
                                       + palette[i].red * palette[i].red
                                       + palette[i].green * palette[i].green;
                            if (distance < best_distance)
                            {
                                color = i;
                                best_distance = distance;
                            }
                        }
                    }
                        break;

                    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                        //color = ((1 << tagfb->framebuffer_blue_mask_size) - 1)
                        //        << tagfb->framebuffer_blue_field_position;
                        break;

                    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                        //color = '\\' | 0x0100;
                        break;

                    default:
                        //color = 0xffffffff;
                        break;
                }

                for (i = 0; i < tagfb->common.framebuffer_width
                            && i < tagfb->common.framebuffer_height; i++)
                {
                    switch (tagfb->common.framebuffer_bpp)
                    {
                        case 8:
                        {
                            multiboot_uint8_t *pixel = fb
                                                       + tagfb->common.framebuffer_pitch * i + i;
                            *pixel = color;
                        }
                            break;
                        case 15:
                        case 16:
                        {
                            multiboot_uint16_t *pixel
                                    = fb + tagfb->common.framebuffer_pitch * i + 2 * i;
                            *pixel = color;
                        }
                            break;
                        case 24:
                        {
                            multiboot_uint32_t *pixel
                                    = fb + tagfb->common.framebuffer_pitch * i + 3 * i;
                            *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                        }
                            break;

                        case 32:
                        {
                            multiboot_uint32_t *pixel
                                    = fb + tagfb->common.framebuffer_pitch * i + 4 * i;
                            *pixel = color;
                        }
                            break;
                    }
                }*/
                break;
            }
        }
    }
}


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