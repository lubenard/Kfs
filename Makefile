# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/14 14:52:43 by lubenard          #+#    #+#              #
#    Updated: 2022/09/13 18:12:00 by lubenard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Using clang because it cross compile by default, no need to compile own toolchain
CC_C = clang
CC_ASM = nasm

NAME = kfs-5.bin
ISO_NAME = kfs-5.iso

SRCDIR = srcs

DEBUG_LOG = 0
IS_TEST = 0

SRC_FILES_ASM = kernel/boot.s \
				kernel/gdt/gdt_asm.s \
				drivers/cpu/cpu_asm.s \
				kernel/idt/idt_asm.s \
				kernel/isr/isr_asm.s \
				kernel/memory/paging_asm.s \
				kernel/processes/processes_asm.s \
				kernel/syscalls/syscalls_asm.s

SRC_FILES_C = kernel/kernel.c \
			  drivers/vga/vga.c \
			  drivers/PS2_keyboard/PS2_keyboard.c \
			  drivers/cpu/cpu.c \
			  kernel/gdt/gdt.c \
			  kernel/idt/idt.c \
			  kernel/isr/isr.c \
			  kernel/isr/irqs/pit.c \
			  kernel/isr/irqs/rtc.c \
			  lib/memlib.c \
			  lib/iolib.c \
			  lib/intlib.c \
			  lib/strlib.c \
			  lib/charlib.c \
			  lib/strarraylib.c \
			  lib/bitwiselib.c \
			  lib/printk/printk.c \
			  lib/printk/parse.c \
			  lib/printk/minwidth.c \
			  lib/printk/fill_buffer.c \
			  lib/printk/flags/flag_c.c \
			  lib/printk/flags/flag_di.c \
			  lib/printk/flags/flag_s.c \
			  lib/printk/flags/flag_u.c \
			  lib/printk/flags/flag_o.c \
			  lib/printk/flags/flag_x.c \
			  lib/printk/flags/flag_p.c \
			  io/io.c \
			  io/shell/shell.c \
			  io/shell/builtins/print.c \
			  io/shell/builtins/echo.c \
			  io/shell/builtins/uptime.c \
			  io/shell/builtins/help.c \
			  io/shell/builtins/shutdown.c \
			  io/shell/builtins/kbd.c \
			  io/shell/builtins/clear.c \
			  io/shell/builtins/date.c \
			  io/shell/builtins/test_process.c \
			  io/shell/builtins/top.c \
			  kernel/memory/memory.c \
			  kernel/memory/memory_infos.c \
			  kernel/grub/grub.c \
			  kernel/debug/debug.c \
			  kernel/memory/page_directory.c \
			  kernel/memory/pmm/pmm.c \
			  kernel/memory/vmm/vmm.c \
			  kernel/memory/vmm/malloc/malloc.c \
			  kernel/memory/vmm/malloc/free.c \
			  kernel/memory/vmm/malloc/calloc.c \
			  kernel/memory/vmm/malloc/realloc.c \
			  kernel/processes/processes.c \
			  kernel/processes/scheduler.c \
			  kernel/processes/signals.c #\
			  #tests/tests.c

SRCS_C = $(addprefix $(SRCDIR)/, $(SRC_FILES_C))
SRCS_ASM = $(addprefix $(SRCDIR)/, $(SRC_FILES_ASM))

OBJ_C = $(SRCS_C:.c=.o)
OBJ_ASM = $(SRCS_ASM:.s=.o)

CFLAGS = -Wall -Wextra -Werror \
		 -m32 --target=i686-elf-clang \
		 -fno-builtin \
		 -fno-exceptions \
		 -fno-pic \
		 -fno-pie \
		 -fno-stack-protector \
		 -nostdlib \
		 -nodefaultlibs \
		 -mgeneral-regs-only \
		 -fno-omit-frame-pointer

ifeq ($(DEBUG_LOG),1)
		CFLAGS += -g3
	endif

ifeq ($(IS_TEST),1)
	CFLAGS += -DLAUNCH_TESTS=1
endif

all:  $(NAME)

$(NAME): $(OBJ_ASM) $(OBJ_C)
	@printf "\033[33mLinking of $(NAME)...\033[0m"
	@$(CC_C) -m32 -T linker.ld -o $(NAME) -fno-pic -fno-pie -nostartfiles -no-pie -ffreestanding -O2 -nostdlib $(OBJ_ASM) $(OBJ_C) -lgcc
	@printf "\033[32m[✓]\033[0m\n"
	@grub-file --is-x86-multiboot2 $(NAME)
	@mkdir -p isodir/boot/grub
	@cp $(NAME) isodir/boot/$(NAME)
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO_NAME) isodir

%.o : %.s
	@printf "\033[36mCompilation de $<...\033[0m"
	@$(CC_ASM) -f elf32 $< -o $@
	@printf "\033[32m[✓]\033[0m\n"

%.o : %.c
	@printf "\033[36mCompilation de $<...\033[0m"
	@$(CC_C) -c $(CFLAGS) $< -o $@
	@printf "\033[32m[✓]\033[0m\n"

clean:
	@printf "\033[31mSuppression des *.o...\033[0m"
	@rm -rf $(OBJ_ASM)
	@rm -rf $(OBJ_C)
	@printf "\033[32m[✓]\033[0m\n"
	@rm -rf isodir

fclean: clean
	@printf "\033[31mSuppression de $(NAME)...\033[0m"
	@rm -f $(NAME)
	@printf "\033[32m[✓]\033[0m\n"
	@printf "\033[31mSuppression de $(ISO_NAME)...\033[0m"
	@rm -f $(ISO_NAME)
	@printf "\033[32m[✓]\033[0m\n"

re: fclean all

# This rule has not for goal to be called.
# Ajust the debug log and avoid calling huge rules like 're',
# just delete what is needed
recompile_run:
	@printf "Recompiling Header... Debug = $(DEBUG_LOG)\n"
	@printf "Recompiling Header... IS_TESTS = $(IS_TESTS)\n"
	@sed -i 's/DEBUG_LOG [0-1]/DEBUG_LOG $(DEBUG_LOG)/' srcs/lib/printk/include/printk.h
	@rm -f $(NAME)
	@rm -f srcs/lib/printk/printk.o
	@make

run:
	@make recompile_run DEBUG_LOG=0
	@qemu-system-i386 -m 512 -cdrom $(ISO_NAME)

run_max_memory:
	@make recompile_run DEBUG_LOG=0
	@qemu-system-i386 -m 4096 -cdrom $(ISO_NAME)

run_debug:
	@make recompile_run DEBUG_LOG=1
	@qemu-system-i386 -serial file:log.txt -d int -m 4096 -cdrom $(ISO_NAME)

run_debug_gdb:
	@make recompile_run DEBUG_LOG=1
	@xdotool type "gdb $(NAME) -ex 'target remote localhost:1234' -e ./gdb_commands.txt"
	@xdotool key Return
	# Open new tab and connect gdb to qemu
	@set WID=`xprop -root | grep "_NET_ACTIVE_WINDOW(WINDOW)"| awk '{print $5}'`
	@xdotool windowfocus $WID
	@xdotool key ctrl+shift+t
	@xdotool type "qemu-system-i386 -serial file:log.txt -s -S -d int -m 4096 -cdrom $(ISO_NAME)"
	@xdotool key Return
	@xdotool key ctrl+Left

relaunch: fclean run

check:
	@printf "\033[32mStarting qemu into cli mode\033[0m\n"
	@qemu-system-i386 -nographic -serial file:log.txt -m 512 -cdrom $(ISO_NAME)
	@python tests/check_logs_debug.py
	@printf "\033[32m[✓] all tests passed \033[0m\n"

compile_test: all
	@make -C tests
	$(CC) -g3 tests/main.c tests/test_libft.a libft.a -o testing_libft

test: compile_test
	./testing_libft

help:
	@printf "\033[32m -\033[0m all: Compile iso\n"
	@printf "\033[32m -\033[0m clean: clean .o files\n"
	@printf "\033[32m -\033[0m fclean: clean .o files & delete iso\n"
	@printf "\033[32m -\033[0m re: launch clean & all\n"
	@printf "\033[32m -\033[0m run: build & run the project into qemu\n"
	@printf "\033[32m -\033[0m run_max_memory: build & run the project into qemu with 4GB of ram\n"
	@printf "\033[32m -\033[0m run_debug: build & run the project into qemu with debug flags\n"
	@printf "\033[32m -\033[0m run_debug_gdb: build & run the project into qemu with debug flags and gdb\n"
	@printf "\033[32m -\033[0m relaunch: launch fclean & run\n"
	@printf "\033[32m -\033[0m check: launch checks based on logs\n"
	@printf "\033[32m -\033[0m compile_tests: compile tests\n"
	@printf "\033[32m -\033[0m test: launch tests\n"
	@printf "\033[32m -\033[0m help: print this help\n"

.SILENT:

.PHONY: all re fclean clean
