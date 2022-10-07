/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 16:06:41 by lubenard          #+#    #+#             */
/*   Updated: 2022/09/16 15:59:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../memory/vmm/malloc/malloc.h"
#include "../../lib/iolib.h"
#include "processes.h"

/*
 * Add signal to given process.
 * The current signal implementation is linked list.
 * TODO: Maybe find better implementation (faster)
 */
void add_signal_to_process(t_process *process, unsigned short signal) {
	t_signal_list *signal_struct;
	t_signal_list *signal_list;

	if (!(signal_struct = malloc(sizeof(t_signal_list)))) {
		printk(KERN_WARNING, "Could not allocate space for signal_struct for PID: %d, Signal: %d", process->pid, signal);
		return ;
	}
	signal_struct->signal = signal;
	signal_list = process->signals;
	if (signal_list == NULL) {
		printd(KERN_INFO, "Added signal as 1rst element of list");
		process->signals = signal_struct;
	} else {
		// Go to end of linked list to add at end
		printd(KERN_INFO, "Added signal as last element of list");
		while (signal_list->next) {
			signal_list = signal_list->next;
		}
		signal_list->next = signal_struct;
	}
}

/*
 * Fork a process and return
 * < 0 if creation is unsuccessfull
 * 0 if created process is returned 
 * > 0 if parent process is returned
 */
int fork() {
	return 0;
}


void add_signal_to_pid(unsigned long pid, unsigned short signal) {
	t_process *process = find_process_by_pid(pid);
	if (process != NULL) {
		printd(KERN_INFO, "Found process struct at addr %p", process);
		add_signal_to_process(process, signal);
	} else {
		printd(KERN_WARNING, "Process not found for PID: %d", pid);
	}
}

void deliver_next_signal_to_process(t_process *process) {
	t_signal_list *current_signal;

	if (process->signals != NULL) {
		process->status = process->signals->signal;
		current_signal = process->signals;
		process->signals = process->signals->next;
		free(current_signal);
	}
}
