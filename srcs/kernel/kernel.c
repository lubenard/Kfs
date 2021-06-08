/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/06/08 18:01:13 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../drivers/vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../io/shell/shell.h"
#include "memory/memory.h"
#include "../lib/iolib.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
# error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
# error "This kernel needs to be compiled with a ix86-elf compiler. You can use clang to compile."
#endif

void display_boot_message() {
	terminal_writestr(" _ _ _     _\n| | | |___| |___ ___ _____ ___\n"
	"| | | | -_| |  _| . |     | -_|\n|_____|___|_|___|___|_|_|_|___|\n\n");
}

/*
 * First kernel called function
 */
void k_main(multiboot_info_t* mb_mmap, unsigned int magic) {
	/* Initialize terminal interface */
	terminal_initialize();

	/* Initialize GDT (Global descriptor table) */
	init_gdt();

	/* Initialize IDT (Interruptor descriptor table) */
	init_idt();

	/* Init kbd management */
	init_kbd();

	(void) magic;
	if (!(mb_mmap->flags & (1<<6))) {
		printk(KERN_ERROR, "Couldn't get memory map!");
	}

	multiboot_memory_map_t* entry = (multiboot_memory_map_t*)mb_mmap->mmap_addr;
	while(entry < ((multiboot_memory_map_t*)mb_mmap->mmap_addr + mb_mmap->mmap_length)) {
		if (entry->type == 1) {
			printk(KERN_INFO, "Ram is available addr_low %d addr_high %d", entry->addr_low, entry->addr_high);
			printk(KERN_INFO, "\tSize of %d %d", entry->len_low, entry->len_high);
		}
		entry = (multiboot_memory_map_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
	}

	/* Enable memory management. Enable paging, userspace and kernel space */
	init_memory();

	display_boot_message();

	/* Init shell management */
	init_shell();
}
