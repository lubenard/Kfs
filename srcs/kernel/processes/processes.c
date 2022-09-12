/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 01:03:43 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/20 19:09:47 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "processes.h"
#include "../memory/vmm/malloc/malloc.h"
#include "../memory/vmm/vmm.h"

// Last pid set to -1 so kernel can be registered as process 0 ?
unsigned int last_pid = -1;

/*
 * Create a process
 * Return the pid of created process
 */
long create_process(t_process *parent, unsigned int ownerId) {
	t_process	*process;
	void		*start_memory;

	(void)*parent;
	if (!(process = malloc(sizeof(t_process)))) {
		return -1;
	}
	if ((start_memory = mmap(PAGESIZE, 0)) == 0) {
		return -1;
	}
	process->pid = ++last_pid;
	process->stack_ptr = start_memory;
	process->stack_size = PAGESIZE;
	process->ownerId = ownerId;
	return process->pid;
}
