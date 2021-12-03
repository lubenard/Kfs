/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2_keyboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:53 by lubenard          #+#    #+#             */
/*   Updated: 2021/12/02 17:32:28 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../kernel/isr/isr.h"
#include "../../drivers/vga/vga.h"
#include "../../io/io.h"
#include "PS2_keyboard.h"
#include "../../lib/iolib.h"

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

static char *locale_code_kbd[] = {
	"QWERTY",
	"AZERTY"
};

int kbd_language;
int shift_status = 0;
kbd_event_t last_typed_key;
unsigned short is_key_multiple;

/*
 * Set keyboard map
 */
char *set_language(int language)
{
	kbd_language = language - 1;
	return (locale_code_kbd[kbd_language]);
}

/*
 * Transform scancode into actual keycode
 */
char translate_key(uint8_t scancode) {
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
 * Fill the last_key_typed structure.
 * This structure will be used by shell to get the last key typed
 */
void set_last_key_typed(uint16_t scancode, uint16_t scancode_two,
						short is_key_special) {
	last_typed_key.key_typed = translate_key(scancode);
	last_typed_key.key_typed_raw = scancode;
	last_typed_key.key_typed_raw_two = scancode_two;
	last_typed_key.is_key_special = is_key_special;
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

/*
 * Handle the special keys
 * (all keys not being printable: arrows, shift, capslock)
 */
void handle_special_key(unsigned char scancode, int is_released)
{
	unsigned int second_scancode = 0;

	if (scancode == 0x2A || scancode == 0xAA) // Left shift
		shift_status = !is_released;
	else if (scancode == 0x3A) { // Caps lock
		shift_status = !shift_status;
		// If caps lock is enabled, set caps lock led, else disable it
		outb(0x60, 0xED);
		if (!shift_status)
			outb(0x60, 4);
		else
			outb(0x60, 0);
	} else if (scancode == 0xE0) {
			is_key_multiple = 1;
			second_scancode = inb(0x60);
			if (second_scancode > 0x6c)
				is_released = 1;
	}
	if (is_released == 0)
		set_last_key_typed(scancode, second_scancode, 1);
	if (is_key_multiple == 1 && is_released) {
		is_key_multiple = 0;
	}
}

/*
 * Detect if the key is 'special'
 */
int is_special_key(unsigned char scancode)
{
	if (scancode == 0x01 || scancode == 0x0E || scancode == 0x1D
		|| scancode == 0x2A || scancode == 0xAA || scancode == 0x36
		|| scancode == 0x38 || scancode == 0x3A || scancode == 0xBA
		|| scancode == 0xE0 || (scancode >= 0x3B && scancode <= 0x46))
		return 1;
	return 0;
}

/*
 * Function to get last key typed
 * Once this function is called, it reset the last key to 0
 * To avoid getting the last same key multiple time
 */
void get_last_typed_key(kbd_event_t *key)
{
	*key = last_typed_key;
	// Reset last typed key
	last_typed_key.key_typed = 0;
	last_typed_key.key_typed_raw = 0;
	last_typed_key.key_typed_raw_two = 0;
	last_typed_key.is_key_special = 0;
}

/*
 * Function called by IRQ when key is delivered
 */
void get_key(registers_t regs)
{
	(void)regs;
	/* Read from the keyboard's data buffer */
    uint16_t scancode = inb(0x60);

	/* If the top bit of the byte we read from the keyboard is
	 *  set, that means that a key has just been released
	 */
	if (is_special_key(scancode)) {
		if (scancode & 0x80 && scancode <= 0xD8)
			handle_special_key(scancode, 1);
		else
			handle_special_key(scancode, 0);
	} else if (!is_key_multiple) {
		if (scancode & 0x80)
			return ;
		else
			set_last_key_typed(scancode, 0, 0);
	}
}

/*
 * Init keyboard.
 * Set default map to 2 (Azerty mapping)
 * and register getkey into IRQ system
 */
void init_kbd()
{
	printk(KERN_INFO, "Kbd init");
	is_key_multiple = 0;
	set_language(2);
	register_interrupt_handler(1, &get_key);
}
