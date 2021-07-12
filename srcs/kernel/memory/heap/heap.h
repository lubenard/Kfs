/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 22:08:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/12 16:04:31 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

#include <stdint.h>

# define MAGIC_NUMBER_MEMORY                     0x1234AB

typedef struct mem_page_tracking {
	struct mem_page_tracking *prev;
	uint32_t addr_low;
	uint32_t addr_high;
	uint32_t len_low;
	uint32_t len_high;
	short is_allocated;
	unsigned int owner_pid;
	struct mem_page_tracking *next;
}				mem_page_tracking_t;

mem_page_tracking_t *create_new_node_memory(uint32_t addr_low, uint32_t size, uint32_t pid);
void delete_node_memory(mem_page_tracking_t *node);
mem_page_tracking_t *first_fit_memory(mem_page_tracking_t *lknd_lst, unsigned int size);

#endif
