/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:39:44 by lubenard          #+#    #+#             */
/*   Updated: 2022/01/07 14:21:16 by lubenard         ###   ########.fr       */
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

void init_memory();

void map_page(void *addr);
void unmap_page(void *addr);
void init_pd_and_map_kernel(void *start_addr, uint32_t nframes);
int check_mapping(void *addr);

/* ASM called functions */
extern void enable_paging(unsigned int *);
extern void flush_tlb();

#endif
