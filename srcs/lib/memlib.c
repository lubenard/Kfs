/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/06/21 07:48:45 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memlib.h"
#include "../kernel/memory/memory.h"

extern uint32_t end;
uint32_t placement_address = (uint32_t)&end;

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

/*
 * Early malloc. Used to create the linked list for memory management
 */
uint32_t e_kmalloc(uint32_t size, int align, uint32_t *phys) {
	if (align == 1 && (placement_address & 0xFFFFF000)) {
		// Align address with physical addr if needed.
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys)
		*phys = placement_address;
	uint32_t tmp = placement_address;
	placement_address += size;
	return tmp;
}

void *malloc(long unsigned size) {
	(void)size;
	//mem_page_tracking_t *lknd_list_head = get_mem_lknd_list_head();
	/*while (lknd_list_head->is_allocated == 1) {
		lknd_list_head = lknd_list_head->next;
	}*/
	return (0);
}
