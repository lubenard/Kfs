/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 16:06:41 by lubenard          #+#    #+#             */
/*   Updated: 2022/09/13 18:16:55 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "processes.h"

void add_signal_to_pid(unsigned long pid, unsigned short signal) {
	(void)pid;
	(void)signal;
	//t_process *process = find_process_by_pid(pid);
	//add_signal_to_process(process, signal);
}
