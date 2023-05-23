/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:39:44 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/18 01:40:44 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include "../grub/grub.h"
# include "../processes/processes.h"
# include <stdint.h>

/**
 * This is the structure of the page directory as a enum.
 * Bitfield has been considered, but suffer from major issues
 * (non-portability and compiler dependent implementation mainly)
 */
enum e_page_directory_flags {
    PD_PRESENT,
    PD_RW,
    PD_USER,
    PD_WRITE_THROUGH,
    PD_CACHE_DISABLE,
    PD_ACCESSED,
    PD_AVL,
    PD_PAGESIZE,
    PD_AVAILABLE2,
    PD_ADDR = 12
};

enum e_page_table_flags {
    PT_PRESENT,
    PT_RW,
    PT_USER,
    PT_WRITE_THROUGH,
    PT_CACHE_DISABLE,
    PT_ACCESSED,
    PT_DIRTY,
    PT_PAT,
    PT_GLOBAL,
    PT_AVAILABLE,
    PT_ADDR = 12
};

typedef struct s_memory_infos {
	// Thoses values are in bytes;
	unsigned long size;
	unsigned long used_size;
} t_memory_infos;

void init_memory(multiboot2_memory_map_t *map_entry);
void init_memory_infos();

void map_page(void *addr, void *page_directory);
void unmap_page(void *addr, void *page_directory);
void copy_kernel_to_process_page_directory(t_page_directory *process_page_directory);
void init_pd_and_map_kernel(void *start_addr, uint32_t nframes);


void update_memory_infos(unsigned long size, unsigned long used_size);
t_memory_infos *get_memory_infos();

/* ASM called functions */
extern void enable_paging(unsigned int *);
extern void flush_tlb();

#endif
