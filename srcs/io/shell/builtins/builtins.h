/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 14:19:15 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 17:40:15 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdint.h>

typedef struct s_command {
	uint32_t	esp;
	uint32_t	ebp;
	char		**command;
	// Should be casted to t_shell
	void		*current_shell;
}	t_command;

void help(t_command *command);
void clear(t_command *command);
void echo(t_command *command);
void print(t_command *command);
void kbd(t_command *command);
void shutdown(t_command *command);
void reboot(t_command *command);

// Last element is 0 to avoid overflows
static const char *builtins_names[] = {"help", "kbd", "clear", "shutdown", "reboot",
							"print", "echo", 0};

static void (*g_builtins_array[])(t_command *command) = {help, kbd, clear, shutdown, reboot,
							print, echo};

#endif
