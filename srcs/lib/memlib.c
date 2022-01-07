/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:32 by lubenard          #+#    #+#             */
/*   Updated: 2022/01/07 17:09:13 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memlib.h"

/**
 * Return the next 'aligned' address on b.
 * Param void *a is the base address.
 * Param size_t b is the alignement wanted.
 * Exemple: a= 0x123456, b= 4096
 * Return:  124000
 * Cast the return as a pointer
 */
size_t roundUp(void *a, size_t b) {
	return (1 + ((size_t)a - 1) / b) * b;
}

/**
 * Return the diff between given address (a), and next aligned address based on b
 * Parameter are the same as roundUp function
 */
size_t roundUpDiff(void *a, size_t b) {
	return (size_t)roundUp(a, b) - (size_t)a;
}

void	*memset(void *s, int c, size_t n) {
	char	*t;
	int		i;

	t = (char *)s;
	i = 0;
	if (!s)
		return 0;
	while (i != (int)n) {
		t[i++] = c;
	}
	return (t);
}

void	*memcpy(void *s1, void const *s2, size_t n) {
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
