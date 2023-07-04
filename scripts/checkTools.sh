#!/bin/sh

RED="\x1b[31m"
GREEN="\x1b[32m"
END="\x1b[0m"
YELLOW="\x1b[1;33m"

CHECKMARK="\xE2\x9C\x94"

requiredFoundSymbol="$GREEN $CHECKMARK $GREEN"
requiredNotFoundSymbol="$RED/!\\$END"
optionnalSymbol="$YELLOW ~ $END"

printf "$RED- Required -$END\n"
for i in "nasm" "clang" "ld" "mformat" "grub-mkrescue" "qemu-system-x86_64" "xorriso"
do
	if ! type "$i" &> /dev/null; then
		printf "$requiredNotFoundSymbol $RED$i$END is not installed\n"
	else
		printf "$requiredFoundSymbol $GREEN$i$END has been found !\n"
	fi
done
printf "\n"
printf "$GREEN- Optionnal -$END\n"
for i in "xdotool" "gdb"
do
	if ! type "$i" &> /dev/null; then
		printf "$RED$i$END is not installed\n"
	else
		printf "$GREEN$i$END has been found !\n"
	fi
done
