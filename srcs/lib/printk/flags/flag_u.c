/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_u.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 18:07:13 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/08 18:17:51 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/printk.h"
#include <limits.h>

unsigned int	negative_number(unsigned int nbr)
{
	if (nbr > UINT_MAX)
		return (UINT_MAX);
	return (nbr);
}

void			flag_u(const char *str, int *i, va_list *ap, t_output *output)
{
	unsigned int arg;
	int minwidth;

	arg = negative_number(va_arg(*ap, unsigned int));
	minwidth = get_minwidth_size(str, i);
	if (minwidth)
		apply_minwidth_uint(output, minwidth, &arg);
	fill_buffer_nbr_u(output, arg);
	(*i)++;
}
