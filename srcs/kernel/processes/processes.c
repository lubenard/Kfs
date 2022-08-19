/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca </var/spool/mail/luca>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 01:03:43 by luca              #+#    #+#             */
/*   Updated: 2022/08/20 01:30:52 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "processes.h"
#include "../../kernel/memory/vmm/malloc/malloc.h"

// Last pid set to -1 so kernel can be registered as process 0 ?
unsigned int last_pid = -1;

/*
 * Create a process
 * Return the pid of created process
 */
long create_process(t_process *parent, unsigned int ownerId) {
	t_process *process;

	(void)*parent;
	if (!(process = malloc(sizeof(t_process)))) {
		return -1;
	}
	process->pid = ++last_pid;
	process->ownerId = ownerId;
	return process->pid;
}
