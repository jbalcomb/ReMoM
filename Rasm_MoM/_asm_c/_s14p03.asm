TITLE   _s14p03.asm VGA_DAC_Init
;F:\Development_OPC\momrtgt-code\MoMModel\MoMTemplate.h
;J:\STU\DBWD\developc\1oom-master\src\lbxfont.c

.MODEL LARGE, C

EXTRN strcpy:PROC
EXTRN LBXE_LoadSingle:PROC ; _s10p01.asm


;segment dseg para public 'DATA' use16
.DATA
dbgExec_VGA_DAC_Init    DB 'DEBUG: Executing _VGA_DAC_Init',0Dh,0Ah,'$',0
;dseg:A7E0 00 00 00 00 00 00 00 00 00 00+    Font_FileName db 10h dup(   0)          ; set in VGA_DAC_Init
Font_FileName db 10h dup(0)
;dseg:A816                                   ; unsigned int Font_Data_Seg
;dseg:A816 00 00                             Font_Data_Seg@ dw 0                     ; single-loaded LBX entry set up in VGA_DAC_Init
;dseg:A816                                                                           ; there is no hard image header, data is loaded to the
;dseg:A816                                                                           ; beginning of the segment from the palette data to
;dseg:A816                                                                           ; form a Font_Header structure for the current font
Font_Data_Seg dw 0
;dseg:A818 00 00                             Border_Data_Seg@ dw 0
Border_Data_Seg dw 0
;dseg:A7F4                                   ; int LBX_Palette_Seg
;dseg:A7F4 00 00                             LBX_Palette_Seg@ dw 0                   ; 15Ch LBX_Alloc_Space paragraphs
LBX_Palette_Seg dw 0
;dseg:A7DE                                   ; unsigned int VGA_DAC_Segment
;dseg:A7DE 00 00                             VGA_DAC_Segment dw 0                    ; 40h LBX_Alloc_Space paragraphs
VGA_DAC_Segment	dw 0
;dseg:A7D6                                   ; unsigned int VGA_DACmod_Segment
;dseg:A7D6 00 00                             VGA_DACmod_Segment@ dw 0                ; 30h paragraphs into the VGA_DAC_Segment allocation
VGA_DACmod_Segment dw 0
;dseg:A7DC 00 00                             UU_DAC_Save_Seg dw 0                    ; 30h LBX_Alloc_Space paragraphs
UU_DAC_Save_Seg	dw 0
;dseg:A7DA 00 00                             Replacement_Colors@ dw 0                ; 180h LBX_Alloc_Space paragraphs
;dseg:A7DA                                                                           ; each 100h byte block represents a different entry
;dseg:A7DA                                                                           ;
;dseg:A7DA                                                                           ; within a block, every byte contains a color index
;dseg:A7DA                                                                           ; closest to the combination of the original color
;dseg:A7DA                                                                           ; at that index, and the color and percent specified
;dseg:A7DA                                                                           ; from the palette image when the block is filled out
;dseg:A7DA                                                                           ;
;dseg:A7DA                                                                           ; VGA_DrawBitmap_R and other _R functions use these to
;dseg:A7DA                                                                           ; replace bytes read from a source image file
Replacement_Colors dw 0
;dseg:A7D4                                   ; unsigned int VGA_FILEH_Hdr
;dseg:A7D4 00 00                             VGA_FILEH_Hdr dw 0                      ; 2 LBX_Alloc_Space paragraphs
;dseg:A7D4                                                                           ; stores an EMM image header for the VGAFILEH contents
VGA_FILEH_Hdr dw 0
;dseg:A7F0 00 00                             Intensity_Scale_Tbl@ dw 0               ; 60h LBX_Alloc_Space paragraphs (should be only 10h)
;dseg:A7F0                                                                           ; used by grayscaling functions to store the altered
;dseg:A7F0                                                                           ; version of the palette (color indexes only)
Intensity_Scale_Tbl dw 0
;ends dseg

segment seg014 byte public 'CODE' use16
    assume cs:seg014
    ;org 0Dh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DAC_Init

proc VGA_DAC_Init far

    Font_File_Name = word ptr  6

    push bp
    mov  bp, sp

    push si
    push di
    
    mov  di, [bp+Font_File_Name]
    push di
    mov  ax, offset Font_FileName
    push ax
    call _strcpy
    add sp, 04h                         ;pop cx, pop cx

    ;mov  ax, 0                          ;xor ax, ax
    ;push ax
    ;push di
    ;call _LBXE_LoadSingle
    ;add sp, 04h                         ;pop cx, pop cx
;   ; loads an LBX entry into a single allocation
                                                              ; returns a segment pointer to the data
                                                              ; quits on failure

;    mov [Font_Data_Seg], ax
;
;    mov ax, 1
;    push ax
;    push di
;    call LBXE_LoadSingle
;    add sp, 04h                         ;pop cx, pop cx
;
;    mov [Border_Data_Seg], ax
;
;    mov ax, 15Ch
;    push ax
;    call LBX_Alloc_Space
;    add sp, 02h                         ;pop cx
    ; creates a far LBX space allocation with a custom header,
    ;   the segment of which is the return value
    ; this can then be filled with multiple suballocations
    ; quits on errors
;
;    mov [LBX_Palette_Seg], ax
;
;    mov ax, 40h
;    push ax
;    call LBX_Alloc_Space
;    add sp, 02h                         ;pop cx
;
;    mov [VGA_DAC_Segment], ax
;
;    mov ax, [VGA_DAC_Segment]
;    add ax, 30h
;    mov [VGA_DACmod_Segment], ax
;
;    mov ax, 30h
;    push ax
;    call LBX_Alloc_Space
;    add sp, 02h                         ;pop cx
;
;    mov [UU_DAC_Save_Seg], ax
;
;    mov ax, 180h
;    push ax
;    call LBX_Alloc_Space
;    add sp, 02h                         ;pop cx
;
;    mov [Replacement_Colors], ax
;
;    mov ax, 2
;    push ax
;    call LBX_Alloc_Space
;    add sp, 02h                         ;pop cx
;
;    mov [VGA_FILEH_Hdr], ax
;
;    mov ax, 60h
;    push ax
;    call LBX_Alloc_Space
;    add sp, 02h                         ;pop cx
;
;    mov [Intensity_Scale_Tbl@], ax
;
;    call VGA_TextDraw_Init
    ; allocates the memory necessary for setting up the display of floating text paragraphs (multi-line strings)
;
;    xor si, si
;    jmp short loc_17F06
;
;loc_17EF5:
;    xor ax, ax
;    push ax
;    push si
;    push [VGA_DAC_Segment]
;    call POKE_Byte
;    add sp, 6
;
;    inc si
;
;loc_17F06:
;    cmp si, 300h ; 768
;    jl short loc_17EF5
;    xor si, si
;    jmp short loc_17F22
;
;loc_17F10:
;    mov ax, 1
;    push ax
;    push si
;    push [VGA_DACmod_Segment@]
;    call POKE_Byte
;    add sp, 6
;    inc si
;
;loc_17F22:
;    cmp si, 100h ; 256
;    jl short loc_17F10

    pop di
    pop si
    
    pop bp

    ret

endp _VGA_DAC_Init

ends seg014

end
