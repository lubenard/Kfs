/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 23:59:46 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/16 23:59:02 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include "../../io/io.h"
#include "vga.h"
#include "../../lib/strlib.h"

/* Les ports d'E/S */
#define TT_PORT_COMMANDE        0x3D4
#define TT_PORT_DATA            0x3D5

/* Les commandes du port d'E/S */
#define TT_COMMANDE_OCTET_SUP    14
#define TT_COMMANDE_OCTET_INF    15

vga_screen_t vga_screen;
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

void move_prec_cursor(unsigned short pos)
{
	if (pos < 0)
		vga_screen.cursor_pos = VGA_WIDTH * VGA_HEIGHT;
	else if (pos > VGA_WIDTH * VGA_HEIGHT)
		vga_screen.cursor_pos = 0;
	else
		vga_screen.cursor_pos = pos;
	outb(TT_PORT_COMMANDE, TT_COMMANDE_OCTET_SUP);
	outb(TT_PORT_DATA, ((vga_screen.cursor_pos>> 8) & 0x00FF));
	outb(TT_PORT_COMMANDE, TT_COMMANDE_OCTET_INF);
	outb(TT_PORT_DATA, vga_screen.cursor_pos & 0x00FF);
}

void move_cursor(short rel_pos)
{
	vga_screen.cursor_pos += rel_pos;
	move_prec_cursor(vga_screen.cursor_pos);
}

vga_screen_t get_screen_datas() {
	return vga_screen;
}

/*
 * Erase all the content of the terminal
 */
void terminal_clear() {
	// We write ' ' on each character for the buffer size
	size_t index;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', vga_screen.terminal_color);
		}
	}
	vga_screen.terminal_row = 0;
	vga_screen.terminal_column = 0;
	vga_screen.cursor_pos = 0;
}

void terminal_set_fg_color(int new_fg_color)
{
	fg_color = new_fg_color;
	vga_screen.terminal_color = vga_entry_color(fg_color, bg_color);
}

void terminal_set_bg_color(int new_bg_color)
{
	bg_color = new_bg_color;
	vga_screen.terminal_color = vga_entry_color(fg_color, bg_color);
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
	vga_screen.terminal_color = character_color;
}

void define_vga_coordonates(size_t x, size_t y) {
	vga_screen.terminal_column = y;
	vga_screen.terminal_row = x;
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
		terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_entry(' ', vga_screen.terminal_color);
	}
	// Set cursor position to first character of last line
	vga_screen.terminal_row = VGA_HEIGHT - 1;
	vga_screen.terminal_column = 0;
	move_prec_cursor((VGA_HEIGHT - 1) * VGA_WIDTH);
}

void terminal_putentryat(char c, uint8_t color, unsigned short pos)
{
	terminal_buffer[pos] = vga_entry(c, color);
}

/*
 * Print a string on the terminal
 * @param c The character to print
 */
void terminal_writec(const char c)
{
	if (c == '\n') {
		vga_screen.terminal_row++;
		vga_screen.terminal_column = 0;
		move_prec_cursor(vga_screen.terminal_row * VGA_WIDTH + vga_screen.terminal_column);
		if (vga_screen.terminal_row >= VGA_HEIGHT)
			move_screen_up();
	} else if (c == '\t') {
		terminal_writestr("    ");
	} else {
		terminal_putentryat(c, vga_screen.terminal_color, vga_screen.cursor_pos);
		move_cursor(1);
		if (++vga_screen.terminal_column == VGA_WIDTH) {
			vga_screen.terminal_column = 0;
			if (++vga_screen.terminal_row == VGA_HEIGHT)
				move_screen_up();
		}
	}
}

/*
 * Useful when deleting character into the line
 */
void move_buffer_left(unsigned short cursor_pos) {
	int i = cursor_pos - 1;
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	while (i != 1999) {
		screen_buffer[i] = screen_buffer[i + 1];
		i++;
	}
}

/*
 * Useful when inserting character into the line
 */
void move_buffer_right(unsigned short cursor_pos) {
	int i = 1999;
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	while (i != cursor_pos - 1) {
		screen_buffer[i + 1] = screen_buffer[i];
		i--;
	}
}

void terminal_dellastchar() {
	if (vga_screen.terminal_column == 0) {
		vga_screen.terminal_row--;
		vga_screen.terminal_column = VGA_WIDTH;
	} else
		vga_screen.terminal_column--;
	move_cursor(-1);
	terminal_putentryat(' ', vga_screen.terminal_color, vga_screen.cursor_pos);
}

void terminal_dellastnchars(unsigned short chars)
{
	unsigned short i = 0;
	while (i != chars) {
		terminal_dellastchar();
		i++;
	}
}

/*
 * Print a string on the terminal
 * @param data The string to print
 */
unsigned short terminal_writestr(const char *data)
{
	size_t size = strlen(data);
	for (size_t i = 0; i < size; i++)
		terminal_writec(data[i]);
	return vga_screen.terminal_row * VGA_WIDTH + vga_screen.terminal_column;
}

void terminal_initialize(void)
{
	vga_screen.terminal_row = 0;
	vga_screen.terminal_column = 0;
	// default coloration for the terminal
	fg_color = VGA_COLOR_LIGHT_GREY;
	bg_color = VGA_COLOR_BLACK;
	vga_screen.terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	// The first adress of the VGA buffer is 0xB8000.
	terminal_buffer = (uint16_t*) 0xB8000;
	terminal_clear();
}
