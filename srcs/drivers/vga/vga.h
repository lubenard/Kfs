/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 00:05:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/12 19:48:40 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Hardware text mode color constants. */
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14
#define VGA_COLOR_WHITE 15

// BY default, this is the size of VGA screen for x86.
// This screen size is now deprecated, but is supported by any x86 computer
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void terminal_initialize(void);
void terminal_writestr(const char *data);
void terminal_writec(const char c);
void terminal_dellastchar();
void terminal_clear();
void terminal_set_fg_color(int new_color);
void terminal_set_bg_color(int new_color);
