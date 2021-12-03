; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    paging_asm.s                                       :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: lubenard <lubenard@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2021/05/26 15:23:32 by lubenard          #+#    #+#              ;
;    Updated: 2021/05/26 20:14:57 by lubenard         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

[GLOBAL enable_paging]
[GLOBAL flush_tlb]

flush_tlb:
	mov eax, cr3
	mov cr3, eax
	ret

enable_paging:
	mov eax, [esp+4]
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80010001
	mov cr0, eax
	ret
