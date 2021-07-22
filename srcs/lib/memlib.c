/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/22 19:59:11 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memlib.h"
#include "../kernel/memory/heap/heap.h"
#include "../kernel/memory/memory.h"

extern uint32_t endKernel;

void	*memset(void *s, int c, size_t n)
{
	char	*t;
	int		i;

	t = (char *)s;
	i = 0;
	while (i != (int)n)
		t[i++] = c;
	return (t);
}

void	*memcpy(void *s1, void const *s2, size_t n)
{
	int		i;
	char	*t1;
	char	*t2;

	i = 0;
	t1 = (char *)s1;
	t2 = (char *)s2;
	while (i != (int)n)
	{
		t1[i] = t2[i];
		i++;
	}
	return (t1);
}

mem_page_tracking_t *first_node_addr = 0;

void *kmalloc(uint32_t size) {
	if (first_node_addr)
		return (void *)first_fit_memory(first_node_addr, size)->addr_low;
	else {
		first_node_addr = first_fit_memory(0, size);
		return (void *)first_node_addr->addr_low;
	}
}
