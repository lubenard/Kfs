/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 17:35:12 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/18 23:15:41 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gdt.h"
#include "../../lib/iolib.h"
#include "../../lib/memlib.h"

GDTEntry_t gdt_entries[7];

/* Set the value of one GDT entry.
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

void init_gdt()
{
	GDTDescriptor_t gdt_ptr;

	// Size is 5 because we have 5 entries
	gdt_ptr.size = (sizeof(GDTEntry_t) * 5) - 1;
	// Offset if the adress of the first entry;
	gdt_ptr.offset = (uint32_t)GDT_BASE;

	// Null Segment is mandatory. Everything is null in this segment
	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment

	// 0x9A -> 1001 1010 in binary: -> Ring 0, Code segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code segment

	// 0x92 -> 1001 0010 in binary: -> Ring 0, Data segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment

	// 0x96 -> 1001 0110 in binary: -> Ring 0, Data segment
	// For more info about how acces byte is defined, have a look here:
	// https://wiki.osdev.org/Global_Descriptor_Table
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0x96, 0xCF); // Kernel stack segment

	// 0xFA -> 1111 1010 in binary: -> Ring 3, (11 in binary), Code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment

	// 0xF2 -> 1111 0010 in binary: -> Ring 3, (11 in binary), Data segment
	gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment

	// 0xF6 -> 1111 0110 in binary: -> Ring 3, Data segment
	gdt_set_gate(6, 0, 0xFFFFFFFF, 0xF6, 0xCF); // User stack segment

	memcpy((char *)gdt_ptr.offset, (char *)gdt_entries, sizeof(GDTEntry_t) * 5);

	// Load new created GDT
	gdt_flush((uint32_t)&gdt_ptr);

	printk(KERN_INFO, "GDT has been initialised at %p", gdt_ptr.offset);
}
