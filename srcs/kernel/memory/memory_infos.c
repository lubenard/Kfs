/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_infos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luca </var/spool/mail/luca>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 00:51:32 by luca              #+#    #+#             */
/*   Updated: 2022/08/18 01:23:40 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory.h"

t_memory_infos memory_infos;

void init_memory_infos() {
	memory_infos.size = 0;
	memory_infos.used_size = 0;
}

void update_memory_infos(unsigned long size, unsigned long used_size) {
	memory_infos.size = size;
	memory_infos.used_size = used_size;
}

t_memory_infos *get_memory_infos() {
	return &memory_infos;
}
