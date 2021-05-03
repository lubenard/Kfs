/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_x.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 17:46:27 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/03 15:15:57 by lubenard         ###   ########.fr       */
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
	char	arg[17];

	(void)str;
	bzero(arg, 17);
	convert_into_hexa(va_arg(*ap, unsigned int), arg);
	fill_buffer_rev(output, arg);
	(*i)++;
}

void	flag_maj_x(const char *str, int *i, va_list *ap, t_output *output)
{
	char	arg[17];

	(void)str;
	bzero(arg, 17);
	convert_into_hexa(va_arg(*ap, unsigned int), arg);
	strupper(arg);
	fill_buffer_rev(output, arg);
	(*i)++;
}
