/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:19:47 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/25 16:17:25 by lubenard         ###   ########.fr       */
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
	terminal_writestr("\tprint_hist - Print the shell history\n");
	terminal_writestr("\techo - echo the content\n");
	terminal_writestr("\tclear - clear the screen\n");
	terminal_writestr("\tshutdown - shutdown the computer\n");
	terminal_writestr("\treboot - reboot the computer\n");
}

void move_command_hist_up(shell_t *shell, unsigned short limit) {
	unsigned short i = 0;
	while (i < limit) {
		strlcpy(shell->cmd_line[i], shell->cmd_line[i + 1],
				strlen(shell->cmd_line[i + 1]));
	 i++;
	}
	memset(shell->cmd_line[limit], 0, SHELL_CMD_SIZE);
}

void print_history(shell_t *shell) {
	int i = 0;
	int j = 1;
	while (i != 4) {
		if (strlen(shell->cmd_line[i]) > 0)
			printk(KERN_NORMAL, "[%d] %s\n", j++, shell->cmd_line[i]);
		i++;
	}
}

/*
 * Decide witch command should be launched based on input
 */
void	handle_input(shell_t *shell) {
	char *current_cmd;

	current_cmd = shell->cmd_line[shell->cmd_hist_curr];
	if (strcmp(current_cmd, "help") == 0)
		print_help();
	else if (strncmp(current_cmd, "kbd language", 12) == 0)
		change_kbd_map(current_cmd);
	else if (strcmp(current_cmd, "clear") == 0)
		terminal_clear();
	else if (strcmp(current_cmd, "shutdown") == 0)
		shutdown();
	else if (strcmp(current_cmd, "reboot") == 0)
		reboot();
	else if (strcmp(current_cmd, "print_stack") == 0)
		print_stack(esp, ebp);
	else if (strncmp(current_cmd, "echo", 4) == 0)
		printk(KERN_NORMAL, "%s\n", &current_cmd[5]);
	else if (strncmp(current_cmd, "print_history", 4) == 0)
		print_history(shell);
	else {
		printk(KERN_ERROR, "Command not found: %s", current_cmd);
		print_help();
	}
	if (shell->cmd_hist_size > 0)
		shell->cmd_hist_size--;
	if (shell->cmd_hist_curr == SHELL_REAL_HIST_SIZE) {
		move_command_hist_up(shell, SHELL_REAL_HIST_SIZE);
	} else {
		move_command_hist_up(shell, 3);
		strlcpy(shell->cmd_line[3],
				shell->cmd_line[shell->cmd_hist_curr - 1],
				strlen(shell->cmd_line[shell->cmd_hist_curr - 1]));
		shell->cmd_hist_curr = SHELL_REAL_HIST_SIZE;
	}
}

/*
 * Handle arrow keys
 */
void handle_special_keys(shell_t *shell, kbd_event_t key) {
	short rel_pos = 0;
	if (key.key_typed_raw_two == 0x4D || key.key_typed_raw_two == 0x4B) {
		if (key.key_typed_raw_two == 0x4D) // Right arrow
			rel_pos = 1;
		else if (key.key_typed_raw_two == 0x4B) // Left arrow
			rel_pos = -1;
		// Avoid cursor going before the prompt or after the cmd
		if (shell->cursor_pos + rel_pos < 0 ||
			shell->cursor_pos + rel_pos > shell->cmd_size)
			rel_pos = 0;
		move_cursor(rel_pos);
		shell->cursor_pos += rel_pos;
	} else {
		if (key.key_typed_raw_two == 0x48) { // Up arrow
			if (shell->cmd_hist_curr > shell->cmd_hist_size) {
				shell->cmd_hist_curr--;
				terminal_dellastnchars(shell->cmd_size);
				terminal_writestr(shell->cmd_line[shell->cmd_hist_curr]);
				shell->cmd_size = strlen(shell->cmd_line[shell->cmd_hist_curr]);
				shell->cursor_pos = shell->cmd_size;
			}
		} else if (key.key_typed_raw_two == 0x50) { // Down arrow
			if (shell->cmd_hist_curr < SHELL_REAL_HIST_SIZE) {
				shell->cmd_hist_curr++;
				terminal_dellastnchars(shell->cmd_size);
				terminal_writestr(shell->cmd_line[shell->cmd_hist_curr]);
				shell->cmd_size = strlen(shell->cmd_line[shell->cmd_hist_curr]);
				shell->cursor_pos = shell->cmd_size;
			}
		}
	}
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
		memset(terminal->active_shell->cmd_line, 0, SHELL_CMD_SIZE);
		terminal->active_shell->cmd_size = 0;
		terminal->active_shell->cursor_pos = 0;
		terminal->active_shell->is_shell_init = 1;
		terminal->active_shell->cmd_hist_size = 4;
		terminal->active_shell->cmd_hist_curr = 4;
	}
}

/*
 * Useful when deleting character into the line
 */
void move_input_buffer_left(shell_t *shell) {
	int i = shell->cursor_pos - 1;
	while (i != SHELL_CMD_SIZE - 2) {
		shell->cmd_line[shell->cmd_hist_curr][i] = shell->cmd_line[shell->cmd_hist_curr][i + 1];
		i++;
	}
}

/*
 * Useful when inserting character into the line
 */
void move_input_buffer_right(shell_t *shell) {
	int i = SHELL_CMD_SIZE - 2;
	while (i != shell->cursor_pos - 1) {
		shell->cmd_line[shell->cmd_hist_curr][i + 1] = shell->cmd_line[shell->cmd_hist_curr][i];
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
				terminal.active_shell->start_cmd_line = terminal_writestr("Shell > ");
				terminal.active_shell->cursor_pos = 0;
			} else {
				if (terminal.active_shell->cmd_size < 127) {
					move_buffer_right(terminal.active_shell->start_cmd_line
										+ terminal.active_shell->cursor_pos);
					terminal_writec(key.key_typed);
					move_input_buffer_right(terminal.active_shell);
					terminal.active_shell->cmd_line[terminal.active_shell->cmd_hist_curr]
								[terminal.active_shell->cursor_pos] = key.key_typed;
					terminal.active_shell->cmd_size++;
					terminal.active_shell->cursor_pos++;
				}
			}
		} else if (key.is_key_special) {
			if (key.key_typed_raw == DELETE_KEY) {
				if (terminal.active_shell->cmd_size > 0) {
					move_buffer_left(terminal.active_shell->start_cmd_line
										+ terminal.active_shell->cursor_pos);
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
			/*else if (key.key_typed_raw == F3_KEY)
				load_shell(&terminal, 2);*/
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

	memset(&first, 0, sizeof(shell_t));
	memset(&second, 0, sizeof(shell_t));
	// Cause weird bug, make crash
	//memset(&third, 0, sizeof(shell_t));
	terminal.first = &first;
	terminal.second = &second;
	terminal.third = &third;
	terminal.first->is_shell_init = 1;
	terminal.active_shell = terminal.first;
	terminal.active_shell->cursor_pos = 0;
	terminal.active_shell->cmd_hist_size = 4;
	terminal.active_shell->cmd_hist_curr = 4;
	wait_for_input(terminal);
}
