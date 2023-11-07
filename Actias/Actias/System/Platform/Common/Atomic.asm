%include "Actias/System/Platform/Common/ActiasMacros.mac"
section .text
bits 64

ACTIAS_DLL_EXPORT(ActiasAtomicAddI8@@16)
ActiasAtomicAddI8@@16:
    mov     eax,  DWORD edx
    lock    xadd  BYTE [rcx], al
    ret

ACTIAS_DLL_EXPORT(ActiasAtomicAddI16@@16)
ActiasAtomicAddI16@@16:
    mov     eax,  DWORD edx
    lock    xadd  WORD [rcx], ax
    ret

ACTIAS_DLL_EXPORT(ActiasAtomicAddI32@@16)
ActiasAtomicAddI32@@16:
    mov     eax,  DWORD edx
    lock    xadd  DWORD [rcx], eax
    ret

ACTIAS_DLL_EXPORT(ActiasAtomicAddI64@@16)
ActiasAtomicAddI64@@16:
    mov     rax,  QWORD rdx
    lock    xadd  QWORD [rcx], rax
    ret
