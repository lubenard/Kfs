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

size_t shell_strncmp(uint16_t const *s1, char const *s2, size_t n) {
	size_t i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	if (n == 0)
		return (0);
	uint16_t character;
	while (s1[i] && s2[i] && i < n - 1) {
		character = s1[i] & 0x000000FF;
		if (character != s2[i])
			break;
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	handle_input(shell_t *shell) {
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	uint16_t *start_of_line = (uint16_t*)&screen_buffer[shell->start_cmd_line];
	/*for (int i = 0; i != shell->cmd_size; i++)
		terminal_writec(screen_buffer[shell->start_cmd_line + i]);
	terminal_writec('\n');
	printk(KERN_INFO, "strncmp = %d", shell_strncmp(start_of_line, "help", 4));*/
	if (shell_strncmp(start_of_line, "help", 3) == 0)
		print_help();
	/*else if (shell_strncmp(start_of_line, "kbd language", 12) == 0) {
		unsigned short new_language;
		if (!(new_language = atoi(&input_buffer[13])))
			printk(KERN_ERROR, "Bad input: please enter valid numbers");
		else {
			if (new_language > 0 && new_language < 3)
				printk(KERN_INFO, "Keyboard layout is now %s", set_language(new_language));
			else
				printk(KERN_INFO, "Invalid option: 1 for QWERTY map, 2 for AZERTY map");
		}
	} */else if (shell_strncmp(start_of_line, "clear", 5) == 0)
		terminal_clear();
	else {
		printk(KERN_ERROR, "Command not found");
		print_help();
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

void change_active_shell(terminal_t *terminal,
						unsigned short new_active_shell) {
	if (new_active_shell == 0)
		terminal->active_shell = terminal->first;
	if (new_active_shell == 1)
		terminal->active_shell = terminal->second;
	if (new_active_shell == 2)
		terminal->active_shell = terminal->third;
}

void copy_screen_into_buffer(shell_t *shell, vga_screen_t datas) {
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		shell->buffer[i] = screen_buffer[i];
	shell->cursor_pos = datas.cursor_pos;
}

void load_shell(terminal_t *terminal, unsigned short new_shell_to_load) {
	vga_screen_t datas = get_screen_datas();
	copy_screen_into_buffer(terminal->active_shell, datas);
	terminal_clear();
	change_active_shell(terminal, new_shell_to_load);
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	if (terminal->active_shell->is_shell_init) {
		for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
			screen_buffer[i] = terminal->active_shell->buffer[i];
		move_prec_cursor(terminal->active_shell->cursor_pos);
	} else {
		terminal_writestr("Shell > ");
		terminal->active_shell->is_shell_init = 1;
	}
}

void wait_for_input(terminal_t terminal) {
	kbd_event_t key;
	terminal.active_shell->start_cmd_line = terminal_writestr("Shell > ");
	terminal.active_shell->cursor_pos = terminal.first->start_cmd_line;
	while (1) {
		get_last_typed_key(&key);
		if (key.key_typed != 0 && key.is_key_special == 0) {
			if (key.key_typed == '\n') {
				terminal_writec('\n');
				handle_input(terminal.first);
				terminal.active_shell->cmd_size = 0;
				terminal.active_shell->start_cmd_line = terminal_writestr("Shell > ");
				terminal.active_shell->cursor_pos = terminal.first->start_cmd_line;
			} else {
				terminal.active_shell->cmd_size++;
				//if (should_move_buffer) {
					move_buffer_right(terminal.active_shell->cursor_pos);
					terminal_writec(key.key_typed);
					terminal.active_shell->cursor_pos++;
				/*}
				else {
					terminal_writec(key.key_typed);
					terminal.first->cursor_pos++;
				}*/
			}
		} else if (key.is_key_special) {
			if (key.key_typed_raw == 0x0E) {
				move_buffer_left(terminal.active_shell->cursor_pos);
				terminal.active_shell->cursor_pos--;
				terminal.active_shell->cmd_size--;
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
	terminal.active_shell = &first;
	memset(terminal.first, 0, sizeof(shell_t));
	memset(terminal.second, 0, sizeof(shell_t));
	memset(terminal.third, 0, sizeof(shell_t));
	terminal.first->is_shell_init = 1;
	wait_for_input(terminal);
}
