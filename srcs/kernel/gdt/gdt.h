/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 14:24:56 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 00:36:23 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_H
# define GDT_H

# include <stdint.h>
# define GDT_BASE 0x00000800

struct tss_entry_struct {
    uint32_t prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
    uint32_t esp0;       // The stack pointer to load when we change to kernel mode.
    uint32_t ss0;        // The stack segment to load when we change to kernel mode.
    uint32_t esp1;       // Unused...
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;         // The value to load into ES when we change to kernel mode.
    uint32_t cs;         // The value to load into CS when we change to kernel mode.
    uint32_t ss;         // The value to load into SS when we change to kernel mode.
    uint32_t ds;         // The value to load into DS when we change to kernel mode.
    uint32_t fs;         // The value to load into FS when we change to kernel mode.
    uint32_t gs;         // The value to load into GS when we change to kernel mode.
    uint32_t ldt;        // Unused...
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

/*
 * We use __attribute__((packed)) to tell compiler not to add extra space
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
extern void tss_flush();
extern void check_kernel_long_mode();

/* Prototype to init GDT */
void init_gdt();

#endif
