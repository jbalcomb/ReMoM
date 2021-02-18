

; ASM Include Template
; EQU
; <enum>
; MACRO ... ENDM
; STRUC





;       Segments Definitions

Header@

;       External References

extrn           __cexit:DIST
extrn           __IOERROR:near
extrn           __envseg:word

ExtSym@         DGROUP@, WORD, __PASCAL__
ExtSym@         _psp, WORD, __CDECL__
ExtSym@         _version, WORD, __CDECL__

        ;* Miscellaneous equates */

ExeSignature    equ     05A4Dh

        ;/* Data for the Loader */

LdDesc          STRUC
LdErrorMsg      db      'Exec failure.',00Dh,00Ah
LdStack         db      80H dup(?)      ;Loader stack
LdPSP           dw      ?               ;PSP address
LdPathName      db      80 dup(?)       ;File to be loaded
LdAX            dw      ?               ;Parse file name results
LdExeSignature  dw      ?               ;EXE header buffer
LdLength        dw      ?
LdNbPages       dw      ?
LdNbItems       dw      ?
LdHdrSize       dw      ?
LdMin           dw      ?
LdMax           dw      ?
LdSS            dw      ?
LdSP            dw      ?
LdCheckSum      dw      ?
LdIP            dw      ?
LdCS            dw      ?
LdLoadAddr      dw      ?               ;Load Overlay interface block
LdRelocFactor   dw      ?               ;Relocation factor to be used
LdDesc          ENDS

        SUBTTL  Loader program
        PAGE
;/*                                                      */
;/*------------------------------------------------------*/
;/*                                                      */
;/*     Loader Program                                   */
;/*     --------------                                   */
;/*                                                      */
;/*------------------------------------------------------*/
;/*                                                      */
CSeg@
LoaderDatas     db      size LdDesc dup (0)
_Loader         PROC    NEAR            ;CX = EnvSeg, DX = End of Memory

