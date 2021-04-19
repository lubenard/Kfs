NAME = kfs-1.bin
ISO_NAME = kfs-1.iso

SRC_ASM = boot.s
SRC_C = kernel.c
INCLUDE_SRC = ./include/

OBJ_ASM = $(SRC_ASM:.s=.o)
OBJ_C = $(SRC_C:.c=.o)

#Using clang because it cross compile by default, no need to compile own toolchain
CC_C = clang
CC_ASM = nasm

CFLAGS = -Wall -Wextra -Werror -g3 \
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
	@$(CC_C) -c $(CFLAGS) -I$(INCLUDE_SRC) $< -o $@
	@printf "\033[32m[✓]\033[0m\n"

clean:
	@rm -rf $(OBJ_ASM)
	@rm -rf $(OBJ_C)
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
