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
    mov ecx, [ebp + 4]
    mov edx, [ebp + 8]
    mov ebx, [ebp + 12]
    mov esi, [ebp + 24]
    mov edi, [ebp + 28]
    ;mov eip, [ebp + 40]
    ;mov esp, [ebp + 16]


    ; Restore registers
    ;mov eax, [ebp + 0]
    ;mov ebp, [ebp + 20]


    ; load eflags
    ;mov eax, [ebp + 32]
    ;push eax
    ;popfd
    ; Right now, eax, ebp, esp are not restored yet

    ; Enter usermode from here(make sure the registers are restored correctly for the user process !)
        mov ax, 0x23
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        push 0x23
        ; Push user esp
        mov eax, [ebp + 16]
        push eax
        mov eax, [ebp + 32]
        push eax
        ;pushfd
        push 0x1b
        ; Push eip
        mov eax, [ebp + 40]
        push eax
        ; Enter usermode from here(make sure the registers are restored correctly for the user process !)

        ; Load eax here
        mov eax, [ebp + 0]
        ; Now, restore ebp
        mov ebp, [ebp + 20]
        ; sti
        iret

