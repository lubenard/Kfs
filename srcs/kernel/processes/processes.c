/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 01:03:43 by lubenard          #+#    #+#             */
/*   Updated: 2023/01/01 21:04:48 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "processes.h"
#include "../kernel.h"
#include "../../lib/strlib.h"
#include "../../lib/iolib.h"
#include "../memory/vmm/malloc/malloc.h"
#include "../memory/vmm/vmm.h"

unsigned int last_pid = 0;

void register_kernel_as_process() {
	t_kernel *kernel_struct = get_kernel_struct();
	t_process *process;

	if (!(process = malloc(sizeof(t_process)))) {
		return ;
	}
	strcpy(process->name, "Kernel");
	process->pid = ++last_pid;
	process->stack_ptr = 0;
	process->stack_size = 0;
	process->status = STATUS_RUN;
	process->ownerId = 0;
	process->signals = NULL;
	printk(KERN_INFO, "Process created at %p", process);
	kernel_struct->processes->processes_list = process;
}

/*
 * Get the correct process structure for given pid
 * This algo is very bad, but functionnal for now
 */
t_process *find_process_by_pid(unsigned long pid) {
	t_kernel *kernel_struct = get_kernel_struct();
	t_process *process = kernel_struct->processes->processes_list;
	t_process *childs = NULL;

	while (process) {
		if (process->pid == pid) {
			return process;
		}
		childs = process->childs;
		if (childs != NULL) {
			while (childs) {
				if (childs->pid == pid) {
                    printd(KERN_WARNING, "Return pid with name %s for given pid %d", childs->name, pid);
                    return childs;
				}
				childs = childs->next;
			}
		}
		process = process->next;
	}
    printd(KERN_WARNING, "No process found for given pid %d", pid);
	return NULL;
}

/*
 * Create a process
 * Return the pid of created process
 */
unsigned long create_process(char *name, t_process *parent, unsigned int ownerId) {
	t_process	*process;
	void		*start_memory;

	(void)*parent;
    printd(KERN_INFO, "Trying to create process...");
	if (!(process = malloc(sizeof(t_process)))) {
        printk(KERN_WARNING, "Error while allocating memory for t_process struct");
		return -1;
	}
	if ((start_memory = mmap(PAGESIZE, 0)) == 0) {
        printk(KERN_WARNING, "Error while allocating memory for process with mmap");
		return -1;
	}
	strlcpy(process->name, name, 20);
	process->pid = ++last_pid;
	process->stack_ptr = start_memory;
	process->stack_size = PAGESIZE;
	process->status = STATUS_RUN;
	process->ownerId = ownerId;
	return process->pid;
}

void destroy_process(unsigned int pid) {
	t_process *killable_process;

	killable_process = find_process_by_pid(pid);
	// TODO: Handle memory allocated for process work
	munmap(killable_process->stack_ptr, PAGESIZE);
	free(killable_process);
}

void init_processes() {

    uint32_t *ptr = (uint32_t*)0xA0000000;
    uint32_t do_page_fault = *ptr;
    (void)do_page_fault;

	t_kernel *kernel_struct = get_kernel_struct();
	t_processes *processes_infos;

	if (!(processes_infos = malloc(sizeof(t_processes)))) {
		printk(KERN_WARNING, "Could not allocate memory for new processes infos stucture !");
		return ;
	}
	kernel_struct->processes = processes_infos;
    printd(KERN_INFO, "Allocated struct process %p", kernel_struct->processes);
}
