/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca </var/spool/mail/luca>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 00:40:09 by luca              #+#    #+#             */
/*   Updated: 2022/08/20 18:09:10 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define SIGHUP 1
# define SIGINT 2
# define SIGQUIT 3
# define SIGFPE 8
# define SIGKILL 9
# define SIGALRM 14
# define SIGTERM 15

# define STATUS_RUN 1
# define STATUS_ZOMBIE 2
# define STATUS_THREAD 3

typedef struct s_process {
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

	// Signals
	unsigned int ownerId;
}			t_process;
