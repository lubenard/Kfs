/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 00:40:09 by lubenard          #+#    #+#             */
/*   Updated: 2022/09/16 16:42:33 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PROCESSES_H
# define PROCESSES_H

# define MAX_PID 32768

#include "../isr/isr.h"

# define SIGHUP 1
# define SIGINT 2
# define SIGQUIT 3
# define SIGFPE 8
# define SIGKILL 9
# define SIGALRM 14
# define SIGTERM 15

# define STATUS_RUN 1
# define STATUS_WAITING 2
# define STATUS_ZOMBIE 3
# define STATUS_THREAD 4
# define STATUS_STOPED 5

typedef struct s_signal_list {
	unsigned short signal;
	struct s_signal_list *next;
}				t_signal_list;

typedef struct s_page_directory {
	uint32_t page_directory[1024];
}				t_page_directory;

typedef struct s_process_registers {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eflags;
    uint32_t cr3;
    uint32_t eip;
}               t_process_registers;

typedef struct s_process {
	char name[20];
	unsigned long pid;
	short status;
	// There can be only one parent
	struct s_process *parent;
	// But multiple childs ?
	struct s_process *childs;
	// Stack and Heap
	void *stack_start;
	void *stack_ptr;
	unsigned long stack_size;

	void *heap_ptr;
	unsigned long heap_size;

	t_signal_list *signals;
	unsigned int ownerId;

	t_process_registers regs;

	t_page_directory *page_directory;

    int priority;

	// Used for childs process ?
	struct s_process *next;
}			t_process;


typedef struct s_scheduler_queue {
    t_process *process;
	// We use a next to use it as linked list
	// TODO: Optimize ?
    struct s_scheduler_queue *next;
}               t_scheduler_queue;

typedef struct s_processes {
    t_scheduler_queue *processes_list;
    t_scheduler_queue *current_process;
    unsigned long size;
    t_scheduler_queue *first;
    t_scheduler_queue *last;
}				t_processes;


void		        init_processes();
unsigned long		create_process(char *name, t_process *parent, unsigned int ownerId, void *functionStart);
void		        register_kernel_as_process();
t_process	        *find_process_by_pid(unsigned long pid);
void		        add_signal_to_pid(unsigned long pid, unsigned short signal);
void                add_process_to_queue(t_process *process);

extern void switch_regs(t_process_registers *regs);

#endif
