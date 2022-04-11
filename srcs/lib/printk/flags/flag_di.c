/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_di.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 18:28:21 by lubenard          #+#    #+#             */
/*   Updated: 2022/04/11 12:25:57 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/printk.h"

void	flag_di(const char *str, int *i, va_list *ap, t_output *output)
{
	int arg;
	int minwidth;

	arg = va_arg(*ap, int);
	minwidth = get_minwidth_size(str, i);
	if (minwidth)
		apply_minwidth_int(output, minwidth, &arg);
	fill_buffer_nbr(output, arg);
	(*i)++;
}
