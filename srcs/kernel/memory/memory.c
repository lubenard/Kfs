/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:47:20 by lubenard          #+#    #+#             */
/*   Updated: 2022/10/13 15:00:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/bitwiselib.h"
#include "grub/grub.h"
#include "pmm/pmm.h"
#include "vmm/vmm.h"

# define MB 1048576
# define GB 1073741824

extern uint32_t endKernel;
extern uint32_t startKernel;

uint32_t static end_kernel_addr = (uint32_t) &endKernel;
uint32_t static start_kernel_addr = (uint32_t) &startKernel;

uint32_t get_kernel_size() {
	return end_kernel_addr - start_kernel_addr;
}

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

typedef uint32_t Elf32_Word;
typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off;

typedef struct {
        Elf32_Word      sh_name;
        Elf32_Word      sh_type;
        Elf32_Word      sh_flags;
        Elf32_Addr      sh_addr;
        Elf32_Off       sh_offset;
        Elf32_Word      sh_size;
        Elf32_Word      sh_link;
        Elf32_Word      sh_info;
        Elf32_Word      sh_addralign;
        Elf32_Word      sh_entsize;
} Elf32_Shdr;

/*static inline Elf32_Shdr *elf_sheader(multiboot_elf_section_header_table_t *hdr) {
    return (Elf32_Shdr *)((int)hdr->addr + hdr->shndx);
}*/

/*static inline Elf32_Shdr *elf_section(multiboot_elf_section_header_table_t *hdr, int idx) {
    return &elf_sheader(hdr)[idx];
}*/

/*static inline char *elf_str_table(multiboot_elf_section_header_table_t *hdr) {
	return (char *)hdr + elf_section(hdr, hdr->shndx)->sh_offset;
}

static inline char *elf_lookup_string(multiboot_elf_section_header_table_t *hdr, int offset) {
	char *strtab = elf_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}*/

void init_memory(unsigned long addr) {
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
         tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag
                                         + ((tag->size + 7) & ~7)))
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
            case MULTIBOOT_TAG_TYPE_MMAP:
            {
                uint32_t nframes;
                multiboot2_memory_map_t *map_entry = get_memory_map_from_grub(tag);
                if (map_entry) {
                    // Getting the size of the memory via grub.
                    // nframes is the number of frames (aka blocks of 4096)
                    nframes = (map_entry->len_low / 0x1000); // Ox1000 is 4096 in hexa (Size of one page)
                    printk(KERN_INFO, "Memory is %d Gb and %d Mb", map_entry->len_low / GB, (map_entry->len_low % GB) / MB);

                    init_memory_infos();
                    update_memory_infos(map_entry->len_low, 0);

                    printk(KERN_INFO, "Kernel is %d bytes, aka %d pages, aka %d page table", get_kernel_size(), get_kernel_size() / 4096, (get_kernel_size() / 4096) / 1024);
                    printk(KERN_INFO, "Kernel start at %p, and end at %p", start_kernel_addr, end_kernel_addr);

                    //printd(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);
                    printd(KERN_INFO, "nframes is %d pages, aka %d page table", nframes, nframes / 1024);

                    void *start_pd = (void *)roundUp((char *)end_kernel_addr + 1, 4096);

                    void *end_pd = (char *)start_pd + (nframes * sizeof(uint32_t));

                    printk(KERN_INFO, "Page tables should begin at %p and end at %p", start_pd, end_pd);

                    init_pd_and_map_kernel(start_pd, nframes);

                    // Init physical memory manager
                    void *start_real_memory = create_pmm_array(end_pd + 1, nframes);

                    update_memory_infos(map_entry->len_low, (size_t)start_real_memory - (size_t)map_entry->addr_low);
                    printk(KERN_INFO, "Memory usage is %d MB %d KB", (get_memory_infos())->used_size / MB, (get_memory_infos()->used_size % MB) / 1024);
                }
            }
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            {
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

	/*

	printk(KERN_INFO, "mb_mmap = 0x%x", mb_mmap);

	if (CHECK_FLAG (mb_mmap->flags, 4)) {
		multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mb_mmap->u.aout_sym);

		printk(KERN_INFO, "multiboot_aout_symbol_table: tabsize = 0x%x, "
				"strsize = 0x%x, addr = 0x%x",
				(unsigned) multiboot_aout_sym->tabsize,
				(unsigned) multiboot_aout_sym->strsize,
				(unsigned) multiboot_aout_sym->addr);
	}

	if (CHECK_FLAG (mb_mmap->flags, 5)) {
		multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mb_mmap->u.elf_sec);

		printk(KERN_INFO, "multiboot_elf_sec: num = %u, size = %d bytes,"
				" addr = 0x%x (%d), shndx = %d",
				(unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
				(unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->addr,
				(unsigned) multiboot_elf_sec->shndx);

		Elf32_Shdr *list = elf_sheader(multiboot_elf_sec);
		printk(KERN_INFO, "Section should be at: 0x%x", elf_sheader(multiboot_elf_sec));
		int i = 0;
		while (i < 10) {
			printk(KERN_INFO, "Section[%d] at addr %p : %s, size = %d", i, list, list->sh_name, list->sh_size);
			list = (Elf32_Shdr *) ((int) list + multiboot_elf_sec->size);
			i++;
		}
	}

	// USED FOR TEST
	map_page((void*)0x400000);
	// Do not cause page fault
	uint32_t *ptr1 = (uint32_t*)0x400000;
	uint32_t not_page_fault = *ptr1;
	(void)not_page_fault;*/

}
