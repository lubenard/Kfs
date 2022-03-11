/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:12:16 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/11 11:47:37 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# define SHELL_HIST_SIZE 5
# define SHELL_CMD_SIZE 128
# define SHELL_REAL_HIST_SIZE SHELL_HIST_SIZE - 1

typedef struct s_shell {
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
}				t_shell;

typedef struct s_terminal {
			t_shell *first;
			t_shell *second;
			t_shell *third;
			t_shell *active_shell;
}				terminal_t;

void init_shell();

//TODO: To remove later
void check_term_struct();

#endif
