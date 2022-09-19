; _s24p0.asm 

.MODEL LARGE, C

.CODE

;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc DBG_Quit far
push    bp
mov    bp, sp

loc_1DB45:
cmp    [DBG_Disabled],    1
jnz    short loc_1DB50

xor    ax, ax
jmp    short loc_1DB55

; ---------------------------------------------------------------------------

loc_1DB50:
call    GAME_QuitWithReport

; ---------------------------------------------------------------------------

loc_1DB55:
pop    bp
retf

endp DBG_Quit


;ends seg024

end
