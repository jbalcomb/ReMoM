TITLE _s34p76.asm GUI_SetRedrawFn
; ST_GUI.H

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SetRedrawFn

proc GUI_SetRedrawFn

    Refresh_Fn = dword ptr 6
    Refresh_Timer = word ptr 0Ah

    push bp
    mov  bp, sp

    mov bx, [bp+Refresh_Timer]
    mov ax, [word ptr bp+Refresh_Fn+2]
    mov dx, [word ptr bp+Refresh_Fn]
    mov [word ptr GUI_Redraw_Function+2], ax
    mov [word ptr GUI_Redraw_Function], dx
    mov [GUI_RedrawFn_Present], 1
    or bx, bx
    jle short loc_2E223
    cmp bx, 0Ah
    jge short loc_2E223
    mov [GUI_Redraw_Timer], bx
    jmp short @@Done

loc_2E223:
    mov [GUI_Redraw_Timer], 2

@@Done:
    pop bp
    ret

endp GUI_SetRedrawFn

;ends seg034

end
