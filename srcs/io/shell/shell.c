/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:19:47 by lubenard          #+#    #+#             */
/*   Updated: 2022/04/11 14:35:54 by lubenard         ###   ########.fr       */
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
#include "../../kernel/memory/vmm/malloc/malloc.h"

uint32_t esp;
uint32_t ebp;

terminal_t *terminal = 0;

void check_term_struct() {
	if (terminal != 0) {
	printk(KERN_INFO, "-------------------------------");
	printk(KERN_INFO, "terminal is located at %p", terminal);
	printk(KERN_INFO, "terminal->active is located at %p", terminal->active_shell);
	printk(KERN_INFO, "terminal->first is located at %p", terminal->first);
	printk(KERN_INFO, "terminal->active->cmd_size= %d", terminal->active_shell->cmd_size);
	printk(KERN_INFO, "-------------------------------");
	}
}

void move_command_hist_up(t_shell *shell, unsigned short limit) {
	unsigned short i = 0;
	while (i < limit) {
		strlcpy(shell->cmd_line[i], shell->cmd_line[i + 1],
				strlen(shell->cmd_line[i + 1]));
	 i++;
	}
	memset(shell->cmd_line[limit], 0, SHELL_CMD_SIZE);
}

int		search_builtin(char *command) {
	int j = 0;
	while (builtins_names[j]) {
		if (strcmp(command, builtins_names[j]) == 0) {
			return j;
		}
		j++;
	}
	return -1;
}

/*
 * Decide witch command should be launched based on input
 */
void	handle_input(t_shell *shell) {
	int builtin_index = 0;
	t_command *command_struc;
	char **current_line = ft_strsplit(shell->cmd_line[shell->cmd_hist_curr], ' ');

	if ((builtin_index = search_builtin(current_line[0])) == -1) {
		printk(KERN_ERROR, "Could not find command named: %s", current_line[0]);
		// Help is expecting a structure, but is not using it.
		// Passing 0 is fine
		help(0);
	} else {
		if (!(command_struc = malloc(sizeof(t_command)))) {
			printk(KERN_ERROR, "Malloc failed for %s", current_line[0]);
		} else {
			command_struc->esp = esp;
			command_struc->ebp = ebp;
			command_struc->command = current_line;
			command_struc->current_shell = shell;

			g_builtins_array[builtin_index](command_struc);
		}
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
void handle_special_keys(t_shell *shell, kbd_event_t *key) {
	short rel_pos = 0;
	if (key->key_typed_raw_two == 0x4D || key->key_typed_raw_two == 0x4B) {
		if (key->key_typed_raw_two == 0x4D) // Right arrow
			rel_pos = 1;
		else if (key->key_typed_raw_two == 0x4B) // Left arrow
			rel_pos = -1;
		// Avoid cursor going before the prompt or after the cmd
		if (shell->cursor_pos + rel_pos < 0 ||
			shell->cursor_pos + rel_pos > shell->cmd_size)
			rel_pos = 0;
		move_cursor(rel_pos);
		shell->cursor_pos += rel_pos;
	} else {
		if (key->key_typed_raw_two == 0x48) { // Up arrow
			if (shell->cmd_hist_curr > shell->cmd_hist_size) {
				shell->cmd_hist_curr--;
				terminal_dellastnchars(shell->cmd_size);
				terminal_writestr(shell->cmd_line[shell->cmd_hist_curr]);
				shell->cmd_size = strlen(shell->cmd_line[shell->cmd_hist_curr]);
				shell->cursor_pos = shell->cmd_size;
			}
		} else if (key->key_typed_raw_two == 0x50) { // Down arrow
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
void copy_screen_into_buffer(t_shell *shell, vga_screen_t datas) {
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
void load_shell(terminal_t *terminal, unsigned short new_t_shello_load) {
	vga_screen_t datas = get_screen_datas();
	copy_screen_into_buffer(terminal->active_shell, datas);
	terminal_clear();
	change_active_shell(terminal, new_t_shello_load);
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
void move_input_buffer_left(t_shell *shell) {
	int i = shell->cursor_pos - 1;
	while (i != SHELL_CMD_SIZE - 2) {
		shell->cmd_line[shell->cmd_hist_curr][i] = shell->cmd_line[shell->cmd_hist_curr][i + 1];
		i++;
	}
}

/*
 * Useful when inserting character into the line
 */
void move_input_buffer_right(t_shell *shell) {
	int i = SHELL_CMD_SIZE - 2;
	while (i != shell->cursor_pos - 1) {
		shell->cmd_line[shell->cmd_hist_curr][i + 1] = shell->cmd_line[shell->cmd_hist_curr][i];
		i--;
	}
}

/*
 * Infinite loop to handle keystrokes and input
 */
void wait_for_input(kbd_event_t *key) {
	//check_term_struct();
	printk(KERN_INFO, "Key here %c %d, terminal address %p", key->key_typed, key->is_key_special, terminal);
	if (key->key_typed != 0 && key->is_key_special == 0) {
		//terminal.active_shell->start_cmd_line = 0;
		//terminal.active_shell->is_shell_init = 0;
		//terminal.active_shell->cmd_size = 0;
		//terminal.active_shell->cursor_pos = 0;
		if (key->key_typed == '\n') {
			terminal_writec('\n');
			//if (terminal->active_shell->cmd_size != 0)
			//	handle_input(terminal->active_shell);
			terminal->active_shell->cmd_size = 0;
			terminal->active_shell->start_cmd_line = terminal_writestr("Shell > ");
			//printk(KERN_INFO, "terminal is located at %p and active_shell at %p", &terminal, terminal.active_shell);
			//printk(KERN_INFO, "More importatntly, cursor_pos is at %p", &(terminal.active_shell->cursor_pos));
			//printk(KERN_INFO, "Value is %d", terminal.active_shell->cursor_pos);
			terminal->active_shell->cursor_pos = 0;
			//printk(KERN_INFO, "here ? is %d", terminal.active_shell->cursor_pos);
		} else {
			printk(KERN_INFO, "terminal->cmd_size = %d", terminal->active_shell->cmd_size);
			if (terminal->active_shell->cmd_size < 127) {
				//printk(KERN_INFO, "< 127 for %c", key->key_typed);
				move_buffer_right(terminal->active_shell->start_cmd_line
								+ terminal->active_shell->cursor_pos);
				terminal_writec(key->key_typed);
				move_input_buffer_right(terminal->active_shell);
				terminal->active_shell->cmd_line[terminal->active_shell->cmd_hist_curr]
								[terminal->active_shell->cursor_pos] = key->key_typed;
				terminal->active_shell->cmd_size++;
				terminal->active_shell->cursor_pos++;
			}
		}
	} /*else if (key->is_key_special) {
		if (key->key_typed_raw == DELETE_KEY) {
			if (terminal.active_shell->cmd_size > 0) {
				move_buffer_left(terminal.active_shell->start_cmd_line
									+ terminal.active_shell->cursor_pos);
				move_input_buffer_left(terminal.active_shell);
				terminal.active_shell->cursor_pos--;
				terminal.active_shell->cmd_size--;
				move_cursor(-1);
			}
		}
		if (key->key_typed_raw == 0xE0) // Arrow keys
			handle_special_keys(terminal.active_shell, key);
		if (key->key_typed_raw == F1_KEY)
			load_shell(&terminal, 0);
		else if (key->key_typed_raw == F2_KEY)
			load_shell(&terminal, 1);
		//else if (key.key_typed_raw == F3_KEY)
		//	load_shell(&terminal, 2);
	}*/
	//while (1) {}
}

static struct kbd_listener listener_callback = {
	.listener = &wait_for_input,
};

/*
 * Init shell
 */
void	init_shell() {
	//Useful to get stack trace for later
	asm volatile ("movl %%esp,%0" : "=r"(esp) ::);
	asm volatile ("movl %%ebp,%0" : "=r"(ebp) ::);
	terminal_t real_term;
	t_shell first;
	t_shell second;
	t_shell third;

	//memset(&real_term, 0, sizeof(terminal_t));
	//memset(&first, 0, sizeof(t_shell));
	//memset(&second, 0, sizeof(t_shell));
	// Cause weird bug, make crash
	//memset(&third, 0, sizeof(t_shell));
	printk(KERN_INFO, "Terminal is located at %p", &real_term);
	printk(KERN_INFO, "First shell is located at %p, and end at %p (size is %d)", &first, (char*)&first + sizeof(t_shell), sizeof(t_shell));
	printk(KERN_INFO, "function wait_for_input is located at %p", &wait_for_input);
	real_term.first = &first;
	real_term.second = &second;
	real_term.third = &third;
	real_term.first->is_shell_init = 1;
	real_term.active_shell = &first;
	printk(KERN_INFO, "First shell is located at %p", real_term.active_shell);
	real_term.first = &first;
	real_term.active_shell->cursor_pos = 0;
	real_term.active_shell->cmd_hist_size = 4;
	real_term.active_shell->cmd_hist_curr = 4;
	real_term.active_shell->start_cmd_line = terminal_writestr("Shell > ");
	terminal = &real_term;
	//check_term_struct();
	(void)second;
	printk(KERN_INFO, "Before registration");
	register_kbd_listener(&listener_callback);
	printk(KERN_INFO, "After registration");
	//check_term_struct();
	(void)second;
}
