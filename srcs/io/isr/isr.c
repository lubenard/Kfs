/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:26:33 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/04 20:52:30 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "isr.h"
#include "../../lib/lib.h"
#include "../io.h"

static const char *interrupt_message[] = {
	"Division by 0", "Debug exception", "Non maskable interrupt",
	"Breakpoint exception", "Into detected overflow",
	"Out of bounds exception", "Invalid opcode", "No coprocessor exception",
	"Double fault", "Coprocessor segment overrun", "Bad TSS",
	"Segment not present", "Stack fault", "General protection fault",
	"Page fault", "Unknown interrupt exception", "Coprocessor fault",
	"Alignement check exception", "Machine check exception"
};

// This gets called from our ASM interrupt handler.
void isr_handler(registers_t regs)
{
	if (regs.int_no < 19)
		printk(KERN_ERROR, "received interrupt: %s",
		interrupt_message[regs.int_no]);
	else
		printk(KERN_ERROR, "received interrupt: %d", regs.int_no);
}

void *irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0
};

void register_interrupt_handler(int8_t n, void (*handler)(struct registers *r))
{
	//printk(KERN_INFO, "Set entry at %d", n);
	irq_routines[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t *regs)
{
	//printk(KERN_INFO, "This function is launched, received %d from regs", regs->int_no);

	//printk(KERN_INFO, "Searching for entry @ int_no %d", regs->int_no - 32);
	if (irq_routines[regs->int_no] != 0) {
		//printk(KERN_INFO, "Found entry @ int_no %d -> %p", regs->int_no, irq_routines[regs->int_no]);
		void (*handler)(struct registers *r) = irq_routines[regs->int_no];
		handler(regs);
	} //else
		//printk(KERN_ERROR, "No entry here @ int_no %d", regs->int_no);

	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	//if (regs->int_no >= 40) {
		// Send reset signal to slave.
		//outb(0x20, 0x20);
		//outb(0xA0, 0x20);
	//}
	// Send reset signal to master. (As well as slave, if necessary).*/
	outb(0x20, 0x20);
}
