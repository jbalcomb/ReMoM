TITLE _s10p16.asm ExtractFileBase (drake178: StripFileExt)

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;asssume es:nothing, ss:nothing, ds:dseg

PUBLIC ExtractFileBase

proc ExtractFileBase far

    LbxFileName = word ptr 6

    push bp
    mov  bp, sp

    mov cx, [bp+LbxFileName]
    xor dx, dx
    jmp short @@WHILE_NOT_ZERO

@@NOT_ZERO:
    mov bx, cx
    add bx, dx

    cmp byte ptr [bx], 60h ; If Below ASCII Value 61h ("a"), Then NOT_LOWERCASE
    jle short @@NOT_LOWERCASE

    mov bx, cx
    add bx, dx
    mov al, [bx]
    add al, 'A' - 'a'
    mov bx, cx
    add bx, dx
    mov [bx], al

@@NOT_LOWERCASE:
    mov bx, cx
    add bx, dx
    cmp byte ptr [bx], '.'
    jnz short @@NEXT_CHARACTER

    mov bx, cx
    add bx, dx
    mov byte ptr [bx], 0

@@NEXT_CHARACTER:
    inc dx

@@WHILE_NOT_ZERO:
    mov bx, cx

loc_16069:
    add bx, dx
    cmp byte ptr [bx], 0
    jnz short @@NOT_ZERO

@@STRING_END:
    pop bp
    ret

endp ExtractFileBase

;ends seg010

end
