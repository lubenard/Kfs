/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:44:38 by lubenard          #+#    #+#             */
/*   Updated: 2022/08/19 02:32:24 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISR_H
# define ISR_H

# include <stdint.h>

# define IRQ0 32
# define IRQ1 33
# define IRQ2 34
# define IRQ3 35
# define IRQ4 36
# define IRQ5 37
# define IRQ6 38
# define IRQ7 39
# define IRQ8 40
# define IRQ9 41
# define IRQ10 42
# define IRQ11 43
# define IRQ12 44
# define IRQ13 45
# define IRQ14 46
# define IRQ15 47

#define PANIC(regs, msg) panic(regs, msg, __FILE__, __LINE__);

typedef struct registers
{
	int32_t ds; // Data segment selector
	int32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; // Pushed by pusha.
	int32_t int_no, err_code; // Interrupt number and error code (if applicable)
	int32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically
} registers_t;

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(int8_t n, isr_t handler);
void irq_set_mask(unsigned char IRQline);
void irq_clear_mask(unsigned char IRQline);

void panic(registers_t regs, const char *message, const char *file, unsigned int line);

#endif
