/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:26:33 by lubenard          #+#    #+#             */
/*   Updated: 2022/03/08 14:13:48 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "isr.h"
#include "../../lib/iolib.h"
#include "../../io/io.h"
#include "../../drivers/vga/vga.h"
#include "../../io/shell/builtins/builtins.h"

/*
 * US RW  P - Description
 * 0  0  0 - Supervisory process tried to read a non-present page entry
 * 0  0  1 - Supervisory process tried to read a page and caused a protection fault
 * 0  1  0 - Supervisory process tried to write to a non-present page entry
 * 0  1  1 - Supervisory process tried to write a page and caused a protection fault
 * 1  0  0 - User process tried to read a non-present page entry
 * 1  0  1 - User process tried to read a page and caused a protection fault
 * 1  1  0 - User process tried to write to a non-present page entry
 * 1  1  1 - User process tried to write a page and caused a protection fault
 */

void panic(const char *message, const char *file, unsigned int line) {
	// Why is there some commented registers ?
	// Some registers are called callee-saved and caller-saved registry.
	// We want the caller-save registry.
	// See here for more:
	// https://en.wikipedia.org/wiki/X86_calling_conventions#Caller-saved_(volatile)_registers
	// Registers are in this order:
	//				eax, ebx, ecx, edx, esi, edi, esp, ebp, eip
	char *registers_names_e[] = {/*"eax",*/ "EBX",/* "ecx", "edx",*/ "ESI", "EDI", "ESP", "EBP", "EIP", 0};
	//char *registers_names_cr[] = {"cr0", "cr1", "cr2", "cr3", "cr4", 0};
	int registers_e[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int registers_cr[] = {0, 0, 0, 0, 0};
	int i = 0;

	// Disable cli
	asm volatile("cli");
	printk(KERN_ERROR, "KERNEL PANIC ! '%s' at %s:%d", message, file, line);
	// Save registers value
	//asm volatile ("mov %%eax, %0" : "=r"(registers[0]));
	asm volatile ("mov %%ebx, %0" : "=r"(registers_e[1]));
	//asm volatile ("mov %%ecx, %0" : "=r"(registers[2]));
	//asm volatile ("mov %%edx, %0" : "=r"(registers[3]));
	asm volatile ("mov %%esi, %0" : "=r"(registers_e[4]));
	asm volatile ("mov %%edi, %0" : "=r"(registers_e[5]));
	asm volatile ("mov %%esp, %0" : "=r"(registers_e[6]));
	asm volatile ("mov %%ebp, %0" : "=r"(registers_e[7]));
	//asm("\t movl %%eip,%0" : "=r"(registers[8]));

	asm volatile ("mov %%cr0, %0" : "=r"(registers_cr[9]));
	//asm volatile ("mov %%cr1, %0" : "=r"(registers[10]));
	asm volatile ("mov %%cr2, %0" : "=r"(registers_cr[11]));
	asm volatile ("mov %%cr3, %0" : "=r"(registers_cr[12]));
	asm volatile ("mov %%cr4, %0" : "=r"(registers_cr[13]));

	while (registers_names_e[i] != 0) {
		printk(KERN_NORMAL, "%s: %.8x ", registers_names_e[i], registers_e[i]);
		if ((i + 1) % 3 == 0)
			printk(KERN_NORMAL, "\n");
		i++;
	}
	// Infinite loop to stop the kernel
	for (;;);
}

static const char *interrupt_message[] = {
	"Division by 0", "Debug exception", "Non maskable interrupt",
	"Breakpoint exception", "Overflow detected",
	"Out of bounds exception", "Invalid opcode", "No coprocessor exception",
	"Double fault", "Coprocessor segment overrun", "Bad TSS",
	"Segment not present", "Stack fault", "General protection fault",
	"Page fault", "Unknown interrupt exception", "x87 Floating point exception",
	"Alignement check exception", "Machine check exception",
	"SIMD Floating point exception", "Virtualization exception",
	"Unknown interrupt exception", "Security exception",
	"Unknown interrupt exception", "Triple fault",
};

void page_fault_handler(registers_t regs) {
	/* Getting faulty adress from cr2 register */
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	if (!(regs.err_code & 0x1))
		printk(KERN_ERROR, "Page not present !"); // Page not present
	if (regs.err_code & 0x2)
		printk(KERN_ERROR, "Read-only page !"); // Write operation?
	if (regs.err_code & 0x4)
		printk(KERN_ERROR, "User-mode !"); // Processor was in user-mode?
	if (regs.err_code & 0x8)
		printk(KERN_ERROR, "Bits reserved !"); // Overwritten CPU-reserved bits of page entry?
	printk(KERN_NORMAL, "\nError happened at 0x%x\n", faulting_address);
	// Will not be needed once process are implemented
	PANIC(interrupt_message[regs.int_no]);
}

/*
 * This gets called from our ASM interrupt handler.
 */
void isr_handler(registers_t regs) {
	if (regs.int_no < 25)
		printk(KERN_ERROR, "Received interrupt: %d: %s", regs.int_no,
		interrupt_message[regs.int_no]);
	else
		printk(KERN_ERROR, "Received interrupt: %d", regs.int_no);
	// 14 -> Page fault error code
	if (regs.int_no == 14)
		page_fault_handler(regs);
	// Should be only FATAL errors
	if (regs.int_no == 0x8 || regs.int_no == 0x12) {
		PANIC(interrupt_message[regs.int_no]);
	}
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

	//printk(KERN_ERROR, "IRQ fired %d", regs.int_no);
	if (regs.int_no >= 32) {
		if (irq_routines[regs.int_no - 32] != 0) {
			handler = irq_routines[regs.int_no - 32];
			handler(regs);
		} /*else {
			
			//printk(KERN_ERROR, "Unhandled IRQ ! IRQ code : %d", regs.int_no - 32);
		}*/
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
