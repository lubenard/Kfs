/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2_keyboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:53 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/04 23:11:16 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../io/io.h"
#include "../../drivers/vga/vga.h"
#include "../../io/isr/isr.h"
#include "../../lib/lib.h"

static const char qwertAsciiTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 0, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',
	']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
	'`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
	0, ' '
};

static const char azertAsciiTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 0, '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^',
	'$', '\n', 0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '%',
	0, 0, '*', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, '*',
	0, ' '
};

static const char *kbd_languages[] = {
	qwertAsciiTable,
	azertAsciiTable
};

int kbd_language;
int shift_status = 0;

void set_language(int language)
{
	kbd_language = language;
}

char translateKey(uint8_t scancode) {
	char keycode;

	if (scancode > 58)
		return 0;
	keycode = kbd_languages[kbd_language][scancode];
	if (shift_status)
		keycode -= 32;
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
	if (scancode == 0x01)
		terminal_writestr("ESC");
	else if (scancode == 0x0E)
		terminal_writestr("DEL");
	else if (scancode == 0x2A || scancode == 0xAA)
		shift_status = !is_released;
	else if (scancode == 0x3A || scancode == 0xBA) {
		shift_status = !is_released;
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

void get_key(registers_t *regs)
{
	(void)regs;

	/* Read from the keyboard's data buffer */
    unsigned char scancode = inb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
	if (scancode & 0x80) {
		if (is_special_key(scancode))
			handle_special_key(scancode, 1);
	} else {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
		if (is_special_key(scancode))
			handle_special_key(scancode, 0);
		else
			terminal_writec(translateKey(scancode));
	}
}

void init_kbd()
{
	register_interrupt_handler(1, get_key);

	// Set default settings to the keyboard
	outb(0x60, 0xF6);

	set_language(1);
}
