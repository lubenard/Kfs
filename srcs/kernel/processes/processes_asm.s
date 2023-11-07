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

    ; Load eflags
    push dword [ebp + 32] ; Push new eflags on the (old ?) stack
    popfd ; Pop last value on stack & Load it into eflags

    ; Right now, eax, ebp, esp are not restored yet

    ; Enter usermode from here(make sure the registers are restored correctly for the user process !)
        mov ax, 0x23 ; ????
        mov ds, ax   ; ????
        mov es, ax   ; ????
        mov fs, ax   ; ????
        mov gs, ax   ; ????

        push 0x23    ; ????
    ; Push user esp on (new ?) stack
        push dword [eax + 16] ; esp
    ; Push user eflags on (new ?) stack
        push dword [ebp + 32] ; eflags
    ;    pushfd ; push all eflags on the stack
        push 0x1b    ; ????
    ; Push eip on (new ?) stack
        push dword [ebp + 40] ; eip

    ; Load eax here
    mov eax, [eax + 0]
    ; Now, restore ebp
    mov ebp, [eax + 20]
    ; sti
    iret

