/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 17:23:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/11/16 18:15:46 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../lib/iolib.h"

char *pmm_array = 0;

// Free block '.', occupied block 'x', shared block '#'

void *create_pmm_array(void *start_addr, unsigned int page_number) {
	pmm_array = start_addr;
	printk(KERN_INFO, "Initialised pmm at %p with size %d", pmm_array, page_number);
	return (char *)pmm_array + page_number;
}

void set_block_status(unsigned int index, char new_block_status) {
	printk(KERN_INFO, "Set block located at index %d new status %c (addr %p)", index, new_block_status, &pmm_array[index]);
	pmm_array[index] = new_block_status;
}

char get_block_status(unsigned int index) {
	return pmm_array[index];
}
