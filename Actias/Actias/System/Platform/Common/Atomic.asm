section .text
bits 64

global ActiasAtomicAddI8@@16, ActiasAtomicAddI16@@16, ActiasAtomicAddI32@@16, ActiasAtomicAddI64@@16

export ActiasAtomicAddI8@@16
ActiasAtomicAddI8@@16:
    mov     eax,  DWORD edx
    lock    xadd  BYTE [rcx], al
    ret

export ActiasAtomicAddI16@@16
ActiasAtomicAddI16@@16:
    mov     eax,  DWORD edx
    lock    xadd  WORD [rcx], ax
    ret

export ActiasAtomicAddI32@@16
ActiasAtomicAddI32@@16:
    mov     eax,  DWORD edx
    lock    xadd  DWORD [rcx], eax
    ret

export ActiasAtomicAddI64@@16
ActiasAtomicAddI64@@16:
    mov     rax,  QWORD rdx
    lock    xadd  QWORD [rcx], rax
    ret
