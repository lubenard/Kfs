# Kfs-1 

I decided to use clang instead of gcc, because clang is cross-compiler bu default.

Using arch, i also needed to install:

```
sudo pacman -S grub-rescue xorriso mtools qemu
```

You can check if you have the required tools using the scripts in scripts/checkTools.sh

Once compiled, you can test your iso using qemu:

```
qemu-system-x86_64 -cdrom myos.iso
```
