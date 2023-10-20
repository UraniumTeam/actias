section .text
bits 64

global ActiasAtomicAddI8, ActiasAtomicAddI16, ActiasAtomicAddI32, ActiasAtomicAddI64

export ActiasAtomicAddI8
ActiasAtomicAddI8:
    mov     eax,  DWORD edx
    lock    xadd  BYTE [rcx], al
    ret

export ActiasAtomicAddI16
ActiasAtomicAddI16:
    mov     eax,  DWORD edx
    lock    xadd  WORD [rcx], ax
    ret

export ActiasAtomicAddI32
ActiasAtomicAddI32:
    mov     eax,  DWORD edx
    lock    xadd  DWORD [rcx], eax
    ret

export ActiasAtomicAddI64
ActiasAtomicAddI64:
    mov     rax,  QWORD rdx
    lock    xadd  QWORD [rcx], rax
    ret
