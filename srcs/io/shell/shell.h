/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:12:16 by lubenard          #+#    #+#             */
/*   Updated: 2021/12/03 18:38:38 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# define SHELL_HIST_SIZE 5
# define SHELL_CMD_SIZE 128
# define SHELL_REAL_HIST_SIZE SHELL_HIST_SIZE - 1

typedef struct shell {
	// Screen buffer when backup is needed
	uint16_t buffer[2000];
	unsigned short start_cmd_line;
	// Command line buffer
	char cmd_line[SHELL_HIST_SIZE][SHELL_CMD_SIZE];
	// Used to remember where we are in history
	unsigned short cmd_hist_size;
	unsigned short cmd_hist_curr;
	// Command line size
	unsigned short cmd_size;
	// Cursor position relative to cmd_line
	unsigned short cursor_pos;
	unsigned short is_shell_init;
}				shell_t;

typedef struct terminal {
			shell_t *first;
			shell_t *second;
			shell_t *third;
			shell_t *active_shell;
}				terminal_t;

void init_shell();

#endif
