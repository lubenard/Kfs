#!/bin/sh

RED="\x1b[31m"
GREEN="\x1b[32m"
END="\x1b[0m"
YELLOW="\x1b[1;33m"

requiredTools=("nasm" "clang" "ld" "mformat" "grub-mkrescue" "qemu-system-x86_64" "xorriso")

optionnalTools=("xdotool" "gdb")

requiredSymbol="$RED/!\ $END"
optionnalSymbol="$YELLOW ~ $END"

printf "$RED- Required -$END\n"
for i in "${requiredTools[@]}"
do
	if ! type "$i" &> /dev/null; then
		printf "$requiredSymbol $RED$i$END is not installed\n"
	else
		printf "$requiredSymbol $GREEN$i$END has been found !\n"
	fi
done
printf "\n"
printf "$GREEN- Optionnal -$END\n"
for i in "${optionnalTools[@]}"
do
	if ! type "$i" &> /dev/null; then
		printf "$optionnalSymbol $RED$i$END is not installed\n"
	else
		printf "$optionnalSymbol $GREEN$i$END has been found !\n"
	fi
done
