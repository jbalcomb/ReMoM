TITLE _s04p0.asm 

.MODEL LARGE, C

.CODE
;segment seg004 byte public 'CODE' use16
; 	 assume cs:seg004
; 	 ;org 0Ch
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_DISK_GetFreeSpace far
push es
push di
push ds
push si
mov dx, 0
mov ah, 36h
int 21h
cmp ax, 0FFFFh
loc_1421A:
jz short loc_14223
mul cx
mul bx
loc_14220:
jmp short loc_14229
nop
loc_14223:
mov dx, 0FFFFh
loc_14226:
mov ax, 0FFFFh
loc_14229:
pop si
pop ds
pop di
pop es
retf
endp UU_DISK_GetFreeSpace

;ends seg004

end
