#Using clang because it cross compile by default, no need to compile own toolchain
CC_C = clang
CC_ASM = nasm

NAME = kfs-2.bin
ISO_NAME = kfs-2.iso

SRCDIR = srcs

SRC_FILES_ASM = kernel/boot.s \
				kernel/gdt/gdt_asm.s \
				kernel/idt/idt_asm.s \
				kernel/isr/isr_asm.s

SRC_FILES_C = kernel/kernel.c \
			  drivers/vga/vga.c \
			  drivers/PS2_keyboard/PS2_keyboard.c \
			  kernel/gdt/gdt.c \
			  kernel/idt/idt.c \
			  kernel/isr/isr.c \
			  lib/memlib.c \
			  lib/strlib.c \
			  lib/printk/printk.c \
			  lib/printk/parse.c \
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
			  io/shell/builtins/print_stack.c \
			  io/shell/builtins/shutdown.c \
			  io/shell/builtins/kbd.c

SRCS_C = $(addprefix $(SRCDIR)/, $(SRC_FILES_C))
SRCS_ASM = $(addprefix $(SRCDIR)/, $(SRC_FILES_ASM))

OBJ_C = $(SRCS_C:.c=.o)
OBJ_ASM = $(SRCS_ASM:.s=.o)

CFLAGS = -Wall -Wextra -Werror \
		 -m32 --target=i686-elf-clang \
		 -fno-builtin \
		 -fno-exceptions \
		 -fno-stack-protector \
		 -nostdlib \
		 -nodefaultlibs

all:  $(NAME)

$(NAME): $(OBJ_C) $(OBJ_ASM)
	@printf "\033[33mLinking of $(NAME)...\033[0m"
	@$(CC_C) -m32 -T linker.ld -o $(NAME) -ffreestanding -O2 -nostdlib $(OBJ_ASM) $(OBJ_C) -lgcc
	@printf "\033[32m[✓]\033[0m\n"
	@grub-file --is-x86-multiboot $(NAME)
	@mkdir -p isodir/boot/grub
	@cp $(NAME) isodir/boot/$(NAME)
	@cp grub.cfg isodir/boot/grub/grub.cfg
	@grub-mkrescue -o $(ISO_NAME) isodir

%.o : %.s
	@printf "\033[36mCompilation de $<...\033[0m"
	@$(CC_ASM) -felf32 $< -o $@
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

launch: all
	qemu-system-x86_64 -cdrom $(ISO_NAME)

.SILENT:

.PHONY: all re fclean clean
