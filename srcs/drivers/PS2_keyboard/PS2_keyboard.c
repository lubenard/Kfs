/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2_keyboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:53 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/12 21:59:07 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../kernel/isr/isr.h"
#include "../../lib/lib.h"
#include "../../drivers/vga/vga.h"
#include "../../io/io.h"

static const char qwertAsciiTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 0, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',
	']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
	'`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
	0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5',
	'6', '+', '1', '2', '3', '0', '.'
};

static const char qwertAsciiTableUp[] = {
	0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
	0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
	'0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const char azertAsciiTable[] = {
	0, 0, '&', '~', '"', '\'', '(', '-', '`', '_', '^', '@', ')', '=', 0,
	'\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
	0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '%', 0, 0, '*',
	'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, '*',
	0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5',
	'6', '+', '1', '2', '3', '0', '.'
};

static const char azertAsciiTableUp[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ']', '+', 0,
	'\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '^', '$', '\n',
	0, 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', '*',
	0, '>', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', '!',
	0, '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3',
	'0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const char *kbd_languages[] = {
	qwertAsciiTable,
	azertAsciiTable
};

static const char *kbd_languages_up[] = {
	qwertAsciiTableUp,
	azertAsciiTableUp
};


int kbd_language;
int shift_status = 0;

void set_language(int language)
{
	kbd_language = language;
}

char translateKey(uint8_t scancode) {
	char keycode;

	if (scancode > 0x58)
		return 0;
	if (shift_status)
		keycode = kbd_languages_up[kbd_language][scancode];
	else
		keycode = kbd_languages[kbd_language][scancode];
	return (keycode);
}

/*
 * ------------------------------------------------------------------------;
 * _________________        LED status byte:                               ;
 * |0|0|0|0|0|1|1|1|                                                       ;
 * +---------------+                     1 = True   0 = False              ;
 *            | | +--->  scroll lock                                       ;
 *            | +----->  num lock                                          ;
 *            +------->  caps lock                                         ;
 * ------------------------------------------------------------------------;
 * Schema found here: https://forum.osdev.org/viewtopic.php?t=10053
 */

void handle_special_key(unsigned char scancode, int is_released)
{
	/*if (scancode == 0x01) // Escape character
		terminal_writestr("ESC");
	else if (scancode == 0x0E) // Delete character
		terminal_dellastchar();
	else*/ if (scancode == 0x2A || scancode == 0xAA) // Left shift
		shift_status = !is_released;
	else if (scancode == 0x3A) { // Caps lock
		shift_status = !shift_status;
		// If caps lock is enabled, set caps lock led, else disable it
		outb(0x60, 0xED);
		if (!shift_status)
			outb(0x60, 4);
		else
			outb(0x60, 0);
	}
}

int is_special_key(unsigned char scancode)
{
	if (scancode == 0x01 || scancode == 0x0E || scancode == 0x1D
		|| scancode == 0x2A || scancode == 0xAA || scancode == 0x36
		|| scancode == 0x38 || scancode == 0x3A || scancode == 0xBA)
		return 1;
	return 0;
}

void get_key(registers_t regs)
{
	(void)regs;

	/* Read from the keyboard's data buffer */
    uint16_t scancode = inb(0x60);

	/* If the top bit of the byte we read from the keyboard is
	 *  set, that means that a key has just been released
	 */
	if (is_special_key(scancode)) {
		if (scancode & 0x80)
			handle_special_key(scancode, 1);
		else
			handle_special_key(scancode, 0);
	} else {
		if (scancode & 0x80)
			return ;
		else {
			terminal_writec(translateKey(scancode));
		}
	}
}

void init_kbd()
{
	set_language(1);
	register_interrupt_handler(1, &get_key);
}
