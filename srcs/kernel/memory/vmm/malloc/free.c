/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 11:21:03 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/23 21:14:09 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "../vmm.h"

extern t_alloc *g_curr_node;

#include "../../../../lib/iolib.h"

void reorganize_pointer(t_block *node) {
	t_block  *cur_block;
	t_alloc *first_alloc_of_bloc;
	t_alloc *last_alloc_of_bloc;

	cur_block = node;
	first_alloc_of_bloc = (t_alloc *)((char *)cur_block + STRUCT_BLOCK_SIZE + 1);

	if (node->next)
		last_alloc_of_bloc = ((t_alloc *)((char*)node->next + STRUCT_BLOCK_SIZE + 1))->prev;
	else
		last_alloc_of_bloc = 0;

	if (first_alloc_of_bloc && first_alloc_of_bloc->prev)
		first_alloc_of_bloc->prev->next = (last_alloc_of_bloc) ? last_alloc_of_bloc->next : 0;
	if (last_alloc_of_bloc && last_alloc_of_bloc->next)
		last_alloc_of_bloc->next->prev = first_alloc_of_bloc->prev;

	if (cur_block->prev)
		cur_block->prev->next = cur_block->next;
	if (cur_block->next)
		cur_block->next->prev = cur_block->prev;
}

int		check_real_freed_nodes(t_block *node) {
	t_alloc *first_alloc = (t_alloc *)((char*)node + STRUCT_BLOCK_SIZE + 1);
	int total_freed_node = 0;
	int total_node = 0;

	while (first_alloc->block == node) {
		if (first_alloc->block != node) {
			break;
		}
		if (first_alloc->is_busy == ALLOC_FREE)
			total_freed_node++;
		total_node++;
		first_alloc = first_alloc->next;
	}

	if (total_node == 0)
		total_node = 1;
	node->total_node = total_node;
	return total_freed_node;
}

void	check_block_to_free(t_alloc *alloc) {
	t_block *block_tmp;

	block_tmp = alloc->block;

	while (block_tmp->prev)
		block_tmp = block_tmp->prev;

	while (block_tmp) {
		if (g_curr_node->block == block_tmp) {
			block_tmp = block_tmp->next;
			continue;
		}
		if (check_real_freed_nodes(block_tmp) == (int)block_tmp->total_node) {
			//check_real_freed_nodes(block_tmp);
			reorganize_pointer(block_tmp);
			munmap(block_tmp, block_tmp->total_size + 1);
			return;
		}
		block_tmp = block_tmp->next;
	}
}

void	real_free(void *ptr) {
	t_alloc *node_ptr;

	if (ptr == 0)
		return;
	node_ptr = (t_alloc *)((char *) ptr - STRUCT_SIZE - 1);
	if (node_ptr->buffer_overflow == MAGIC_NUMBER && node_ptr->is_busy != ALLOC_FREE) {
		node_ptr->is_busy = ALLOC_FREE;
		if (node_ptr->block->total_node == 0)
			node_ptr->block->total_node = 1;
		if (node_ptr->block->next || node_ptr->block->prev) {
			check_block_to_free(node_ptr);
		}
	} else
		printk(KERN_NORMAL, "Invalid free\n");
}

void free(void *ptr) {
	real_free(ptr);
}
