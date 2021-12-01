/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:44:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/12/01 19:18:13 by lubenard         ###   ########.fr       */
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

typedef struct registers {
	unsigned int gs, fs, es, ds;      /* pushed the segs last */
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
	unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
} registers_t;

typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(int8_t n, isr_t handler);

#endif
