/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 17:13:10 by lubenard          #+#    #+#             */
/*   Updated: 2022/09/13 18:13:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "../../../drivers/vga/vga.h"
#include "../../../kernel/processes/processes.h"

int function_test_1() {
	int i = 0;

	while (i != 10) {
		terminal_writestr("Hello from function test 1");
		//sleep(2);
		i++;
	}
	return 0;
}

int function_test_2() {
	int i = 0;

	while (i != 10) {
		terminal_writestr("Hello from function test 2");
		//sleep(2);
		i++;
	}
	return 0;
}

void test_process(t_command *command) {
	(void)command;
	terminal_writestr("Info: Test process ");
	terminal_set_fg_color(VGA_COLOR_GREEN);
	terminal_writestr("Launched");
	terminal_set_fg_color(VGA_COLOR_LIGHT_GREY);
	terminal_writec('\n');
}
