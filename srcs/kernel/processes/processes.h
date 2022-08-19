/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca </var/spool/mail/luca>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/20 00:40:09 by luca              #+#    #+#             */
/*   Updated: 2022/08/20 01:28:53 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define SIGHUP 1
# define SIGINT 2
# define SIGQUIT 3
# define SIGFPE 8
# define SIGKILL 9
# define SIGALRM 14
# define SIGTERM 15

typedef struct s_process {
	unsigned long pid;
	short status;
	// There can be only one parent
	struct s_process *parent;
	// But multiple childs
	struct s_process *childs;
	// Stack and Heap
	// Signals
	unsigned int ownerId;
}			t_process;
