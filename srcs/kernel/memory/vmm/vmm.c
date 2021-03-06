/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 21:32:58 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/23 21:02:15 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vmm.h"
#include "../pmm/pmm.h"

void munmap(void *ptr, unsigned int size) {
	(void)ptr;
	(void)size;
	pmm_unset_pages(ptr, size);
	return ;
}

void *mmap(unsigned int size, int flags) {
	(void)size;
	(void)flags;
	return pmm_next_fit(size, flags);
}
