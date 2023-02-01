/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/22 00:05:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:35:04 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VGA_H
# define VGA_H

# include "../../lib/stddef.h"
# include <stdint.h>

/* Hardware text mode color constants. */
# define VGA_COLOR_BLACK 0
# define VGA_COLOR_BLUE 1
# define VGA_COLOR_GREEN 2
# define VGA_COLOR_CYAN 3
# define VGA_COLOR_RED 4
# define VGA_COLOR_MAGENTA 5
# define VGA_COLOR_BROWN 6
# define VGA_COLOR_LIGHT_GREY 7
# define VGA_COLOR_DARK_GREY 8
# define VGA_COLOR_LIGHT_BLUE 9
# define VGA_COLOR_LIGHT_GREEN 10
# define VGA_COLOR_LIGHT_CYAN 11
# define VGA_COLOR_LIGHT_RED 12
# define VGA_COLOR_LIGHT_MAGENTA 13
# define VGA_COLOR_LIGHT_BROWN 14
# define VGA_COLOR_WHITE 15

// BY default, this is the size of VGA screen for x86.
// This screen size is now deprecated, but is supported by any x86 computer
# define VGA_WIDTH 80
# define VGA_HEIGHT 25

typedef struct vga_screen {
	size_t terminal_row;
	size_t terminal_column;
	uint8_t terminal_color;
	unsigned short cursor_pos;
}				vga_screen_t;

void terminal_initialize(void);
unsigned short terminal_writestr(const char *data);
void terminal_writec(const char c);
void terminal_dellastnchars(unsigned short chars);
void terminal_dellastchar();
void terminal_clear();
void terminal_set_fg_color(int new_color);
void terminal_set_bg_color(int new_color);
void move_prec_cursor(unsigned short pos);
void move_cursor(short rel_pos);
void move_buffer_right(unsigned short cursor_pos);
void move_buffer_left(unsigned short cursor_pos);
void define_vga_coordonates(size_t x, size_t y);
vga_screen_t get_screen_datas();

#endif
