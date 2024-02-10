
[GLOBAL check_cpu_id]
[GLOBAL check_cpu_vendor]
[GLOBAL get_cpu_features]

extern printd
extern display_cpu_vendor
extern display_cpu_features

cpuid_not_supported db 'CPUID is not supported', 0
cpuid_supported db 'CPUID is supported', 0

check_cpu_id:
    push ebp                             ; Stack & Frame Setup and Save
    mov  ebp, esp

    pushfd                               ; Save EFLAGS
    pushfd                               ; Store EFLAGS
    xor dword [esp],0x00200000           ; Invert the ID bit in stored EFLAGS
    popfd                                ; Load stored EFLAGS (with ID bit inverted)
    pushfd                               ; Store EFLAGS again (ID bit may or may not be inverted)
    pop eax                              ; eax = modified EFLAGS (ID bit may or may not be inverted)
    xor eax,[esp]                        ; eax = whichever bits were changed
    popfd                                ; Restore original EFLAGS
    and eax, 0x00200000                  ; eax = zero if ID bit can't be changed, else non-zero
    cmp eax, 0                           ; Compare eax to 0
    je id_not_supported                  ; if equal, jump to id_not_supported
    jmp id_supported                     ; Else, jump to id_supported
    ret

id_not_supported:
    push 0x0                             ; Push flags
    push cpuid_not_supported             ; Push String
    push 0x3                             ; Push Info Type
    call printd                          ; Call printf
    mov esp, ebp                         ; Stack & Frame Restore
    pop ebp
    ret

id_supported:
    push 0x0                            ; Push flags
    push cpuid_supported                ; Push String
    push 0x0                            ; Push Info Type
    call printd                         ; Call printf
    mov esp, ebp                        ; Stack & Frame Restore
    pop ebp
    ret

check_cpu_vendor:
    push ebp                            ; Stack & Frame Setup and Save
    mov  ebp, esp

    mov eax, 0x0                        ; Get CPU Vendor ID String
    cpuid                               ; Get Infos
    push ecx                            ; Infos are returned in ebx, edx, ecx
    push edx                            ; We push thoses on the stack to pass them to C function
    push ebx
    call display_cpu_vendor             ; Call our C function

    mov esp, ebp                        ; Stack & Frame Restore
    pop ebp
    ret

get_cpu_features:
    push ebp                            ; Stack & Frame Setup and Save
    mov  ebp, esp

    mov eax, 0x1                        ; Get CPU Vendor ID String
    cpuid
    push ecx                            ; Infos are returned in edx, ecx
    push edx                            ; We push thoses on the stack to pass them to C function
    call display_cpu_features

    mov esp, ebp                        ; Stack & Frame Restore
    pop ebp
    ret