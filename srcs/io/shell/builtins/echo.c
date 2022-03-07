/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:27:09 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 16:28:07 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/iolib.h"
#include "builtins.h"

void echo(t_command *command) {
	int i = 1;

	while (command->command[i]) {
		printk(KERN_NORMAL, "%s ", command->command[i++]);
	}
	printk(KERN_NORMAL, "\n");
}
