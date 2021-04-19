# Kfs-1 

I decided to use clang instead of gcc, because clang is cross-compiler bu default.

Using arch, i also needed to install:

```
sudo pacman -S grub-rescue xorriso mtools qemu
```

Once compiled, you can test your iso using qemu:

```
qemu-system-x86_64 -cdrom myos.iso
```
