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
    push %1                     ; Push the interrupt number.
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
extern syscalls_handler

global syscalls

isr_common_stub:
   pusha                ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

   mov ax, ds           ; Lower 16-bits of eax = ds.
   push eax             ; save the data segment descriptor

   mov ax, 0x10         ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call isr_handler     ; call C function isr_handler

   pop eax              ; reload the original data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   popa                 ; Pops edi,esi,ebp...
   add esp, 8           ; Cleans up the pushed error code and pushed ISR number
   sti
   iret                 ; pops 5 things at once: CS, EIP, EFLAGS, SS, and esp


; This is a stub that we have created for IRQ based ISRs. This calls
; 'irq_handler' in our C code.
irq_common_stub:
   pusha                ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

   mov ax, ds           ; Lower 16-bits of eax = ds.
   push eax             ; save the data segment descriptor

   mov ax, 0x10         ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call irq_handler     ; Call handler

                        ; After call of the handler function, we want to
                        ; restore the state of registers
   pop ebx              ; reload the original data segment descriptor
   mov ds, bx
   mov es, bx
   mov fs, bx
   mov gs, bx

   popa                 ; Pops edi,esi,ebp... (Remove them from the stack)
   add esp, 8           ; Cleans up the pushed error code and pushed ISR number
   sti
   iret                 ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; This is a stub to handle sycalls.
; This call syscalls_handler in our C code
syscalls:
   pusha                ; Pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

   mov ax, ds           ; Lower 16-bits of eax = ds.
   push eax             ; save the data segment descriptor

   mov ax, 0x10         ; load the kernel data segment descriptor
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   call syscalls_handler     ; Call handler

                        ; After call of the handler function, we want to
                        ; restore the state of registers
   pop ebx              ; reload the original data segment descriptor
   mov ds, bx
   mov es, bx
   mov fs, bx
   mov gs, bx

   popa                 ; Pops edi,esi,ebp... (Remove them from the stack)
   add esp, 8           ; Cleans up the pushed error code and pushed ISR number
   sti
   iret                 ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP