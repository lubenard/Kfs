LoadGDT:
	lgdt [edi]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	pop edi
	mov eax, 0x08
	push eax
	push edi
	ret
GLOBAL LoadGDT
