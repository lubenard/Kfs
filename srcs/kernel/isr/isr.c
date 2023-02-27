/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:26:33 by lubenard          #+#    #+#             */
/*   Updated: 2022/10/13 15:10:22 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "isr.h"
#include "../../lib/iolib.h"
#include "../../io/io.h"
#include "../../drivers/vga/vga.h"
#include "../../io/shell/builtins/builtins.h"

//TODO: Remove when unneeded
#include "../../io/shell/shell.h"
#include "../debug/debug.h"

/* Assume, as is often the case, that EBP is the first thing pushed. If not, we are in trouble. */
struct stackframe {
  struct stackframe* ebp;
  uint32_t eip;
};

void getStackTrace(unsigned int MaxFrames) {
	struct stackframe *stk;
	asm ("movl %%ebp,%0" : "=r"(stk) ::);
	printk(KERN_ERROR, "Stack trace: from last to first call:");
	for (unsigned int frame = 0; stk && frame < MaxFrames; ++frame) {
		// Unwind to previous stack frame
        char *symbol = get_symbol_from_address((void *)stk->eip);

        if (symbol != 0) {
            printk(KERN_ERROR, "%d : 0x%x %s",frame, stk->eip, symbol);
        } else {
            printd(KERN_ERROR, "%d : 0x%x %s",frame, stk->eip, (symbol == 0) ? "Undefined" : symbol);
        }

		stk = stk->ebp;
	}
}

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

void panic(registers_t regs, const char *message, const char *file, unsigned int line) {
	// Why is there some commented registers ?
	// Some registers are called callee-saved and caller-saved registry.
	// We want the caller-save registry.
	// See here for more:
	// https://en.wikipedia.org/wiki/X86_calling_conventions#Caller-saved_(volatile)_registers
	// Registers are in this order:
	//				eax, ebx, ecx, edx, esi, edi, esp, ebp, eip
	char *registers_names_cr[] = {"cr0", "cr1", "cr2", "cr3", "cr4", 0};
	int registers_cr[] = {0, 0, 0, 0, 0};
	int i = 0;

	// Disable cli
	asm volatile("cli");
	printk(KERN_ERROR, "KERNEL PANIC ! '%s' at %s:%d", message, file, line);

	// Save registers value
	asm volatile ("mov %%cr0, %0" : "=r"(registers_cr[0]));
	//asm volatile ("mov %%cr1, %0" : "=r"(registers_cr[1]));
	asm volatile ("mov %%cr2, %0" : "=r"(registers_cr[2]));
	asm volatile ("mov %%cr3, %0" : "=r"(registers_cr[3]));
	asm volatile ("mov %%cr4, %0" : "=r"(registers_cr[4]));

	printk(KERN_NORMAL, "EAX: %.8x EBX: %.8x ECX: %.8x\n", regs.eax, regs.ebx, regs.ecx);
	printk(KERN_NORMAL, "EDX: %.8x ESI: %.8x EDI: %.8x\n", regs.edx, regs.esi, regs.edi);
	printk(KERN_NORMAL, "ESP: %.8x EBP: %.8x EIP: %.8x\n\n", regs.esp, regs.ebp, regs.eip);

	while (registers_names_cr[i] != 0) {
		printk(KERN_NORMAL, "%s: %.8x ", registers_names_cr[i], registers_cr[i]);
		if ((i + 1) % 3 == 0)
			printk(KERN_NORMAL, "\n");
		i++;
	}
	printk(KERN_NORMAL, "\n\n");
	getStackTrace(10);
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

void print_faulty_address() {
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	printk(KERN_NORMAL, "\nError happened at 0x%x\n", faulting_address);
}

void page_fault_handler(registers_t regs) {

	if (!(regs.err_code & 0x1))
		printk(KERN_ERROR, "Page not present !"); // Page not present
	if (regs.err_code & 0x2)
		printk(KERN_ERROR, "Read-only page !"); // Write operation?
	if (regs.err_code & 0x4)
		printk(KERN_ERROR, "User-mode !"); // Processor was in user-mode?
	if (regs.err_code & 0x8)
		printk(KERN_ERROR, "Bits reserved !"); // Overwritten CPU-reserved bits of page entry?
	print_faulty_address();
	// Will not be needed for EVERY page fault once process are implemented
	PANIC(regs, interrupt_message[regs.int_no]);
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

	if (regs.int_no == 0
		|| (regs.int_no >= 5 && regs.int_no <= 7)
		|| (regs.int_no >= 11 && regs.int_no <= 13)
		|| regs.int_no == 16 || regs.int_no == 17
		|| regs.int_no == 18) {
			print_faulty_address();
		}

	// 14 -> Page fault error code
	if (regs.int_no == 14)
		page_fault_handler(regs);
	// Should be only FATAL errors
	if (regs.int_no == 0x8 || regs.int_no == 0x12) {
		PANIC(regs, interrupt_message[regs.int_no]);
	}
	// to remove when implementing processus. this is to avoid getting infinite interrupts
	while (1) {}
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
 * Clear bit from IMR, restoring irq triggering
 */
void irq_clear_mask(unsigned char IRQline) {
	uint16_t port;
	uint8_t value;

	if (IRQline < 8) {
		port = 0x21;
	} else {
		port = 0xA1;
		IRQline -= 8;
	}
	value = inb(port) & ~(1 << IRQline);
	outb(port, value);
}

/*
 * This set a flag on IMR (Interrupt Mask Register)
 * Calling this function DISABLE interrupt
 * From osdev:
 * When a bit is set, the PIC ignores the request and continues normal operation
 */
void irq_set_mask(unsigned char IRQline) {
	uint16_t port;
	uint8_t value;

	if (IRQline < 8) {
		port = 0x21;
	} else {
		port = 0xA1;
		IRQline -= 8;
	}
	value = inb(port) | (1 << IRQline);
	outb(port, value);
}

/*
 * This gets called from our ASM interrupt handler
 */
void irq_handler(registers_t regs) {

	void (*handler)(registers_t r);

	//printk(KERN_INFO, "IRQ fired numero: %d", regs.int_no - 32);
	if (regs.int_no >= 32) {
		if (irq_routines[regs.int_no - 32] != 0) {
			handler = irq_routines[regs.int_no - 32];
			handler(regs);
		} else {
			printk(KERN_ERROR, "Unhandled IRQ ! IRQ code : %d", regs.int_no - 32);
		}
	}
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no - 32 >= 8) {
		//printd(KERN_INFO, "Send reset signal to slave");
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	//printd(KERN_INFO, "Send reset signal to master");
	// Send reset signal to master.
	outb(0x20, 0x20);
}
