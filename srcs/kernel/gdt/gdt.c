/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 17:35:12 by lubenard          #+#    #+#             */
/*   Updated: 2023/12/05 13:26:24 by luca             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gdt.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"

GDTEntry_t gdt_entries[7];
tss_entry_t tss_entry;

/**
 * Set the value of one GDT entry.
 * @param num the entry we fill (between 0 and 4)
 */
static void gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	// Set all base values to 0
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	// limit is 0xFFFFFFFF -> 4294967295 in decimal, for full access to 4gb of ram
	// Set size to 0xFFFF -> 65535 in decimal
	gdt_entries[num].size = (limit & 0xFFFF);
	// Granularity will be equal to 0xF
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;
	gdt_entries[num].granularity |= gran & 0xF0;

	gdt_entries[num].access = access;
}

static void setup_tss(uint32_t num, uint16_t ss0, uint32_t esp0) {
    // Firstly, let's compute the base and limit of our entry into the GDT.
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    // Now, add our TSS descriptor's address to the GDT.
    gdt_set_gate(num, base, limit, 0xE9, 0x00);

    // Ensure the descriptor is initially zero.
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;  // Set the kernel stack segment.
    tss_entry.esp0 = esp0; // Set the kernel stack pointer.

    // Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what
    // segments should be loaded when the processor switches to kernel mode. Therefore,
    // they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
    // but with the last two bits set, making 0x0b and 0x13. The setting of these bits
    // sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
    // to switch to kernel mode from ring 3.
    tss_entry.cs = 0x0b;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}


void init_gdt()
{
	GDTDescriptor_t gdt_ptr;

    // Disable interruptions
    asm volatile ("cli");

	// Size is 5 because we have 5 entries
	gdt_ptr.size = (sizeof(GDTEntry_t) * 6) - 1;
	// Offset if the address of the first entry;
	gdt_ptr.offset = (uint32_t)GDT_BASE;

	// Null Segment is mandatory. Everything is null in this segment
	gdt_set_gate(0, 0, 0, 0, 0);

	// 0x9A -> 1001 1010 in binary: -> Ring 0, Kernel Code segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	// 0x92 -> 1001 0010 in binary: -> Ring 0, Kernel Data segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	// 0x96 -> 1001 0110 in binary: -> Ring 0, Data segment
	// For more info about how access byte is defined, have a look here:
	// https://wiki.osdev.org/Global_Descriptor_Table
	//gdt_set_gate(3, 0, 0xFFFFFFFF, 0x96, 0xCF); // Kernel stack segment

	// 0xFA -> 1111 1010 in binary: -> Ring 3, (11 in binary), User Code segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

	// 0xF2 -> 1111 0010 in binary: -> Ring 3, (11 in binary), User Data segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	// 0xF6 -> 1111 0110 in binary: -> Ring 3, Data segment
	//gdt_set_gate(6, 0, 0xFFFFFFFF, 0xF6, 0xCF); // User stack segment

    setup_tss(5, 0x10, 0x0);

    memcpy((char *)gdt_ptr.offset, (char *)gdt_entries, sizeof(GDTEntry_t) * 7);

	// Load new created GDT & TSS
	gdt_flush((uint32_t)&gdt_ptr);
    tss_flush();

    // Re-enable interruptions
    asm volatile ("sti");
    printk(KERN_INFO, "TSS initialised");
    printk(KERN_INFO, "GDT has been initialised at %p with %d entries", gdt_ptr.offset, gdt_ptr.size);
}
