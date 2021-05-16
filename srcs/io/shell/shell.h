/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 00:12:16 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:12:44 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

typedef struct shell {
	// Screen buffer when backup is needed
	uint16_t buffer[2000];
	unsigned short start_cmd_line;
	// Command line buffer
	char cmd_line[256];
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
