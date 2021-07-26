/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 12:12:52 by lubenard          #+#    #+#             */
/*   Updated: 2021/07/23 16:34:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "memory.h"

/*uint32_t *frames;
uint32_t nframes;

void setFrames(uint32_t *new_frames, uint32_t new_nframes) {
	frames = new_frames;
	nframes = new_nframes;
	memset(frames, 0, INDEX_FROM_BIT(nframes));
}

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000;
	frames[INDEX_FROM_BIT(frame)] |= (0x1 << OFFSET_FROM_BIT(frame));
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
	uint32_t frame = frame_addr / 0x1000;
	frames[INDEX_FROM_BIT(frame)] &= ~(0x1 << OFFSET_FROM_BIT(frame));
}

// Static function to find the first free frame.
static uint32_t first_frame() {
	uint32_t i;
	uint32_t j;
	for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
		if (frames[i] != 0xFFFFFFFF) {
			for (j = 0; j < 32; j++) {
				uint32_t toTest = 0x1 << j;
				if (!(frames[i] & toTest))
					return i * 4 * 8 + j;
			}
		}
	}
	return 0;
}

// Allocate a frame
void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
	if (page->frame != 0)
		return; // Frame was already allocated, return straight away.
	else {
		// idx is now the index of the first free frame.
		uint32_t idx = first_frame();
		if (idx == (uint32_t) - 1) {
			printk(KERN_ERROR, "No more free frame !");
			//PANIC("No free frames!");
		}
		set_frame(idx * 0x1000);
		page->present = 1;
		page->rw = (is_writeable) ? 1 : 0;
		page->user = (is_kernel) ? 0 : 1;
		page->frame = idx;
	}
}

// Deallocate a frame.
void free_frame(page_t *page) {
	uint32_t frame;
	if (!(frame = page->frame)) {
		return;
	} else {
		clear_frame(frame);
		page->frame = 0x0;
	}
}*/
