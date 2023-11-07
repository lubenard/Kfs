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
	t_process *process;

	if (!(process = malloc(sizeof(t_process)))) {
		return ;
	}
	strcpy(process->name, "kernel");
	process->pid = ++last_pid;
	process->stack_ptr = 0;
	process->stack_size = 0;
	process->status = STATUS_RUN;
	process->ownerId = 0;
	process->signals = NULL;
	printd(KERN_INFO, "Kernel Process created at %p, pid %d", process, process->pid);
}

/*
 * Get the correct process structure for given pid
 * This algo is very bad, but functionnal for now
 * TODO: adapt to scheduler stuctures
 */
t_process *find_process_by_pid(unsigned long pid) {
    t_kernel *kernel_struct = get_kernel_struct();
	t_process *process = kernel_struct->processes->processes_list->process;
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

void show_process_registers(t_process *process) {
    printd(KERN_INFO, "---- Displaying registers for: %s (%d) -----", process->name, process->pid);
    printd(KERN_INFO, "- eax: %p", process->regs.eax);
    printd(KERN_INFO, "- ecx: %p", process->regs.ecx);
    printd(KERN_INFO, "- edx: %p", process->regs.edx);
    printd(KERN_INFO, "- ebx: %p", process->regs.ebx);
    printd(KERN_INFO, "- esp: %p", process->regs.esp);
    printd(KERN_INFO, "- ebp: %p", process->regs.ebp);
    printd(KERN_INFO, "- esi: %p", process->regs.esi);
    printd(KERN_INFO, "- edi: %p", process->regs.edi);
    printd(KERN_INFO, "- eflags: %p", process->regs.eflags);
    printd(KERN_INFO, "- cr3: %p", process->regs.cr3);
    printd(KERN_INFO, "- eip: %p", process->regs.eip);
    
}

/*
 * Create a process
 * Return the pid of created process
 */
unsigned long create_process(char *name, t_process *parent, unsigned int ownerId, void *functionStart) {
	t_process	*process;
	void		*start_memory;

    (void)*parent;
    if (last_pid + 1 > MAX_PID) {
        printk(KERN_ERROR, "Cannot create more processes ! Process limit have been reached !");
        return 0;
    }

    printd(KERN_INFO, "Trying to create process...");
	if (!(process = malloc(sizeof(t_process)))) {
        printk(KERN_WARNING, "Error while allocating memory for t_process struct");
		return -1;
	}

    if ((process->page_directory = mmap(4096, USERPAGE)) == 0) {
        printk(KERN_WARNING, "Error while allocating memory for t_page_directory struct");
        return -1;
    }
    if ((start_memory = mmap(PAGESIZE, USERPAGE)) == 0) {
        printk(KERN_WARNING, "Error while allocating memory for process with mmap");
		return -1;
	}
	strlcpy(process->name, name, 20);
	process->pid = ++last_pid;
	process->stack_start = start_memory;
	process->stack_ptr = start_memory;
	process->stack_size = PAGESIZE;
	process->status = STATUS_RUN;
	process->ownerId = ownerId;
    process->priority = 0;
    printk(KERN_INFO, "Process created at %p, id %d, memory page from %p to %p", process, process->pid, start_memory, (char*)start_memory + PAGESIZE);
    copy_kernel_to_process_page_directory(get_kernel_struct()->kernel_page_directory, process->page_directory);

    // Used for testing in struct !!
    //process->regs.ecx = 0xdeadbeef;
    process->regs.esp = (uint32_t)start_memory;
    process->regs.ebp = (uint32_t)start_memory;
    process->regs.eip = (uint32_t)functionStart;
    process->regs.eflags = 0x206; // enable interrupt
    process->regs.cr3 = process->page_directory->page_directory[0];
    show_process_registers(process);
    add_process_to_queue(process);
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

	t_kernel *kernel_struct = get_kernel_struct();
	t_processes *processes_infos;

	if (!(processes_infos = malloc(sizeof(t_processes)))) {
		printk(KERN_WARNING, "Could not allocate memory for new processes infos stucture !");
		return ;
	}
	kernel_struct->processes = processes_infos;
    printd(KERN_INFO, "Allocated struct process %p", kernel_struct->processes);
}
