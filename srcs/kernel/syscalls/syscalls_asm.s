[GLOBAL test_syscalls]    ; Allows the C code to call idt_flush().

test_syscalls:
	mov eax, 1
	int 0x80