TITLE _o60p02.asm GAME_PrepareCredits
; MGC_DEF.H

.MODEL LARGE, C

.CODE
;segment ovr060 para public 'OVERLAY' use16
;	 assume cs:ovr060
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GAME_PrepareCredits

proc GAME_PrepareCredits far

push bp
mov bp, sp
mov [GAME_FirstShownCred], 0
mov [GAME_CreditTextTop], 95
mov ax, 3125
push ax
push [LBX_Sandbox_Segment]
call LBX_Alloc_First
pop cx
pop cx
mov [GUI_Credits_Text_IMG], ax
push [GUI_Credits_Text_IMG]
mov ax, 122
push ax
mov ax, 280
push ax
call LBX_Image_Prepare
add sp, 6
pop bp
retf

endp GAME_PrepareCredits

;ends ovr060

end
