/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 23:59:46 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/07 18:08:12 by lubenard         ###   ########.fr       */
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

void move_cursor(unsigned short pos)
{
	outb(TT_PORT_COMMANDE, TT_COMMANDE_OCTET_SUP);
	outb(TT_PORT_DATA, ((pos >> 8) & 0x00FF));
	outb(TT_PORT_COMMANDE, TT_COMMANDE_OCTET_INF);
	outb(TT_PORT_DATA, pos & 0x00FF);
}

/*
 * Erase all the content of the terminal
 */
void clear_terminal() {
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

/*
 * The color of the character is contained in it's 8 highest bytes.
 * We need to extract them to restore the color when moving a character up
 * The color is contained as 4 first bytes for background color, and 4 bytes for
 * fg_color.
 *
 * How it works:
 * The character 'a' in cyan on blue background
 * 0001 0011 0110 0001
 *  |    |    |    |
 *  |    |    --------- 97 in decimal ('a' in ascii table)
 *  |    -------------- 3 in decimal (color for cyan foreground)
 *  ------------------- 1 in decimal (color for blue background)
 * More infos here: https://wiki.osdev.org/Printing_To_Screen
 */
void set_character_color(uint16_t character)
{
	int character_color = ((1 << 4) - 1) & (character >> 8);
	terminal_color = character_color;
}

void move_screen_up()
{
	// Move all character by one line up
	for (size_t y = 1; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index - VGA_WIDTH] = terminal_buffer[index];
		}
	}
	// Fill the last line with space
	// We are forced to restore with terminal_color, or it broke the cursor
	// (It become black, and invisible (black on black background))
	for (size_t i = 0; i < VGA_WIDTH; i++) {
		terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(' ', terminal_color);
	}
	// Set cursor position to first character of last line
	terminal_row = VGA_HEIGHT - 1;
	terminal_column = 0;
	move_cursor((VGA_HEIGHT - 1) * VGA_WIDTH);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
	move_cursor(index + 1);
}

/*
 * Print a string on the terminal
 * @param c The character to print
 */
void terminal_writec(const char c)
{
	if (c == '\n') {
		terminal_row++;
		terminal_column = 0;
		move_cursor(terminal_row * VGA_WIDTH + terminal_column);
		if (terminal_row >= VGA_HEIGHT)
			move_screen_up();
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				move_screen_up();
		}
	}
}

/*
 * Print a string on the terminal
 * @param data The string to print
 */
void terminal_writestr(const char *data)
{
	size_t size = strlen(data);
	for (size_t i = 0; i < size; i++)
		terminal_writec(data[i]);
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
	clear_terminal();
}

