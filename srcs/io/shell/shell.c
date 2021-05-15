#include "../../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../../drivers/vga/vga.h"
#include "../../lib/lib.h"
#include "shell.h"

void print_help() {
	terminal_writestr("Commands available:\n");
	terminal_writestr("\thelp - print this help\n");
	terminal_writestr("\tkbd language - set kbd map: 1 for QWERTY, 2 for AZERTY\n");
	terminal_writestr("\tclear - clear the screen\n");
}

void	handle_input(shell_t *shell) {
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	char start_of_line = (char)screen_buffer[shell->start_cmd_line];
	for (int i = 0; i != shell->cmd_size; i++)
		terminal_writec(screen_buffer[shell->start_cmd_line + i]);
	terminal_writec('\n');
	printk(KERN_INFO, "strncmp = %d", strncmp(&start_of_line, "help", 2));
	//if (strncmp(&start_of_line, "help", 3) == 0)
	//	print_help();
	/*else if (strncmp(&shell.start_cmd_line, "kbd language", 12) == 0) {
		unsigned short new_language;
		if (!(new_language = atoi(&input_buffer[13])))
			printk(KERN_ERROR, "Bad input: please enter valid numbers");
		else {
			if (new_language > 0 && new_language < 3)
				printk(KERN_INFO, "Keyboard layout is now %s", set_language(new_language));
			else
				printk(KERN_INFO, "Invalid option: 1 for QWERTY map, 2 for AZERTY map");
		}
	} else if (strcmp(input_buffer, "clear") == 0)
		terminal_clear();
	*//*else {
		printk(KERN_ERROR, "Command not found");
		//print_help();
	}*/
}

/*
 * Useful when inserting character into the line
 */
void move_buffer_right(shell_t *shell) {
	int i = 1999;
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	while (i != shell->cursor_pos - 1) {
		screen_buffer[i + 1] = screen_buffer[i];
		i--;
	}
}

/*
 * Useful when deleting character into the line
 */
void move_buffer_left(shell_t *shell) {
	int i = shell->cursor_pos - 1;
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	while (i != 1999) {
		screen_buffer[i] = screen_buffer[i + 1];
		i++;
	}
}

void handle_special_keys(shell_t *shell, kbd_event_t key) {
	short rel_pos = 0;
	if (key.key_typed_raw_two == 0x4D)
		rel_pos = 1;
	else if (key.key_typed_raw_two == 0x4B)
		rel_pos = -1;
	move_cursor(rel_pos);
	shell->cursor_pos += rel_pos;
}

shell_t *get_current_active_shell(terminal_t terminal) {
	if (terminal.active_shell == 0)
		return terminal.first;
	if (terminal.active_shell == 1)
		return terminal.second;
	if (terminal.active_shell == 2)
		return terminal.third;
	return 0;
}

void copy_screen_into_buffer(shell_t *shell, vga_screen_t datas) {
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		shell->buffer[i] = screen_buffer[i];
	shell->cursor_pos = datas.cursor_pos;
}

void load_shell(terminal_t *terminal, unsigned short new_shell_to_load) {
	shell_t *shell = get_current_active_shell(*terminal);
	vga_screen_t datas = get_screen_datas();
	copy_screen_into_buffer(shell, datas);
	terminal_clear();
	terminal->active_shell = new_shell_to_load;
	shell = get_current_active_shell(*terminal);
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	if (shell->is_shell_init) {
		for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
			screen_buffer[i] = shell->buffer[i];
		move_prec_cursor(shell->cursor_pos);
	} else {
		terminal_writestr("Shell > ");
		shell->is_shell_init = 1;
	}
}

void wait_for_input(terminal_t terminal) {
	kbd_event_t key;
	terminal.first->start_cmd_line = terminal_writestr("Shell > ");
	terminal.first->cursor_pos = terminal.first->start_cmd_line;
	while (1) {
		get_last_typed_key(&key);
		if (key.key_typed != 0 && key.is_key_special == 0) {
			if (key.key_typed == '\n') {
				terminal_writec('\n');
				handle_input(terminal.first);
				terminal.first->cmd_size = 0;
				terminal.first->start_cmd_line = terminal_writestr("Shell > ");
			} else {
				terminal.first->cmd_size++;
				//if (should_move_buffer) {
					move_buffer_right(get_current_active_shell(terminal));
					terminal_writec(key.key_typed);
					terminal.first->cursor_pos++;
				/*}
				else {
					terminal_writec(key.key_typed);
					terminal.first->cursor_pos++;
				}*/
			}
		} else if (key.is_key_special) {
			if (key.key_typed_raw == 0x0E) {
				move_buffer_left(get_current_active_shell(terminal));
				//terminal_dellastchar();
				terminal.first->cursor_pos--;
				move_cursor(-1);
			}
			if (key.key_typed_raw == 0xE0)
				handle_special_keys(terminal.first, key);
			if (key.key_typed_raw == 0x3B)
				load_shell(&terminal, 0);
			else if (key.key_typed_raw == 0x3C)
				load_shell(&terminal, 1);
			else if (key.key_typed_raw == 0x3D)
				load_shell(&terminal, 2);
		}
	}
}

void	init_shell() {
	terminal_t terminal;
	shell_t first;
	shell_t second;
	shell_t third;

	terminal.first = &first;
	terminal.second = &second;
	terminal.third = &third;
	terminal.active_shell = 0;
	memset(terminal.first, 0, sizeof(shell_t));
	memset(terminal.second, 0, sizeof(shell_t));
	memset(terminal.third, 0, sizeof(shell_t));
	terminal.first->is_shell_init = 1;
	wait_for_input(terminal);
}
