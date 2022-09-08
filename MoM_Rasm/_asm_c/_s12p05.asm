TITLE  _s12p05.asm EMM_MakeNamedHandle

.MODEL LARGE, C

;segment seg012 byte public 'CODE' use16
.CODE
    ;assume cs:seg012
    ;org 6
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_MakeNamedHandle

; attempts to allocate the indicated amount of pages
; to an EMM handle, and set its name
; returns the handle if successful, 0 otherwise
proc EMM_MakeNamedHandle far
;;;computeControlDigit PROC FAR C USES DS SI DI barcodestr:DWORD
;;;EMS_MakeNamedHandle PROC FAR C USES DS SI DI argPageCount:WORD, argHandleName:FAR PTR BYTE
;;;handle:WORD, ptr_handle_name:FAR PTR BYTE                           
;;;get_handle_name    PROC                            \
;;;                   USES DI,                        \
;;;                   ptr_handle_name:FAR PTR BYTE,   \
;;;                   handle:WORD
;;PROC EMS_MakeNamedHandle FAR
;;    ARG argPageCount:WORD,argHandleName:FAR PTR BYTE

    varEmmHandle = word ptr -2
    argPageCount = word ptr 6
    ;;argHandleName = word ptr 8
    ;;argHandleName = dword ptr 8
    argHandleName = dword ptr 8

    push bp
    mov  bp, sp
    sub  sp, 2
    push es
    push ds
    push si
    push di

    mov ah, 43h
    mov bx, [bp+argPageCount]
    ;mov bx, [argPageCount]
    int 67h
    cmp ah, 0
    jnz @@error
    
    mov [bp+varEmmHandle], dx

    ;;;;;;mov si, [bp+argHandleName]
    ;;;;lds si, dword ptr [bp+argHandleName]  ; load far pointer to source string
    ;;;;;mov si, word ptr [bp+argHandleName+2]
    ;;;;mov bx, word ptr [bp+argHandleName]
    ;;;;mov ds, bx
    ;;;;;mov si, word ptr [bp+argHandleName+2]
    ;;;;mov si, 8
    ;;mov bx, sp
    ;;mov ds, bx
    ;;mov si, 8
    ;lea ax, [bp+argHandleName]
    ;mov ds, ax
    ;mov si, 0

    ; TODO(JimBalcomb): add code to branch around DOSBox bug - if running in DOSBOX...
    ;lds si, [bp+argHandleName]
    les di, [bp+argHandleName]  ; BUG: DOSBox uses the wrong segment and index registers (DOSBox, DOSBox-X, DOSBox-Staging)

    ;lds si, argHandleName
    ;;mov dx, si
    ;;mov ah, 09h
    ;;int 21h
    mov ax, 5301h
    mov dx, [bp+varEmmHandle]
    int 67h
    cmp ah, 0
    jnz @@error

    mov ax, [bp+varEmmHandle]

    pop  di
    pop  si
    pop  ds
    pop  es
    mov  sp, bp
    pop  bp
    ret

@@error:
    ;mov ax, 0

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp EMM_MakeNamedHandle

;ends seg012

end
