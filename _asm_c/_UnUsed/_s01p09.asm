TITLE _s01p0.asm 

.MODEL LARGE, C

.CODE
; Segment type: Pure code
;segment seg001 byte public 'CODE' use16
;    assume cs:seg001
;    org 9
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_HLP_Load_MainMenu far
push bp
mov bp, sp
mov ax, 0Ah
push ax
mov ax, 4
push ax
xor ax, ax
push ax
mov ax, offset HLP_Entry_Table
push ax
mov ax, 25
push ax
mov ax, offset cnst_HLPENTRY_File
push ax
call LBXR_DirectLoader
add sp, 0Ch
mov ax, 4
push ax
mov ax, offset HLP_Entry_Table
push ax
loc_13F18:
call GUI_SetHelp
pop cx
pop cx
pop bp
locret_13F20:
retf
endp UU_HLP_Load_MainMenu

;ends seg001

end
