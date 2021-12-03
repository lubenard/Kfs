# Kfs-1 

Kfs-1 is the first kernel project.

Subject: [here](en.subject.pdf)

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

This kernel has a very very basic shell working, type ```help``` to see the available commands

You can also change screens by pressing: F1, F2, F3
