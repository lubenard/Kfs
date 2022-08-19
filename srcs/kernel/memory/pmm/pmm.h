/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 17:29:34 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/18 01:47:14 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMM_H
# define PMM_H

# define PMM_BLOCK_FREE '.'
# define PMM_BLOCK_OCCUPIED 'x'
# define PMM_BLOCK_SHARED '#'

void *create_pmm_array(void *start_addr, unsigned int page_number);
void set_block_status(unsigned int index, char new_block_status);
char get_block_status(unsigned int index);
void *pmm_next_fit(unsigned int size, int flags);
void pmm_unset_pages(void *ptr, unsigned int size);

typedef struct s_pmm {
	unsigned int	pmm_page_number;
	unsigned int	available_pages_number;
	unsigned int	pmm_last_index;
	void			*pmm_memory_start;
}				t_pmm;

#endif
