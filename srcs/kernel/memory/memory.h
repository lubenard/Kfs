/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:39:44 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/23 04:43:36 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stdint.h>

# define INDEX_FROM_BIT(a) (a / (8 * 4))
# define OFFSET_FROM_BIT(a) (a % (8 * 4))

/* The magic field should contain this. */
# define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

/* This should be in %eax. */
# define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

# define PAGE_SIZE 4096

/*
 * This is bitfield:
 * See more here :
 * https://stackoverflow.com/questions/3186008/in-c-what-does-a-colon-mean-inside-a-declaration
 */
typedef struct page {
	uint32_t present    : 1;   // Page present in memory
	uint32_t rw         : 1;   // Read-only if clear, readwrite if set
	uint32_t user       : 1;   // Supervisor level only if clear
	uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
	uint32_t dirty      : 1;   // Has the page been written to since last refresh?
	uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
	uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table {
	page_t pages[1024];
} page_table_t;

typedef struct page_directory {
	page_table_t *tables[1024];
	/* Physical addr of tables */
	uint32_t tablesPhysical[1024];
	/*
	 * The physical address of tablesPhysical. This comes into play
	 * when we get our kernel heap allocated and the directory
	 * may be in a different location in virtual memory.
	 */
	uint32_t physicalAddr;
} page_directory_t;

void init_memory();

void alloc_frame(page_t *page, int is_kernel, int is_writeable);

page_t *get_page(uint32_t address, int make, page_directory_t *dir);

void setFrames(uint32_t *new_frames, uint32_t nframes);

uint32_t e_kmalloc(uint32_t size, int align, uint32_t *phys);

void set_placement_addr(uint32_t new_placement_address);

/* ASM called functions */
extern void enable_paging(unsigned int *);
extern void flush_tlb();

#endif
