; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    idt_asm.s                                          :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/05/26 20:16:42 by lubenard          #+#    #+#              ;
;    Updated: 2021/05/26 20:16:56 by lubenard         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

[GLOBAL idt_flush]    ; Allows the C code to call idt_flush().

idt_flush:
	mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter.
	lidt [eax]        ; Load the IDT pointer.
	sti               ; Enable interruptions
	ret
