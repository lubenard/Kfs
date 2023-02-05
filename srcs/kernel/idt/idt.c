/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 21:45:15 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/19 20:21:39 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "idt.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"
#include "../../io/io.h"
#include "../isr/irqs/irqs.h"
#include "../../io/shell/builtins/builtins.h"


struct idt_ptr_struct idt_ptr;
struct idt32 idt_entries[256];

static void idt_set_gate(int8_t num, int32_t base, int16_t sel, int8_t flags)
{
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_mid = (base >> 16) & 0xFFFF;

	idt_entries[num].selector = sel;
	// Must always be 0
	idt_entries[num].zero = 0;
	// We must uncomment the OR below when we get to using user-mode.
	// It sets the interrupt gate's privilege level to 3.
	idt_entries[num].flags = flags /* | 0x60 */;
}

void init_idt()
{
	idt_ptr.limit = sizeof(struct idt32) * 256 - 1;
	idt_ptr.base  = (int32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(struct idt32)*256);

	idt_flush((uint32_t)&idt_ptr);

	// Fill 32 first entrys
	idt_set_gate(0, (int32_t)isr0, 0x08, 0x8E);
	idt_set_gate(1, (int32_t)isr1, 0x08, 0x8E);
	idt_set_gate(2, (int32_t)isr2, 0x08, 0x8E);
	idt_set_gate(3, (int32_t)isr3, 0x08, 0x8E);
	idt_set_gate(4, (int32_t)isr4, 0x08, 0x8E);
	idt_set_gate(5, (int32_t)isr5, 0x08, 0x8E);
	idt_set_gate(6, (int32_t)isr6, 0x08, 0x8E);
	idt_set_gate(7, (int32_t)isr7, 0x08, 0x8E);
	idt_set_gate(8, (int32_t)isr8, 0x08, 0x8E);
	idt_set_gate(9, (int32_t)isr9, 0x08, 0x8E);
	idt_set_gate(10, (int32_t)isr10, 0x08, 0x8E);
	idt_set_gate(11, (int32_t)isr11, 0x08, 0x8E);
	idt_set_gate(12, (int32_t)isr12, 0x08, 0x8E);
	idt_set_gate(13, (int32_t)isr13, 0x08, 0x8E);
	idt_set_gate(14, (int32_t)isr14, 0x08, 0x8E);
	idt_set_gate(15, (int32_t)isr15, 0x08, 0x8E);
	idt_set_gate(16, (int32_t)isr16, 0x08, 0x8E);
	idt_set_gate(17, (int32_t)isr17, 0x08, 0x8E);
	idt_set_gate(18, (int32_t)isr18, 0x08, 0x8E);
	idt_set_gate(19, (int32_t)isr19, 0x08, 0x8E);
	idt_set_gate(20, (int32_t)isr20, 0x08, 0x8E);
	idt_set_gate(21, (int32_t)isr21, 0x08, 0x8E);
	idt_set_gate(22, (int32_t)isr22, 0x08, 0x8E);
	idt_set_gate(23, (int32_t)isr23, 0x08, 0x8E);
	idt_set_gate(24, (int32_t)isr24, 0x08, 0x8E);
	idt_set_gate(25, (int32_t)isr25, 0x08, 0x8E);
	idt_set_gate(26, (int32_t)isr26, 0x08, 0x8E);
	idt_set_gate(27, (int32_t)isr27, 0x08, 0x8E);
	idt_set_gate(28, (int32_t)isr28, 0x08, 0x8E);
	idt_set_gate(29, (int32_t)isr29, 0x08, 0x8E);
	idt_set_gate(30, (int32_t)isr30, 0x08, 0x8E);
	idt_set_gate(31, (int32_t)isr31, 0x08, 0x8E);

	/*unsigned char a1;
	unsigned char a2;

	a1 = inb(0x21);
	a2 = inb(0xA1);*/

	// Remap PIC
	// Start initialisation sequence
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	// Master PIC vector offset
	outb(0x21, 0x20);
	// Slave PIC vector offset
	outb(0xA1, 0x28);

	// Tell master PIC there is a slave pic at IRQ2 (0000 0100)
	outb(0x21, 0x04);
	// Tell Slave PIC its cascade identity (0000 0010)
	outb(0xA1, 0x02);

	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	// Restore saved masks
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	idt_set_gate(32, (int32_t)irq0, 0x08, 0x8E);
	idt_set_gate(33, (int32_t)irq1, 0x08, 0x8E);
	idt_set_gate(34, (int32_t)irq2, 0x08, 0x8E);
	idt_set_gate(35, (int32_t)irq3, 0x08, 0x8E);
	idt_set_gate(36, (int32_t)irq4, 0x08, 0x8E);
	idt_set_gate(37, (int32_t)irq5, 0x08, 0x8E);
	idt_set_gate(38, (int32_t)irq6, 0x08, 0x8E);
	idt_set_gate(39, (int32_t)irq7, 0x08, 0x8E);
	idt_set_gate(40, (int32_t)irq8, 0x08, 0x8E);
	idt_set_gate(41, (int32_t)irq9, 0x08, 0x8E);
	idt_set_gate(42, (int32_t)irq10, 0x08, 0x8E);
	idt_set_gate(43, (int32_t)irq11, 0x08, 0x8E);
	idt_set_gate(44, (int32_t)irq12, 0x08, 0x8E);
	idt_set_gate(45, (int32_t)irq13, 0x08, 0x8E);
	idt_set_gate(46, (int32_t)irq14, 0x08, 0x8E);
	idt_set_gate(47, (int32_t)irq15, 0x08, 0x8E);

	printk(KERN_INFO, "IDT has been initialised at %p", &idt_ptr);

	// Init pit to get ticks
	init_pit();

	/* Init rtc to get date from bios */
	init_rtc();

    date(0);
}

