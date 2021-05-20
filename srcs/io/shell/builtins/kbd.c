/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kbd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 15:11:23 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/19 15:15:12 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/strlib.h"
#include "../../../lib/iolib.h"
#include "../../../drivers/PS2_keyboard/PS2_keyboard.h"

/*
 * Command in charge for changing kbd map
 */
void	change_kbd_map(char *cmd_line) {
	unsigned short new_language;
	if (!(new_language = atoi(&cmd_line[13])))
		printk(KERN_ERROR, "Bad input: please enter valid numbers");
	else {
		if (new_language > 0 && new_language < 3)
			printk(KERN_INFO, "Keyboard layout is now %s", set_language(new_language));
		else
			printk(KERN_INFO, "Invalid option: 1 for QWERTY map, 2 for AZERTY map");
	}
}

