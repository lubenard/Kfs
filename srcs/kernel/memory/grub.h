/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grub.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 19:38:48 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/03 19:42:51 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRUB_H
# define GRUB_H

#include <stdint.h>

typedef struct multiboot_framebuffer_palette {
	uint32_t framebuffer_palette_addr;
	uint16_t framebuffer_palette_num_colors;
}			multiboot_framebuffer_palette_t;

typedef struct multiboot_framebuffer {
	uint8_t framebuffer_red_field_position;
	uint8_t framebuffer_red_mask_size;
	uint8_t framebuffer_green_field_position;
	uint8_t framebuffer_green_mask_size;
	uint8_t framebuffer_blue_field_position;
	uint8_t framebuffer_blue_mask_size;
}			multiboot_framebuffer_t;

/* The symbol table for a.out. */
typedef struct multiboot_aout_symbol_table {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
}			multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct multiboot_elf_section_header_table {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
}			multiboot_elf_section_header_table_t;

/* This pointer for this struct is supposed to be in framebuffer_palette_addr */
struct multiboot_color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

typedef struct multiboot_info {
	/* Multiboot info version number */
	uint32_t flags;

	/* Available memory from BIOS */
	uint32_t mem_lower;
	uint32_t mem_upper;

	/* "root" partition */
	uint32_t boot_device;

	/* Kernel command line */
	uint32_t cmdline;

	/* Boot-Module list */
	uint32_t mods_count;
	uint32_t mods_addr;

	union {
		multiboot_aout_symbol_table_t aout_sym;
		multiboot_elf_section_header_table_t elf_sec;
	} u;

	/* Memory Mapping buffer */
	uint32_t mmap_length;
	uint32_t mmap_addr;

	/* Drive Info buffer */
	uint32_t drives_length;
	uint32_t drives_addr;

	/* ROM configuration table */
	uint32_t config_table;

	/* Boot Loader Name */
	uint32_t boot_loader_name;

	/* APM table */
	uint32_t apm_table;

	/* Video */
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;

	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;

	union {
		multiboot_framebuffer_palette_t framebuffer_palette;
		multiboot_framebuffer_t framebuffer;
	};
} multiboot_info_t;

struct multiboot_mmap_entry {
	uint32_t size;
	uint32_t addr_low;
	uint32_t addr_high;
	uint32_t len_low;
	uint32_t len_high;
	uint32_t type;
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

multiboot_memory_map_t *get_memory_map_from_grub(multiboot_info_t *mb_mmap);

#endif
