/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 23:59:46 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/27 16:30:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include "../../lib/lib.h"
#include "../../io/io.h"
#include "vga.h"

/* Les ports d'E/S */
#define TT_PORT_COMMANDE        0x3D4
#define TT_PORT_DATA            0x3D5

/* Les commandes du port d'E/S */
#define TT_COMMANDE_OCTET_SUP    14
#define TT_COMMANDE_OCTET_INF    15

// BY default, this is the size of VGA screen for x86.
// This screen size is now deprecated, but is supported by any x86 computer
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

static int fg_color;
static int bg_color;

static inline uint8_t vga_entry_color(int fg_color, int bg_color)
{
	return fg_color | bg_color << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void move_cusor(unsigned short pos)
{
	outb(TT_PORT_COMMANDE, TT_COMMANDE_OCTET_SUP);
	outb(TT_PORT_DATA, ((pos >> 8) & 0x00FF));
	outb(TT_PORT_COMMANDE, TT_COMMANDE_OCTET_INF);
	outb(TT_PORT_DATA, pos & 0x00FF);
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	// default coloration for the terminal
	fg_color = VGA_COLOR_LIGHT_GREY;
	bg_color = VGA_COLOR_BLACK;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	// The first adress of the VGA buffer is 0xB8000.
	terminal_buffer = (uint16_t*) 0xB8000;
	// We write ' ' on each character for the buffer size
	size_t index;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_set_fg_color(int new_fg_color)
{
	fg_color = new_fg_color;
	terminal_color = vga_entry_color(fg_color, bg_color);
}

void terminal_set_bg_color(int new_bg_color)
{
	bg_color = new_bg_color;
	terminal_color = vga_entry_color(fg_color, bg_color);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	move_cusor(index + 1);
}

void move_screen_up()
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = terminal_buffer[y + 1 * VGA_WIDTH + x];
		}
	}
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				move_screen_up();
		}
	}
}

void terminal_writestr(const char *data)
{
	size_t size = strlen(data);
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

