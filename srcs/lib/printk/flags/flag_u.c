/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_u.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 18:07:13 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/03 15:15:51 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/printk.h"
#include <limits.h>

char	*ft_itoa_uint(unsigned int nbr)
{
	(void)nbr;
	return "42";
}

unsigned int	negative_number(unsigned int nbr)
{
	if (nbr > UINT_MAX)
		return (UINT_MAX);
	return (nbr);
}

void			flag_u(const char *str, int *i, va_list *ap, t_output *output)
{
	char	*arg;

	(void)str;
	arg = ft_itoa_uint(negative_number(va_arg(*ap, unsigned int)));
	fill_buffer_str(output, arg);
	(*i)++;
}
