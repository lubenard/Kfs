; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    gdt_asm.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/05/17 00:36:47 by lubenard          #+#    #+#              ;
;    Updated: 2021/05/17 00:36:49 by lubenard         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

[GLOBAL gdt_flush]    ; Allows the C code to call gdt_flush().
[GLOBAL tss_flush]    ; Allows our C code to call tss_flush().

tss_flush:
   mov ax, 0x2B      ; Load the index of our TSS structure - The index is
                     ; 0x28, as it is the 5th selector and each is 8 bytes
                     ; long, but we set the bottom two bits (making 0x2B)
                     ; so that it has an RPL of 3, not zero.
   ltr ax            ; Load 0x2B into the task state register.
   ret

gdt_flush:
	mov eax, [esp + 4]  ; Get the pointer to the GDT, passed as a parameter.
					  ; esp points to argc, and argc being a 'int', we jump 4 bytes
					  ; later to go on argv[0]
	lgdt [eax]        ; Load the new GDT pointer given as parameter

	mov ax, 0x10      ; 0x10 is the offset in the GDT to our kernel data segment
					  ; Since each GDT entry is 8 bytes, and kernel code descriptor
					  ; is 3rd (after Null segment and Kernel code segment).
					  ; 0 -> 7   : Null segment
					  ; 8 -> 15  : Kernel code segment
					  ; 16 -> 23 : Kernel data segment
					  ; In hexa, 16 is 0x10
	mov ds, ax        ; ds is for data segment
	mov es, ax        ; es is for extra segment
	mov fs, ax
	mov gs, ax
	mov ss, ax        ; ss is for stack segment
	jmp 0x08:.flush   ; 0x08 is the offset to our code segment
.flush:
	ret				  ; return
