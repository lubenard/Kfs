/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 22:08:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/22 21:13:05 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

#include <stdint.h>

# define MAGIC_NUMBER_MEMORY                     0x1234AB
# define KHEAP_START                             0xC0000000
# define KHEAP_MIN_SIZE                          0x1000 // 4096 bytes
# define kHEAP_MAX_SIZE                          0xF4240 // 1 000 000 bytes, 1Mb

typedef struct mem_page_tracking {
	struct mem_page_tracking *prev;
	struct mem_page_tracking *next;
	short is_allocated;
	unsigned int owner_pid;
	struct mem_page_tracking_tail *tail;
	uint32_t len_low;
	unsigned short magic_number;
	uint32_t addr_low;
}	__attribute__((packed))	mem_page_tracking_t;


typedef struct mem_page_tracking_tail {
	unsigned short magic_number;
}	__attribute__((packed))	mem_page_tracking_tail_t;



mem_page_tracking_t *create_new_node_memory(uint32_t addr_low, uint32_t size, uint32_t pid, short should_early_malloc);
void delete_node_memory(mem_page_tracking_t *node);
mem_page_tracking_t *first_fit_memory(mem_page_tracking_t *lknd_lst, unsigned int size);

#endif
