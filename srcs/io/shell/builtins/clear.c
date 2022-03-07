/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 17:07:41 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/07 17:09:09 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../drivers/vga/vga.h"
#include "builtins.h"

void clear(t_command *command) {
	(void)command;
	terminal_clear();
}
