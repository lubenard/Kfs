/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uptime.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 13:55:40 by lubenard          #+#    #+#             */
/*   Updated: 2022/07/19 14:41:58 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../lib/iolib.h"
#include "builtins.h"
#include "../../../kernel/isr/irqs/irqs.h"

void uptime(t_command *command) {
	(void)command;
	printk(KERN_NORMAL, "Uptime is %d ticks\n", get_tick());
}
