/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 22:17:41 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/13 13:48:50 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include "../../../lib/iolib.h"
#include "../../../lib/memlib.h"

extern uint32_t endKernel;

mem_page_tracking_t *create_new_node_memory(uint32_t addr_low, uint32_t size, uint32_t pid) {
	mem_page_tracking_t *node;
	// We can create a header element right now.
	if (!(node = (mem_page_tracking_t*)e_kmalloc(sizeof(mem_page_tracking_t), 0, 0)))
		return 0;
	(void)addr_low;
	(void)size;
	(void)pid;
	printk(KERN_INFO, "node is at addr %x", &node);
	//node->addr_low = addr_low;
	//node->addr_high = entry->addr_high;
	//node->len_low = size;
	printk(KERN_INFO, "Success alloc");
	//node->len_high = entry->len_high;
	//node->is_allocated = 1;
	//node->owner_pid = pid;
	printk(KERN_INFO, "Success return");
	return node;
}

void delete_node_memory(mem_page_tracking_t *node) {
	// Just "connect" the prev and next node together
	if (node->prev && node->next)
		node->prev->next = node->next;
	memset(node, 0, sizeof(mem_page_tracking_t));
}

mem_page_tracking_t *first_fit_memory(mem_page_tracking_t *lknd_lst, unsigned int size) {
	// If there is no element in the linked list, all memory is free.
	if (!lknd_lst) {
		printk(KERN_INFO, "Creating header linking list node");
		mem_page_tracking_t *head = create_new_node_memory((uint32_t) 0, size, 0);
		printk(KERN_INFO, "First node is size %d containing addr %x", head->len_low, head->addr_low);
		return head;
	} else {
		while (lknd_lst) {
			if (lknd_lst->addr_low + size < lknd_lst->next->addr_low) {
				// We can allocate here
				mem_page_tracking_t *node = create_new_node_memory(lknd_lst->addr_low + lknd_lst->len_low, size, 0);
				printk(KERN_INFO, "Node is size %d", node->len_low);
				return node;
			}
			lknd_lst = lknd_lst->next;
		}
		return 0;
	}
}
