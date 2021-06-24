/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page_allocator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 21:44:56 by lubenard          #+#    #+#             */
/*   Updated: 2021/06/24 15:12:11 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

// Should set the first free page to allocated
mem_page_tracking_t *page_allocator(mem_page_tracking_t *list) {
	mem_page_tracking_t *tmp;

	tmp = list;
	while (tmp->next) {
		if (!tmp->is_allocated) {
			tmp->is_allocated = 1;
			return (tmp);
		}
		tmp = tmp->next;
	}
}

// Free the given page
void free_page(mem_page_tracking_t *list, unsigned int page_to_free) {
	mem_page_tracking_t *tmp;
	unsigned int i;

	tmp = list;
	while (i != page_to_free) {
		tmp = tmp->next;
		i++;
	}
	tmp->is_allocated = 0;
}
