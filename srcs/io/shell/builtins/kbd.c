/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kbd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 15:11:23 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 17:02:48 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/strlib.h"
#include "../../../lib/iolib.h"
#include "../../../drivers/PS2_keyboard/PS2_keyboard.h"
#include "builtins.h"

/*
 * Command in charge for changing kbd map
 */
void kbd(t_command *command) {
	int i = 0;
	char error = 0;
	while (g_kbd_list[i]) {
		if (strcmp(command->command[1], g_kbd_list[i]) == 0)
			error = i;
		i++;
	}
	if (error == 0) {
		printk(KERN_ERROR, "Invalid option: can only be QWERTY or AZERTY");
		return;
	}
	printk(KERN_INFO, "Keyboard layout is now %s", set_language(error));
}
