/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:24:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/24 11:48:39 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memlib.h"

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
