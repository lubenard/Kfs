/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printk.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 19:26:18 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/02 16:06:49 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include "include/printk.h"
#include "../lib.h"
#include "../../drivers/vga/vga.h"

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
		parsing(str, ap);
		va_end(ap);
	}
	terminal_writestr("\n");
	terminal_set_fg_color(VGA_COLOR_LIGHT_GREY);
}
