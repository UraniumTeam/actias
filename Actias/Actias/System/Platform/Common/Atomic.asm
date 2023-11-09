%include "Actias/System/Platform/Common/ActiasMacros.mac"
section .text
bits 64

ACTIAS_DLL_EXPORT(ActiasAtomicAddI8)
global ActiasAtomicAddI8
ActiasAtomicAddI8:
    mov     eax,  DWORD edx
    lock    xadd  BYTE [rcx], al
    ret

ACTIAS_DLL_EXPORT(ActiasAtomicAddI16)
global ActiasAtomicAddI16
ActiasAtomicAddI16:
    mov     eax,  DWORD edx
    lock    xadd  WORD [rcx], ax
    ret

ACTIAS_DLL_EXPORT(ActiasAtomicAddI32)
global ActiasAtomicAddI32
ActiasAtomicAddI32:
    mov     eax,  DWORD edx
    lock    xadd  DWORD [rcx], eax
    ret

ACTIAS_DLL_EXPORT(ActiasAtomicAddI64)
global ActiasAtomicAddI64
ActiasAtomicAddI64:
    mov     rax,  QWORD rdx
    lock    xadd  QWORD [rcx], rax
    ret
