/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 15:47:13 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 15:54:41 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../drivers/vga/vga.h"

void help(char **command) {
	(void)command;
	terminal_writestr("Commands available:\n");
	terminal_writestr("\thelp - print this help\n");
	terminal_writestr("\tkbd language - set kbd layout: QWERTY or AZERTY\n");
	terminal_writestr("\tprint_stack - Print the current stack\n");
	terminal_writestr("\tprint_hist - Print the shell history\n");
	terminal_writestr("\techo - echo the content\n");
	terminal_writestr("\tclear - clear the screen\n");
	terminal_writestr("\tshutdown - shutdown the computer\n");
	terminal_writestr("\treboot - reboot the computer\n");
}
