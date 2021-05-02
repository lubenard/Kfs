/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isr.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 16:44:38 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/02 16:44:40 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

typedef struct registers
{
   int32_t ds; // Data segment selector
   int32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   int32_t int_no, err_code; // Interrupt number and error code (if applicable)
   int32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically
} registers_t;
