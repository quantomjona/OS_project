
global setGdt
setGdt:
    mov eax , [esp+4]
    lgdt [eax]
    MOV eax, 0x10
    MOV ds, ax
    MOV es, ax
    MOV fs, ax
    MOV gs, ax
    MOV ss, ax
    JMP 0x08:.flush
.flush:
    RET

global flushTss
flushTss:
    mov ax,0x2B
    ltr ax
    ret
