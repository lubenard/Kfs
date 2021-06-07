/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:39:44 by lubenard          #+#    #+#             */
/*   Updated: 2021/06/04 17:03:45 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

# include <stdint.h>

typedef struct mem_tracker {
	struct mem_tracker *prev;
	unsigned int start;
	unsigned int length;
	struct mem_tracker *next;
}			   mem_tracker_t;

void init_memory();
extern void enable_paging(unsigned int *);
extern void flush_tlb();
extern void get_memory_size();
#endif
