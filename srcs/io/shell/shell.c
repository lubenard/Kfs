/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:19:47 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/19 15:33:10 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../../drivers/vga/vga.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../lib/strlib.h"
#include "../io.h"
#include "shell.h"
#include "builtins/builtins.h"

uint32_t esp;
uint32_t ebp;

void print_help() {
	terminal_writestr("Commands available:\n");
	terminal_writestr("\thelp - print this help\n");
	terminal_writestr("\tkbd language - set kbd map: 1 for QWERTY, 2 for AZERTY\n");
	terminal_writestr("\tprint_stack - Print the current stack\n");
	terminal_writestr("\techo - echo the content\n");
	terminal_writestr("\tclear - clear the screen\n");
	terminal_writestr("\tshutdown - shutdown the computer\n");
	terminal_writestr("\treboot - reboot the computer\n");
}

/*
 * Decide witch command should be launched based on input
 */
void	handle_input(shell_t *shell) {
	if (strcmp(shell->cmd_line, "help") == 0)
		print_help();
	else if (strncmp(shell->cmd_line, "kbd language", 12) == 0)
		change_kbd_map(shell->cmd_line);
	else if (strcmp(shell->cmd_line, "clear") == 0)
		terminal_clear();
	else if (strcmp(shell->cmd_line, "shutdown") == 0)
		shutdown();
	else if (strcmp(shell->cmd_line, "reboot") == 0)
		reboot();
	else if (strcmp(shell->cmd_line, "print_stack") == 0)
		print_stack(esp, ebp);
	else if (strncmp(shell->cmd_line, "echo", 4) == 0)
		printk(KERN_NORMAL, "%s\n", &shell->cmd_line[5]);
	else {
		printk(KERN_ERROR, "Command not found: %s", shell->cmd_line);
		print_help();
	}
}

/*
 * Handle arrow keys
 */
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

/*
 * Change the active shell by changing pointer
 */
void change_active_shell(terminal_t *terminal,
						unsigned short new_active_shell) {
	if (new_active_shell == 0)
		terminal->active_shell = terminal->first;
	if (new_active_shell == 1)
		terminal->active_shell = terminal->second;
	if (new_active_shell == 2)
		terminal->active_shell = terminal->third;
}

/*
 * Get the whole screen into a backup buffer
 */
void copy_screen_into_buffer(shell_t *shell, vga_screen_t datas) {
	uint16_t *screen_buffer = (uint16_t*) 0xB8000;
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		shell->buffer[i] = screen_buffer[i];
	shell->start_cmd_line = datas.cursor_pos - shell->cmd_size;
}

/*
 * Handle the management switching screens
 * Backup the screen into a buffer
 * Change active shell
 * Restore new active shell datas and screen_buffer
 */
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

/*
 * Useful when deleting character into the line
 */
void move_input_buffer_left(shell_t *shell) {
	int i = shell->cursor_pos - 1;
	while (i != 254) {
		shell->cmd_line[i] = shell->cmd_line[i + 1];
		i++;
	}
}

/*
 * Useful when inserting character into the line
 */
void move_input_buffer_right(shell_t *shell) {
	int i = 254;
	while (i != shell->cursor_pos - 1) {
		shell->cmd_line[i + 1] = shell->cmd_line[i];
		i--;
	}
}

/*
 * Infinite loop to handle keystrokes and input
 */
void wait_for_input(terminal_t terminal) {
	kbd_event_t key;
	terminal.active_shell->start_cmd_line = terminal_writestr("Shell > ");
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
				if (terminal.active_shell->cmd_size < 256) {
					move_buffer_right(terminal.active_shell->start_cmd_line + terminal.active_shell->cursor_pos);
					terminal_writec(key.key_typed);
					move_input_buffer_right(terminal.active_shell);
					terminal.active_shell->cmd_line[terminal.active_shell->cursor_pos] = key.key_typed;
					terminal.active_shell->cmd_size++;
					terminal.active_shell->cursor_pos++;
				}
			}
		} else if (key.is_key_special) {
			if (key.key_typed_raw == DELETE_KEY) {
				if (terminal.active_shell->cmd_size > 0) {
					move_buffer_left(terminal.active_shell->start_cmd_line + terminal.active_shell->cursor_pos);
					move_input_buffer_left(terminal.active_shell);
					terminal.active_shell->cursor_pos--;
					terminal.active_shell->cmd_size--;
					move_cursor(-1);
				}
			}
			if (key.key_typed_raw == 0xE0) // Arrow keys
				handle_special_keys(terminal.active_shell, key);
			if (key.key_typed_raw == F1_KEY)
				load_shell(&terminal, 0);
			else if (key.key_typed_raw == F2_KEY)
				load_shell(&terminal, 1);
			else if (key.key_typed_raw == F3_KEY)
				load_shell(&terminal, 2);
		}
	}
}

/*
 * Init shell
 */
void	init_shell() {
	asm volatile ("movl %%esp,%0" : "=r"(esp) ::);
	asm volatile ("movl %%ebp,%0" : "=r"(ebp) ::);
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
	terminal.active_shell->cursor_pos = 0;
	wait_for_input(terminal);
}
