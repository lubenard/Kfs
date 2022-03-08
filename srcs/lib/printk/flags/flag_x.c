/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_x.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 17:46:27 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/08 18:29:12 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/printk.h"

void	convert_into_hexa(unsigned long value, char ret[17])
{
	int			i;
	long long	tmp;

	i = 0;
	if (value == 0)
	{
		ret[0] = '0';
		return ;
	}
	while (value != 0)
	{
		tmp = 0;
		tmp = value % 16;
		if (tmp < 10)
			ret[i] = tmp + 48;
		else
			ret[i] = tmp + 87;
		++i;
		value = value / 16;
	}
}

void	flag_x(const char *str, int *i, va_list *ap, t_output *output)
{
	char	hex_arg[17];
	unsigned int arg;
	int minwidth;

	bzero(hex_arg, 17);
	arg = va_arg(*ap, unsigned int);
	convert_into_hexa(arg, hex_arg);
	minwidth = get_minwidth_size(str, i);
	if (minwidth)
		apply_minwidth_hex(output, minwidth, hex_arg);
	fill_buffer_rev(output, hex_arg);
	(*i)++;
}

void	flag_maj_x(const char *str, int *i, va_list *ap, t_output *output)
{
	char hex_arg[17];
	unsigned int arg;
	int minwidth;

	bzero(hex_arg, 17);
	arg = va_arg(*ap, unsigned int);
	convert_into_hexa(arg, hex_arg);
	minwidth = get_minwidth_size(str, i);
	if (minwidth)
		apply_minwidth_hex(output, minwidth, hex_arg);
	strupper(hex_arg);
	fill_buffer_rev(output, hex_arg);
	(*i)++;
}
