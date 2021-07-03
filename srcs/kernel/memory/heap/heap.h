/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 22:08:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/03 22:09:19 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# define MAGIC_NUMBER_MEMORY                     0x1234AB

typedef struct mem_page_tracking {
	struct mem_page_tracking *prev;
	uint32_t addr_low;
	uint32_t addr_high;
	uint32_t len_low;
	uint32_t len_high;
	uint32_t is_allocated;
	uint32_t owner_pid;
	struct mem_page_tracking *next;
}				mem_page_tracking_t;


#endif
