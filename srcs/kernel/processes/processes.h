/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 00:40:09 by lubenard          #+#    #+#             */
/*   Updated: 2022/09/15 16:09:10 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PROCESSES_H
# define PROCESSES_H

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

typedef struct s_process {
	char name[20];
	unsigned long pid;
	short status;
	// There can be only one parent
	struct s_process *parent;
	// But multiple childs ?
	struct s_process *childs;
	// Stack and Heap
	void *stack_ptr;
	unsigned long stack_size;

	void *heap_ptr;
	unsigned long heap_size;

	t_signal_list *signals;
	unsigned int ownerId;

	// We use a next to use it as linked list
	// TODO: Optimize ?
	struct s_process *next;
}			t_process;

void		register_kernel_as_process();
t_process	*find_process_by_pid(unsigned long pid);
void		add_signal_to_pid(unsigned long pid, unsigned short signal);

#endif
