/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 19:26:18 by lubenard          #+#    #+#             */
/*   Updated: 2022/04/11 12:22:58 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "include/printk.h"
#include "../../drivers/vga/vga.h"
#include "../iolib.h"

void print(int info_type, const char *str, va_list *args) {
	if (info_type == KERN_INFO) {
		terminal_set_fg_color(VGA_COLOR_CYAN);
		terminal_writestr("[INFO] ");
	} else if (info_type == KERN_ERROR) {
		terminal_set_fg_color(VGA_COLOR_RED);
		terminal_writestr("[ERROR] ");
	} else if (info_type == KERN_WARNING) {
		terminal_set_fg_color(VGA_COLOR_BROWN);
		terminal_writestr("[WARNING] ");
	}
	parsing(str, args);
	if (info_type != KERN_NORMAL) {
		terminal_writec('\n');
		serial_writestring(0x3F8, "\n");
		terminal_set_fg_color(VGA_COLOR_LIGHT_GREY);
	}
}

/*
 * Printf variant for debug
 */
void printd(int info_type, const char *str, ...) {
	va_list args;

	if (DEBUG_LOG) {
		va_start(args, str);
		print(info_type, str, &args);
		va_end(args);
	}
}

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
	va_list args;

	if (str != NULL) {
		va_start(args, str);
		print(info_type, str, &args);
		va_end(args);
	}
}
