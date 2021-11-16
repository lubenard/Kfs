/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 17:29:34 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/16 18:14:53 by lubenard         ###   ########.fr       */
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

#endif
