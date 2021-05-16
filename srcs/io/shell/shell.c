/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:19:47 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:19:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../../drivers/vga/vga.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/strlib.h"
#include "../io.h"
#include "shell.h"

void print_help() {
	terminal_writestr("Commands available:\n");
	terminal_writestr("\thelp - print this help\n");
	terminal_writestr("\tkbd language - set kbd map: 1 for QWERTY, 2 for AZERTY\n");
	terminal_writestr("\tclear - clear the screen\n");
	terminal_writestr("\tshutdown - shutdown the computer\n");
	terminal_writestr("\treboot - reboot the computer\n");
}

void	handle_input(shell_t *shell) {
	if (strcmp(shell->cmd_line, "help") == 0)
		print_help();
	else if (strncmp(shell->cmd_line, "kbd language", 12) == 0) {
		unsigned short new_language;
		if (!(new_language = atoi(&shell->cmd_line[13])))
			printk(KERN_ERROR, "Bad input: please enter valid numbers");
		else {
			if (new_language > 0 && new_language < 3)
				printk(KERN_INFO, "Keyboard layout is now %s", set_language(new_language));
			else
				printk(KERN_INFO, "Invalid option: 1 for QWERTY map, 2 for AZERTY map");
		}
	} else if (strcmp(shell->cmd_line, "clear") == 0)
		terminal_clear();
	else if (strcmp(shell->cmd_line, "shutdown") == 0)
		outw(0x604, 0x2000);
	else if (strcmp(shell->cmd_line, "reboot") == 0) {
		// 8242 reset
		uint8_t good = 0x02;
		while (good & 0x02)
			good = inb(0x64);
		outb(0x64, 0xFE);
	}
	else {
		printk(KERN_ERROR, "Command not found: %s", shell->cmd_line);
		print_help();
	}
}

void handle_special_keys(shell_t *shell, kbd_event_t key) {
	short rel_pos = 0;
	if (key.key_typed_raw_two == 0x4D)
		rel_pos = 1;
	else if (key.key_typed_raw_two == 0x4B)
		rel_pos = -1;
	if (shell->cursor_pos + rel_pos < 0)
		rel_pos = 0;
	else if (shell->cursor_pos + rel_pos > shell->cmd_size)
		rel_pos = 0;
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
	shell->start_cmd_line = datas.cursor_pos - shell->cmd_size;
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
		move_prec_cursor(terminal->active_shell->start_cmd_line + terminal->active_shell->cmd_size);
		define_vga_coordonates(terminal->active_shell->start_cmd_line / 80, terminal->active_shell->start_cmd_line % 80);
	} else {
		terminal->active_shell->start_cmd_line = terminal_writestr("Shell > ");
		memset(terminal->active_shell->cmd_line, 0, 256);
		terminal->active_shell->cmd_size = 0;
		terminal->active_shell->cursor_pos = 0;
		terminal->active_shell->is_shell_init = 1;
	}
}

void wait_for_input(terminal_t terminal) {
	kbd_event_t key;
	terminal.active_shell->start_cmd_line = terminal_writestr("Shell > ");
	terminal.active_shell->cursor_pos = 0;
	while (1) {
		get_last_typed_key(&key);
		if (key.key_typed != 0 && key.is_key_special == 0) {
			if (key.key_typed == '\n') {
				terminal_writec('\n');
				if (terminal.active_shell->cmd_size != 0)
					handle_input(terminal.active_shell);
				terminal.active_shell->cmd_size = 0;
				memset(terminal.active_shell->cmd_line, 0, 256);
				terminal.active_shell->start_cmd_line = terminal_writestr("Shell > ");
				terminal.active_shell->cursor_pos = 0;
			} else {
				//if (should_move_buffer) {
					if (terminal.active_shell->cmd_size < 256) {
						move_buffer_right(terminal.active_shell->start_cmd_line + terminal.active_shell->cursor_pos);
						terminal_writec(key.key_typed);
						terminal.active_shell->cmd_line[terminal.active_shell->cmd_size] = key.key_typed;
						terminal.active_shell->cmd_size++;
						terminal.active_shell->cursor_pos++;
					}
				/*}
				else {
					terminal_writec(key.key_typed);
					terminal.first->cursor_pos++;
				}*/
			}
		} else if (key.is_key_special) {
			if (key.key_typed_raw == 0x0E) { // Delete key
				if (terminal.active_shell->cmd_size > 0) {
					move_buffer_left(terminal.active_shell->start_cmd_line + terminal.active_shell->cursor_pos);
					terminal.active_shell->cursor_pos--;
					//terminal.active_shell->cmd_line[terminal.active_shell->] = key.key_typed;
					terminal.active_shell->cmd_size--;
					move_cursor(-1);
				}
			}
			if (key.key_typed_raw == 0xE0) // Arrow keys
				handle_special_keys(terminal.active_shell, key);
			if (key.key_typed_raw == 0x3B) // F1
				load_shell(&terminal, 0);
			else if (key.key_typed_raw == 0x3C) // F2
				load_shell(&terminal, 1);
			else if (key.key_typed_raw == 0x3D) // F3
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
