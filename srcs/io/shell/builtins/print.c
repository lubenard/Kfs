/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 14:10:58 by lubenard          #+#    #+#             */
/*   Updated: 2022/04/11 12:24:17 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../../lib/memlib.h"
#include "../../../lib/strlib.h"
#include "../../../lib/iolib.h"
#include "../../../drivers/vga/vga.h"
#include "builtins.h"
#include "../shell.h"

void int_into_str_hex(unsigned int addr, char *result, int size) {
	int		len;
	char	base_str[16] = "0123456789abcdef";

	len = size - 1;
	memset(result, '0', size);
	result[size - 1] = 0;
	while (addr != 0) {
		result[--len] = base_str[addr % 16];
		addr = addr / 16;
	}
}

void shell_print_stack(uint32_t esp, uint32_t ebp) {
	int i;
	char *addr;
	char hex[3];
	while (esp <= ebp) {
		i = 0;
		printk(KERN_NORMAL, "0x%x ", esp);
		addr = (char*)esp;
		while (i != 10) {
			int_into_str_hex(addr[i++], hex, 3);
			printk(KERN_NORMAL, "%s ", hex);
		}
		i = 0;
		while (i != 10) {
			if (isprint(addr[i]))
				printk(KERN_NORMAL, "%c", addr[i]);
			else
				terminal_writec('.');
			i++;
		}
		esp += 10;
		printk(KERN_NORMAL, "\n");
	}
}

void shell_print_history(t_shell *shell) {
	int i = 0;
	int j = 1;
	while (i != 4) {
		if (strlen(shell->cmd_line[i]) > 0)
			printk(KERN_NORMAL, "[%d] %s\n", j++, shell->cmd_line[i]);
		i++;
	}
}

void shell_print(t_command *command) {
	if (strcmp(command->command[1], "stack") == 0)
		shell_print_stack(command->esp, command->ebp);
	else if (strcmp(command->command[1], "history") == 0)
		shell_print_history((t_shell *)command->current_shell);
}
