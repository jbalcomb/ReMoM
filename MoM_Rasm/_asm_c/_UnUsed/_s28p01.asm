TITLE _s28p01.asm 
; .H

.MODEL LARGE, C

.CODE
;segment seg028 byte public 'CODE' use16
;	 assume cs:seg028
;	 ;org 1
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_LBX_Image_Copy far
Total_Size= word ptr -2
Dst_Seg= word ptr 6
Src_Seg= word ptr 8
push bp
mov bp, sp
loc_1EE74:
sub sp, 2
push si
mov si, [bp+Src_Seg]
xor ax, ax
push ax
push si
call PEEK_Word
pop cx
pop cx
push ax
mov ax, LBX_IMG_HDR.Height
push ax
push si
call PEEK_Word
pop cx
pop cx
mov dx, ax
pop ax
imul dx
add ax, 10h
mov [bp+Total_Size], ax
push [bp+Total_Size]
push si
xor ax, ax
push ax
push [bp+Dst_Seg]
xor ax, ax
push ax
call MEM_Copy_Far
add sp, 0Ah
pop si
mov sp, bp
pop bp
retf
endp UU_LBX_Image_Copy
