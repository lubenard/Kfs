/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   top.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 15:11:23 by lubenard          #+#    #+#             */
/*   Updated: 2022/09/15 16:11:36 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "../../../kernel/kernel.h"

/*
 * See all running processes
 */
void top(t_command *command) {
	(void)command;
	t_kernel *kernel_struct = get_kernel_struct();
	t_process *tmp = kernel_struct->processes_list;

	while (tmp) {
		printk(KERN_NORMAL, "Name: %s PID: %d Owner: %d, Status: %d, Signals: %d\n", tmp->name, tmp->pid, tmp->ownerId, tmp->status, tmp->signals->signal);
		if (tmp->childs) {
			printk(KERN_NORMAL, "\tChild found: %s PID: %d Owner: %d\n", tmp->name, tmp->pid, tmp->ownerId);
		}
		tmp = tmp->next;
	}
}
