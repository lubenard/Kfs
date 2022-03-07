/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shutdown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 14:48:47 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 16:33:52 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../io.h"
#include "builtins.h"

/*
 * Send a shutdown signal
 * It should not work on real hardware /!\
 * It it only working for QEMU
 */
void	shutdown(t_command *command) {
	(void)command;
	outw(0x604, 0x2000);
}

/*
 * Send a keyboard reboot signal
 * This should work on real hardware
 */
void	reboot(t_command *command) {
	(void)command;
	// 8242 reset
	uint8_t good = 0x02;
	while (good & 0x02)
		good = inb(0x64);
	outb(0x64, 0xFE);
}
