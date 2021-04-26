/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:24:56 by lubenard          #+#    #+#             */
/*   Updated: 2021/04/26 17:05:25 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

/*
 * We use __attribute__((packed)) to tell compiler to not add extra space
 * between variables of the structure
 */

struct GDTDescriptor {
	uint16_t size;   // Size of the table - 1
	uint32_t offset; // Address of the first entry of the GDT
} __attribute__((packed));

struct GDTEntry {
	uint16_t size;     // Used by the processor to prevent programs from addressing outside the segment
	uint16_t base_low;      // Lower 16 bits of the base
	uint8_t base_middle;       // Next 8 bits of the base
	uint8_t access;  // Determine what ring this segment can be used in
	uint8_t granularity; // Determine if you have full access of access to 4gb of ram
	uint8_t base_high;       // Upper 8 bits of the base (making base 32 bits)
} __attribute__((packed));

typedef struct GDTEntry GDTEntry_t;
typedef struct GDTDescriptor GDTDescriptor_t;

/* Asm function */
extern void gdt_flush(uint32_t);

/* Prototype to init GDT */
void init_gdt();
