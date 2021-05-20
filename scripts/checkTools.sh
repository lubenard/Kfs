#!/bin/sh

RED="\x1b[31m"
GREEN="\x1b[32m"
END="\x1b[0m"

commandToCheck=("nasm" "clang" "ld" "mformat" "grub-mkrescue" "qemu-system-x86_64" "xorriso")

for i in "${commandToCheck[@]}"
do
	if ! type "$i" &> /dev/null; then
		printf "$RED$i$END is not installed\n"
	else
		printf "$GREEN$i$END has been found !\n"
	fi
done
