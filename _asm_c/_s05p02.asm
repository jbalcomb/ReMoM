TITLE _s05p02.asm GAME_QuitProgram

.MODEL LARGE, C

segment dseg para public 'DATA' use16
dbg_VideoModeText        DB 'DEBUG: Video Mode 03h',0Dh,0Ah,'$',0
ends dseg

EXTRN VGA_SetTextMode:PROC ; _s06p01c.c
EXTRN DOS_QuitWithMsg:PROC ; _s06p02.asm

;segment seg005 byte public 'CODE' use16
;    assume cs:seg005
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing
.CODE

PUBLIC GAME_QuitProgram

proc GAME_QuitProgram far

    argQuitMessage = word ptr 6

    push bp
    mov  bp, sp
    
;    call SND_Stop_Music

;    call MOUSE_Reset

;    call SND_Shutdown

    ;call _EMM_ReleaseAll

;    call CRP_Empty_Exit_Fn3

    call VGA_SetTextMode
    ;mov  ax, 03h
    ;int  10h
    
    push [bp+argQuitMessage]
    call DOS_QuitWithMsg
    add sp, 02h                         ;pop cx

    pop  bp
    ret

endp GAME_QuitProgram

;ends seg005

end
