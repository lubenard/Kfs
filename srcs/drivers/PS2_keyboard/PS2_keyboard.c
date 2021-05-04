/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2_keyboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:53 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/04 21:29:26 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../io/io.h"
#include "../../drivers/vga/vga.h"
#include "../../io/isr/isr.h"

static const char qwertAsciiTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 0, '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',
	']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
	'`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
	0, ' '
};

static const char azertAsciiTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 0, '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[',
	']', '\n', 0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '\'',
	'`', 0, '\\', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, '*',
	0, ' '
};

static const char *kbd_languages[] = {
	qwertAsciiTable,
	azertAsciiTable
};

int kbd_language;

void set_language(int language)
{
	kbd_language = language;
}

char translateKey(uint8_t scancode, int isUppercase) {
	char keycode;

	if (scancode > 58)
		return 0;
	keycode = kbd_languages[kbd_language][keycode];
	//if (isUppercase)
	return (keycode);
}

int shift_status = 0;

void get_key(registers_t *regs)
{
	(void)regs;
	//unsigned char scancode;
    /* Read from the keyboard's data buffer */
    unsigned char scancode = inb(0x60);

    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80) {
        //terminal_writestr("Upper bit is okay\n");
		/* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
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
		if (scancode == 42)
			shift_status = 1;
		else
			terminal_writec(translateKey(scancode, shift_status));
    }
}

void init_kbd()
{
	register_interrupt_handler(1, get_key);
	set_language(0);
}
