/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 18:02:32 by lubenard          #+#    #+#             */
/*   Updated: 2021/05/17 15:31:46 by lubenard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../drivers/vga/vga.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "../io/io.h"
#include "../drivers/PS2_keyboard/PS2_keyboard.h"
#include "../io/shell/shell.h"

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
void k_main(void)
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Initialize GDT (Global descriptor table) */
	init_gdt();

	/* Initialize IDT (Interruptor descriptor table) */
	init_idt();

	/* Init kbd management */
	init_kbd();

	display_boot_message();

	/* Init shell management */
	init_shell();
}
