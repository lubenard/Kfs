/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 16:44:02 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:03:53 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/printk.h"
#include "../../drivers/vga/vga.h"

int find_flag(const char *str, int i)
{
	while (!isalpha(str[i]) && str[i] != '%')
		i++;
	return (i);
}

int replace(const char *str, int *i, va_list *ap, t_output *output)
{
	int j;

	(*i)++;
	if ((j = strchri("csdixX%pou", str[find_flag(str, *i)])) >= 0)
		g_pointer_array[j](str, i, ap, output);
	else
		fill_buffer(output, str[*i], i);
	return (0);
}

int parsing(const char *str, va_list *ap)
{
	t_output	output;
	int			i;

	i = 0;
	bzero(output.buffer, PRINTF_BUFF_SIZE);
	output.size_string = 0;
	output.j = 0;
	while (str[i])
	{
		if (str[i] == '%')
			replace(str, &i, ap, &output);
		else
			fill_buffer(&output, str[i], &i);
	}
	terminal_writestr(output.buffer);
	output.size_string += strlen(output.buffer);
	output.j = 0;
	bzero(output.buffer, PRINTF_BUFF_SIZE);
	return (output.size_string);
}
