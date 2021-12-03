# Kfs-2

Kfs-2 is the second kernel project.

![Screenshot_20210520_115316](https://user-images.githubusercontent.com/42534397/118958810-0b277180-b962-11eb-827e-57badc451665.png)

## Compile and test this project:

I decided to use clang instead of gcc, because clang is cross-compiler by default.

Using arch, i also needed to install:

```
sudo pacman -S grub-rescue xorriso mtools qemu
```

You can check if you have the required tools using the scripts in scripts/checkTools.sh

Once compiled, you can test your iso using qemu:

```
qemu-system-x86_64 -cdrom myos.iso
```

## How it works

This kernel has a very very basic shell working, type ```help``` to see the available commands.

You can also change screens by pressing: F1, F2, F3.
You can also use left / right arrows to move cursor, and up / down arrows to go to previous / next command.
