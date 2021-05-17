/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:26:33 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 15:31:22 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "isr.h"
#include "../../lib/iolib.h"
#include "../../io/io.h"

static const char *interrupt_message[] = {
	"Division by 0", "Debug exception", "Non maskable interrupt",
	"Breakpoint exception", "Into detected overflow",
	"Out of bounds exception", "Invalid opcode", "No coprocessor exception",
	"Double fault", "Coprocessor segment overrun", "Bad TSS",
	"Segment not present", "Stack fault", "General protection fault",
	"Page fault", "Unknown interrupt exception", "Coprocessor fault",
	"Alignement check exception", "Machine check exception"
};

/*
 * This gets called from our ASM interrupt handler.
 */
void isr_handler(registers_t regs)
{
	if (regs.int_no < 19)
		printk(KERN_ERROR, "received interrupt: %s",
		interrupt_message[regs.int_no]);
	else
		printk(KERN_ERROR, "received interrupt: %d", regs.int_no);
	while (1){}
}

void *irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

/*
 * Register a IRQ with associated function
 */
void register_interrupt_handler(int8_t n, isr_t handler) {
	irq_routines[n] = handler;
}

/*
 * This gets called from our ASM interrupt handler
 */
void irq_handler(registers_t regs) {

	void (*handler)(registers_t r);

	if (irq_routines[regs.int_no - 32] != 0) {
		handler = irq_routines[regs.int_no - 32];
		handler(regs);
	}
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no - 32 >= 8) {
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	// Send reset signal to master.
	outb(0x20, 0x20);
}
