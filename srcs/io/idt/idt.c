/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 21:45:15 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/02 16:24:38 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "idt.h"
#include "../../lib/lib.h"

struct idt_ptr_struct idt_ptr;
struct idt32 idt_entries[256];

static void idt_set_gate(int8_t num, int32_t base, int16_t sel, int8_t flags)
{
	idt_entries[num].base_low = base & 0xFFFF;
	idt_entries[num].base_mid = (base >> 16) & 0xFFFF;

	idt_entries[num].selector = sel;
	idt_entries[num].zero = 0;
	// We must uncomment the OR below when we get to using user-mode.
	// It sets the interrupt gate's privilege level to 3.
	idt_entries[num].flags   = flags /* | 0x60 */;
}

void init_idt()
{
	idt_ptr.limit = sizeof(struct idt32) * 256 - 1;
	idt_ptr.base  = (int32_t)&idt_entries;

	memset(&idt_entries, 0, sizeof(struct idt32)*256);

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

	// Tell our bios to load our new IDT
	idt_flush((int32_t)&idt_ptr);
	printk(KERN_INFO, "IDT has been initialised at %p", &idt_ptr);
}

