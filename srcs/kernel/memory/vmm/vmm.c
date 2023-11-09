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
	pmm_unset_pages(ptr, size);
}

// TODO: use flags and get CR3 instead
void *mmap(unsigned int size, int flags, void *custom_page_directory) {
	return pmm_next_fit(size, flags, custom_page_directory);
}
