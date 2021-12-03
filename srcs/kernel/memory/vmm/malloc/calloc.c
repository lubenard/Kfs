/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 13:38:02 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/23 17:40:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "../../../../lib/strlib.h"

void	*calloc(size_t nitems, size_t size) {
	void	*ptr;

	size *= nitems;
	if (!(ptr = real_malloc(size + 1)))
		return (NULL);
	bzero(ptr, size + 1);
	return (ptr);
}
