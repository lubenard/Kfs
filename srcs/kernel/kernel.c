/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:32 by lubenard          #+#    #+#             */
/*   Updated: 2022/10/09 14:53:12 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"
#include "../drivers/vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../io/shell/shell.h"
#include "memory/memory.h"
#include "memory/grub/grub.h"
#include "../lib/iolib.h"
#include "../lib/memlib.h"
#include "memory/vmm/malloc/malloc.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
# error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
# error "This kernel needs to be compiled with a ix86-elf compiler. You can use clang to compile."
#endif

t_kernel *kernel_struct;

t_kernel *get_kernel_struct() {
	return kernel_struct;
}

void init_kernel_struct() {
	if (!(kernel_struct = malloc(sizeof(t_kernel)))) {
		return;
	}
}

void display_boot_message() {
	terminal_writestr(" _ _ _     _\n| | | |___| |___ ___ _____ ___\n"
	"| | | | -_| |  _| . |     | -_|\n|_____|___|_|___|___|_|_|_|___|\n\n");
}

/*
 * First kernel called function
 */
void k_main(unsigned long magic, unsigned long addr) {
	(void)magic;
	(void)addr;

    /* Enable debug log via port */
    init_com_port(0x3F8);

	/* Initialize terminal interface */
	terminal_initialize();

	/* Initialize GDT (Global descriptor table) */
	init_gdt();

	/* Initialize IDT (Interruptor descriptor table) */
	init_idt();

	/* Init kbd management */
	init_kbd();

	/* Enable memory management. Enable paging */
	//init_memory(mb_mmap);

    //init_kernel_struct();

	//init_processes();

	//register_kernel_as_process();

	//add_signal_to_pid(1, STATUS_WAITING);

    //create_process("Test process", find_process_by_pid(1), 0);
    //create_process("Test process 2", find_process_by_pid(1), 0);

	//display_boot_message();
	/* Init shell management */
	//init_shell();

	// Voluntary Page fault, do not uncomment
	/*uint32_t *ptr = (uint32_t*)0xA0000000;
	uint32_t do_page_fault = *ptr;
	(void)do_page_fault;*/
	//asm volatile ("int $0xE");

	while (1) {}
}
