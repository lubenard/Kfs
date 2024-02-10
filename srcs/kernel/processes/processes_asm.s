; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    processes_asm.s                                    :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2023/05/03 15:23:32 by lubenard          #+#    #+#              ;
;    Updated: 2023/05/03 20:14:57 by lubenard         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

GLOBAL switch_regs

switch_regs:
    cli

    ; Move general registers
    mov ecx, [eax + 4]
    mov edx, [eax + 8]
    mov ebx, [eax + 12]
    mov ebp, [eax + 20]
    mov esi, [eax + 24]
    mov edi, [eax + 28]
    ; Right now, eax, ebp, esp are not restored yet

    ; 0x23 is the User Data Segment
    push 0x23
    ; Esp is the stack pointer
    push dword [eax + 16]

    ; Push new eflags on the stack
    push dword [eax + 32]
    ;popfd ; Pop last value on stack & Load it into eflags

    ; Push User Code Segment value on the stack
    push 0x1b

    ; Push eip on the stack
    push dword [eax + 40]

    ; Load eax here
    mov eax, [eax + 0]

    ; Select User Data Segment (0x20 | 0x3 = 0x23) and move it onto every register excepted cs
    ; Previously, it is 0x10 (0x10 | 0x0 = 0x10). The second hex number represent the ring we actually want
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    iret

