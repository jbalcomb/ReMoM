TITLE _s13p16.asm EMS_Shutdown
;drake187: "MM_ReleaseAll"

.MODEL LARGE

segment dseg para public 'DATA' use16
ends dseg


segment seg013 byte public 'CODE' use16
    assume cs:seg013
    ;org 7
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC C EMS_Shutdown

; releases all open EMM handles and associated memory,
; including the handle of the overlay manager, which
; would normally be released by the exit functions
proc EMS_Shutdown far

    push bp
    mov bp, sp
    push si
    xor si, si
    jmp short loc_17622
loc_1760E:
    mov ax, si
    mov dx, 0Ch
    imul dx
    mov bx, ax
    push [EMM_Table.Handle+bx]
    call EMS_ReleaseHandle
    pop cx
    inc si
loc_17622:
    cmp si, [EMS_Open_Handles]
    jl short loc_1760E
    cmp [EMS_Overlay_Handle], 0
    jz short loc_17639
    push [EMS_Overlay_Handle]
    call EMS_ReleaseHandle
    pop cx
loc_17639:
    pop si
    pop bp
    ret

endp EMS_Shutdown

ends seg013

end
