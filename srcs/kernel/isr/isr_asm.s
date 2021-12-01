; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    isr_asm.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/05/02 16:43:24 by lubenard          #+#    #+#              ;
;    Updated: 2021/12/01 19:14:55 by lubenard         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

; This macro creates a stub for an ISR which does NOT pass it's own
; error code (adds a dummy errcode byte).
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts firstly.
    push byte 0                 ; Push a dummy error code.
    push byte %1                ; Push the interrupt number.
    jmp isr_common_stub         ; Go to our common handler code.
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts.
    push byte %1                ; Push the interrupt number
    jmp isr_common_stub
%endmacro

%macro IRQ 2                    ; We need 2 parameters for our macro
  global irq%1                  ; First declare the name (irq0, irq1 etc...) as global
  irq%1:                        ; Declare our function with the name (irq0, irq1 etc...)
    cli                         ; Disable interrupts
    push byte 0                 ; push 0 on the stack
    push byte %2                ; Push irqcode into the stack (second parameter)
    jmp irq_common_stub         ; call irq_common_stub function
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_ERRCODE 29
ISR_ERRCODE 30
ISR_NOERRCODE 31
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10,42
IRQ 11,43
IRQ 12,44
IRQ 13,45
IRQ 14,46
IRQ 15,47

; Import handlers from isr.c
extern isr_handler
extern irq_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, isr_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

; This is a stub that we have created for IRQ based ISRs. This calls
; 'irq_handler' in our C code.
irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp                 ; At this point ESP is a pointer to where GS (and the rest
                             ; of the interrupt handler state resides)
                             ; Push ESP as 1st parameter as it's a 
                             ; pointer to a registers_t  
    call irq_handler
    pop ebx                  ; Remove the saved ESP on the stack. Efficient to just pop it 
                             ; into any register. You could have done: add esp, 4 as well
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret
