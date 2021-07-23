/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 22:17:41 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/22 23:33:35 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include "../../../lib/iolib.h"
#include "../../../lib/memlib.h"
#include "../../memory/memory.h"

extern uint32_t endKernel;

mem_page_tracking_t *create_new_node_memory(uint32_t addr_low, uint32_t size, uint32_t pid, short should_early_malloc) {
	mem_page_tracking_t *node;
	(void)addr_low;
	(void)should_early_malloc;
	// We can create a header element right now.
	if (!(node = (mem_page_tracking_t *)e_kmalloc(sizeof(mem_page_tracking_t), 0, 0)))
		return 0;
	printk(KERN_INFO, "Node is created at addr %d", node);
	//printk(KERN_INFO, "TEST KMALLOC %d", e_kmalloc(10, 0, 0));
	node->prev = 0;
	node->addr_low = (uint32_t) &node + sizeof(mem_page_tracking_t);
	printk(KERN_INFO, "Node mem addr %d long and starting at %x.", size, node->addr_low);
	set_placement_addr(size);
	printk(KERN_INFO, "Increase new placement_addr to %d", size);
	node->len_low = size;
	node->is_allocated = 1;
	node->owner_pid = pid;
	node->next = 0;
	node->magic_number = MAGIC_NUMBER_MEMORY;
	mem_page_tracking_tail_t *tail;
	if (!(tail = (mem_page_tracking_tail_t *)e_kmalloc(sizeof(mem_page_tracking_tail_t), 0, 0)))
		return 0;
	node->tail = tail;
	printk(KERN_INFO, "Tail node is created at addr %d", tail);
	tail->magic_number = MAGIC_NUMBER_MEMORY;
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
		printk(KERN_NORMAL, "HEAD EST CREE\n");
		mem_page_tracking_t *head = create_new_node_memory((uint32_t) &endKernel, size, 0, 1);
		//printk(KERN_INFO, "First node is size %d containing addr %x.", head->len_low, head->addr_low);
		return head;
	} else {
		while (lknd_lst) {
			if (lknd_lst->next) {
				if (lknd_lst->addr_low + size < lknd_lst->next->addr_low) {
					// We can allocate here
					printk(KERN_NORMAL, "NODE EST CREE\n");
					mem_page_tracking_t *node = create_new_node_memory(lknd_lst->addr_low + lknd_lst->len_low, size, 0, 0);
					printk(KERN_INFO, "New node is size %d containing addr %x", node->len_low, node->addr_low);
					return node;
				}
			} else {
				// Mean we are at the end of memory
				printk(KERN_NORMAL, "NODE EST CREE\n");
				mem_page_tracking_t *node = create_new_node_memory(lknd_lst->addr_low + lknd_lst->len_low, size, 0, 0);
				printk(KERN_INFO, "New node 2 is size %d containing addr %x", node->len_low, node->addr_low);
				return node;
			}
			lknd_lst = lknd_lst->next;
		}
		return 0;
	}
}
