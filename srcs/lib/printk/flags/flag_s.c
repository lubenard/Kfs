/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_s.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 17:43:48 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/27 18:18:27 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/printk.h"

void	flag_s(const char *str, int *i, va_list ap, t_output *output)
{
	char	*arg;

	(void)str;
	arg = va_arg(ap, char *);
	if (arg == NULL)
		arg = "(null)";
	fill_buffer_str(output, arg);
	(*i)++;
}
