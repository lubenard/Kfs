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

void	init_shell() {
	terminal_writestr("Shell > ");
	unsigned char key = 0;
	index = 0;
	while (1) {
		get_last_typed_key(&key);
		if (key != 0) {
			terminal_writec(key);
			if (key == '\n') {
				if (strcmp(input_buffer, "help") == 0)
					terminal_writestr("help - print this help\n");
				clear_buffer();
				terminal_writestr("Shell > ");
			} else {
				input_buffer[index++] = key;
			}
		}
	}
}
