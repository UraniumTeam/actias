%include "Actias/System/Platform/Common/ActiasMacros.mac"
section .text
bits 64

%define YWORD_AT(i) (i * 32)

ACTIAS_DLL_EXPORT(ActiasCopyAlignedMemory@@24)
ActiasCopyAlignedMemory@@24:
    cmp     r8, 256
    jb      .end
    xor     eax, eax

.loop:
    vmovups ymm0, YWORD [rdx + rax + YWORD_AT(0)]
    vmovups ymm1, YWORD [rdx + rax + YWORD_AT(1)]
    vmovups ymm2, YWORD [rdx + rax + YWORD_AT(2)]
    vmovups ymm3, YWORD [rdx + rax + YWORD_AT(3)]
    vmovups ymm4, YWORD [rdx + rax + YWORD_AT(4)]
    vmovups ymm5, YWORD [rdx + rax + YWORD_AT(5)]
    vmovups ymm6, YWORD [rdx + rax + YWORD_AT(6)]
    vmovups ymm7, YWORD [rdx + rax + YWORD_AT(7)]

    vmovups YWORD [rcx + rax + YWORD_AT(0)], ymm0
    vmovups YWORD [rcx + rax + YWORD_AT(1)], ymm1
    vmovups YWORD [rcx + rax + YWORD_AT(2)], ymm2
    vmovups YWORD [rcx + rax + YWORD_AT(3)], ymm3
    vmovups YWORD [rcx + rax + YWORD_AT(4)], ymm4
    vmovups YWORD [rcx + rax + YWORD_AT(5)], ymm5
    vmovups YWORD [rcx + rax + YWORD_AT(6)], ymm6
    vmovups YWORD [rcx + rax + YWORD_AT(7)], ymm7

    add     r8,   -256
    add     rax,   256
    cmp     r8,    255
    ja      .loop

    vzeroupper

.end:
    ret

ACTIAS_DLL_EXPORT(ActiasStreamMemory@@24)
ActiasStreamMemory@@24:
    cmp     r8, 256
    jb      .end
    xor     eax, eax

.loop:
    vmovups ymm0, YWORD [rdx + rax + YWORD_AT(0)]
    vmovups ymm1, YWORD [rdx + rax + YWORD_AT(1)]
    vmovups ymm2, YWORD [rdx + rax + YWORD_AT(2)]
    vmovups ymm3, YWORD [rdx + rax + YWORD_AT(3)]
    vmovups ymm4, YWORD [rdx + rax + YWORD_AT(4)]
    vmovups ymm5, YWORD [rdx + rax + YWORD_AT(5)]
    vmovups ymm6, YWORD [rdx + rax + YWORD_AT(6)]
    vmovups ymm7, YWORD [rdx + rax + YWORD_AT(7)]

    vmovntps YWORD [rcx + rax + YWORD_AT(0)], ymm0
    vmovntps YWORD [rcx + rax + YWORD_AT(1)], ymm1
    vmovntps YWORD [rcx + rax + YWORD_AT(2)], ymm2
    vmovntps YWORD [rcx + rax + YWORD_AT(3)], ymm3
    vmovntps YWORD [rcx + rax + YWORD_AT(4)], ymm4
    vmovntps YWORD [rcx + rax + YWORD_AT(5)], ymm5
    vmovntps YWORD [rcx + rax + YWORD_AT(6)], ymm6
    vmovntps YWORD [rcx + rax + YWORD_AT(7)], ymm7

    add     r8,   -256
    add     rax,   256
    cmp     r8,    255
    ja      .loop

    vzeroupper

.end:
    ret

