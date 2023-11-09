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

extern printd

message:
        db      'Ecx is %p', 0

switch_regs:
    cli
    ; Debug message
    ;push dword [eax + 4]
    ;push message
    ;push 0x1
    ;call  printd

    ; Move general registers
    mov ecx, [eax + 4]
    mov edx, [eax + 8]
    mov ebx, [eax + 12]
    mov ebp, [eax + 20]
    mov esi, [eax + 24]
    mov edi, [eax + 28]
    ; Right now, eax, ebp, esp are not restored yet

    push 0x23
    push esp

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

