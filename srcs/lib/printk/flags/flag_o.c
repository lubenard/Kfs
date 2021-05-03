/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_o.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 17:57:08 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/03 15:15:35 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/printk.h"

char	*ft_itoa_ulong(unsigned long long nbr)
{
	(void)nbr;
	return "42";
}

char	*convert_into_octal(unsigned long long value)
{
	unsigned long long	i;
	unsigned long long	octal_num;

	octal_num = 0;
	i = 1;
	while (value != 0)
	{
		octal_num += (value % 8) * i;
		value /= 8;
		i *= 10;
	}
	return (ft_itoa_ulong(octal_num));
}

void	flag_o(const char *str, int *i, va_list *ap, t_output *output)
{
	char	*arg;
	short	len_arg;

	(void)str;
	arg = convert_into_octal(va_arg(*ap, unsigned int));
	len_arg = strlen(arg);
	fill_buffer_str(output, arg);
	(*i)++;
}
