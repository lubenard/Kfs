/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 20:31:46 by lubenard          #+#    #+#             */
/*   Updated: 2022/07/19 14:41:03 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../isr.h"
#include "../../../io/io.h"
#include "../../../lib/iolib.h"

uint32_t tick = 0;

uint32_t get_tick() {
	return tick;
}

void timer_callback(registers_t regs) {
	(void)regs;
	tick++;
	//printk(KERN_INFO, "Tick is %d", tick);
}

void init_pit() {
	register_interrupt_handler(0, &timer_callback);

	int frequency = 50;
	// The value we send to the PIT is the value to divide it's input clock
	// (1193180 Hz) by, to get our required frequency. Important to note is
	// that the divisor must be small enough to fit into 16-bits.
	uint32_t divisor = 1193180 / frequency;

	// Send the command byte.
	outb(0x43, 0x36);

	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor>>8) & 0xFF);

	// Send the frequency divisor.
	outb(0x40, l);
	outb(0x40, h);
	printk(KERN_INFO, "PIT has been initialised");
}
