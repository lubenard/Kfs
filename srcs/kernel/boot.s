; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    boot.s                                             :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/05/17 00:20:13 by lubenard          #+#    #+#              ;
;    Updated: 2021/11/26 19:04:23 by lubenard         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; Declare a multiboot header that marks the program as a kernel. These are magic
; values that are documented in the multiboot standard. The bootloader will
; search for this signature in the first 8 KiB of the kernel file, aligned at a
; 32-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 8 KiB of the kernel file.
MAGIC           equ 0xE85250D6 ; 'magic number' lets bootloader find the header
ARCHITECTURE    equ 0 ; MULTIBOOT_ARCHITECTURE_I386
LENGTH          equ (multiboot_header_end - multiboot_header)
CHECKSUM        equ (0x100000000 -(MAGIC + ARCHITECTURE + LENGTH))

section .multiboot
    align 8
    multiboot_header:
	    dd MAGIC
        dd ARCHITECTURE
        dd LENGTH ; Length
        dd CHECKSUM   ; checksum of above, to prove we are multiboot

    align 8
    entry_address_tag:
    	dw 3 ; MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS
    	dw 1
    	dd (entry_address_tag_end - entry_address_tag)
    	dd start
    entry_address_tag_end:

    ;align 8
    ;framebuffer_tag_start:
    ;    dw 5 ; MULTIBOOT_HEADER_TAG_FRAMEBUFFER
    ;    dw 1 ; MULTIBOOT_HEADER_TAG_OPTIONAL
    ;    dd framebuffer_tag_end - framebuffer_tag_start
    ;    dd 1024
    ;    dd 768
    ;    dd 32
    ;framebuffer_tag_end:

    align 8
    dw 0 ; MULTIBOOT_HEADER_TAG_END
    dw 0
    dd 8

    multiboot_header_end:
    global start
    start:
	    ; Enable SSE instructions:
	    ; Sse introduce 70 new instructions.
	    ; It fixed the bug of invalid opcode with keyboard IRQ
	    ; See more here https://wiki.osdev.org/SSE
	    ;mov eax, cr0        ; cr0 cannot be manipulated directly, manipulate eax instead
	    ;and ax, 0xFFFB      ; clear coprocessor emulation CR0.EM
	    ;or ax, 0x2          ; set coprocessor monitoring  CR0.MP
	    ;mov cr0, eax
	    ;mov eax, cr4        ; cr4 too cannot be manipulated directly
	    ;or ax, 3 << 9       ; set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	    ;mov cr4, eax

	    ; The bootloader has loaded us into 32-bit protected mode on a x86
	    ; machine. Interrupts are disabled. Paging is disabled. The processor
	    ; state is as defined in the multiboot standard. The kernel has full
	    ; control of the CPU. The kernel can only make use of hardware features
	    ; and any code it provides as part of itself. There's no printf
	    ; function, unless the kernel provides its own <stdio.h> header and a
	    ; printf implementation. There are no security restrictions, no
	    ; safeguards, no debugging mechanisms, only what the kernel provides
	    ; itself. It has absolute and complete power over the
	    ; machine.

	    ; To set up a stack, we set the esp register to point to the top of our
	    ; stack (as it grows downwards on x86 systems). This is necessarily done
	    ; in assembly as languages such as C cannot function without a stack.

	    mov esp, stack_top

	    ; This is a good place to initialize crucial processor state before the
	    ; high-level kernel is entered. It's best to minimize the early
	    ; environment where crucial features are offline. Note that the
	    ; processor is not fully initialized yet: Features such as floating
	    ; point instructions and instruction set extensions are not initialized
	    ; yet.

	    ; We push eax and ebx on the stack to get infos about the memory map.
	    ; Thankfully, grub is handling this for us.
	    ; See more here: https://wiki.osdev.org/Detecting_Memory_(x86)#Memory_Map_Via_GRUB
	    ; If grub is not present, we need to ask the bios via : INT 0x15, EAX = 0xE820

	    ;push eax
	    ;push ebx

	    ; We need to setup ebp to null to avoid running into gargabe when getting stacktrace
	    xor ebp, ebp       ; Set %ebp to NULL

	    ; Enter the high-level kernel. The ABI requires the stack is 16-byte
	    ; aligned at the time of the call instruction (which afterwards pushes
	    ; the return pointer of size 4 bytes). The stack was originally 16-byte
	    ; aligned above and we've since pushed a multiple of 16 bytes to the
	    ; stack since (pushed 0 bytes so far) and the alignment is thus
	    ; preserved and the call is well defined.
	    ; note, that if you are building on Windows, C functions may have "_" prefix in assembly: _kernel_main

        ;  Reset EFLAGS. */
        push   0
        popf

        ;  Push the pointer to the Multiboot information structure. */
        push   ebx
        ;  Push the magic value. */
        push   eax
        extern k_main
        call k_main

	    cli
    .hang:
    	hlt
	    jmp .hang
    .end:

; The multiboot standard does not define the value of the stack pointer register
; (esp) and it is up to the kernel to provide a stack. This allocates room for a
; small stack by creating a symbol at the bottom of it, then allocating 16384
; bytes for it, and finally creating a symbol at the top. The stack grows
; downwards on x86. The stack is in its own section so it can be marked nobits,
; which means the kernel file is smaller because it does not contain an
; uninitialized stack. The stack on x86 must be 16-byte aligned according to the
; System V ABI standard and de-facto extensions. The compiler will assume the
; stack is properly aligned and failure to align the stack will result in
; undefined behavior.
section .bss
    align 16
    stack_bottom:
        resb 16384 ; 16 KiB
    stack_top: