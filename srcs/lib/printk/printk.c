/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 19:26:18 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/15 14:20:51 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "include/printk.h"
#include "../lib.h"
#include "../../drivers/vga/vga.h"

/*
 * We are forced to pass va_list with address or it will only print one arg.
 * Normally, the va_arg update the index to fetch the next argument.
 * However, it seems there is a diference of type between i386 and amd64 for 
 * va_list format.
 * If in amd64, just passing the struct is okay and update the index, we need 
 * to pass the address in i386
 * See here for more infos:
 * https://stackoverflow.com/a/52356204/9108386
 */

void printk(int info_type, const char *str, ...) {
	va_list ap;

	if (info_type == KERN_INFO) {
		terminal_set_fg_color(VGA_COLOR_CYAN);
		terminal_writestr("[INFO] ");
	}
	else if (info_type == KERN_ERROR) {
		terminal_set_fg_color(VGA_COLOR_RED);
		terminal_writestr("[ERROR] ");
	}
	if (str != NULL) {
		va_start(ap, str);
		parsing(str, &ap);
		va_end(ap);
	}
	if (info_type != KERN_NORMAL) {
		terminal_writec('\n');
		terminal_set_fg_color(VGA_COLOR_LIGHT_GREY);
	}
}
