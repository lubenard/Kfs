/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 17:25:18 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/16 22:16:13 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "../include/printk.h"
#include "../../../drivers/vga/vga.h"

void	flag_percent(const char *str, int *i, va_list *ap, t_output *output)
{
	char	arg;

	(void)*ap;
	(void)str;
	arg = '%';
	fill_buffer(output, arg, i);
	//*i += flags.flag_lenght - 1;
}

void	flag_c(const char *str, int *i, va_list *ap, t_output *output)
{
	char	arg;

	(void)str;
	arg = va_arg(*ap, int);
	if (isprint(arg)) {
		fill_buffer(output, arg, i);
		//(*i)++;
	}
	else
		(*i)++;
}
