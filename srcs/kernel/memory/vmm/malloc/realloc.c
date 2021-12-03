/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 11:22:31 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/23 17:41:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "malloc.h"
#include "../../../../lib/iolib.h"
#include "../../../../lib/memlib.h"

extern t_alloc *g_curr_node;

void	*realloc(void *ptr, size_t size) {
	void *ptr_realloc;
	t_alloc *node_ptr;
	size_t size_to_copy;

	if (ptr == NULL) {
		return (real_malloc(size));
	}
	node_ptr = (t_alloc *)((char *) ptr - STRUCT_SIZE - 1);
	if (node_ptr->buffer_overflow == MAGIC_NUMBER && node_ptr->is_busy == 2) {
		size_to_copy = ((int)size < node_ptr->size) ? size : node_ptr->size - STRUCT_SIZE;
		ptr_realloc = real_malloc(size);
		memcpy(ptr_realloc, ptr, size_to_copy);
		real_free(ptr);
	} else
		return NULL;
	return ptr_realloc;
}
