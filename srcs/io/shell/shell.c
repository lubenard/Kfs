#include "../../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../../drivers/vga/vga.h"
#include "../../lib/lib.h"

char input_buffer[256];
static unsigned int index;

void	clear_buffer() {
	for (int i = 0; i < 256; i++) {
		input_buffer[i] = 0;
	}
	index = 0;
}

void print_help() {
	terminal_writestr("Commands available:\n");
	terminal_writestr("\thelp - print this help\n");
	terminal_writestr("\tkbd language - set kbd map: 1 for QWERT, 2 for AZERTY\n");
}

void	handle_input() {
	if (strcmp(input_buffer, "help") == 0)
		print_help();
	else if (strncmp(input_buffer, "kbd language", 12) == 0) {
		unsigned short new_language;
		if (!(new_language = atoi(&input_buffer[13])))
			printk(KERN_ERROR, "Bad input: please enter valid numbers");
		else {
			if (new_language > 0 && new_language < 3)
				printk(KERN_INFO, "Keyboard layout is now %s", set_language(new_language));
			else
				printk(KERN_INFO, "Invalid option: 1 for QWERTY map, 2 for AZERTY map");
		}
	} else {
		printk(KERN_ERROR, "Command not found");
		print_help();
	}
}

void handle_special_keys(kbd_event_t key) {
	if (key.key_typed_raw_two == 0x4D)
		move_cursor(1);
	else if (key.key_typed_raw_two == 0x4B)
		move_cursor(-1);
}

void	init_shell() {
	terminal_writestr("Shell > ");
	kbd_event_t key;
	index = 0;
	while (1) {
		get_last_typed_key(&key);
		if (key.key_typed != 0 && key.is_key_special == 0) {
			terminal_writec(key.key_typed);
			if (key.key_typed == '\n') {
				handle_input();
				clear_buffer();
				terminal_writestr("Shell > ");
			} else
				input_buffer[index++] = key.key_typed;
		} else if (key.is_key_special) {
			if (key.key_typed_raw == 0x0E) {
				terminal_dellastchar();
				input_buffer[index--] = 0;
			}
			if (key.key_typed_raw == 0xE0)
				handle_special_keys(key);
		}
	}
}
