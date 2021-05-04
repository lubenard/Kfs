/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/04 18:42:38 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "drivers/vga/vga.h"
#include "io/gdt/gdt.h"
#include "io/idt/idt.h"
#include "lib/lib.h"
#include "io/io.h"
#include "drivers/PS2_keyboard/PS2_keyboard.h"
#include "io/timer/timer.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
# error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
# error "This kernel needs to be compiled with a ix86-elf compiler. You can use clang to compile."
#endif

void k_main(void)
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Initialize GDT (Global descriptor table) */
	init_gdt();

	/* Initialize IDT (Interruptor descriptor table) */
	init_idt();

	/* Write 42 on the terminal */
	terminal_writestr("42\n");

	asm volatile ("sti");

	init_timer(50);
	//init_kbd();
	//terminal_writestr("> ");

	for(;;) {
		asm("hlt");
	}
}
