; _s24p0.asm 

.MODEL LARGE, C

.CODE

;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC 


proc DBG_ScreenDump far

Conversion_String= byte    ptr -186h
ScreenDump_File= byte ptr -176h
Buffer=    byte ptr -166h
Scrdmp_File_Number= word ptr -8
Word_Field_Value= word ptr -6
Dword_Field_Value= dword ptr -4

push    bp
mov    bp, sp
sub    sp, 186h
push    si
push    di
cmp    [DBG_Disabled],    1
jnz    short loc_1DB6C

xor    ax, ax
jmp    loc_1DF54

; ---------------------------------------------------------------------------

loc_1DB6C:
call    VGA_SetDirectDraw

call    MOUSE_Disable_CDraw

mov    [bp+Scrdmp_File_Number], 0
mov    ax, offset cnst_Scrdmp00_Full
push    ax
lea    ax, [bp+ScreenDump_File]
push    ax
call    strcpy

jmp    loc_1DC0F

; ---------------------------------------------------------------------------

loc_1DB8C:
inc    [bp+Scrdmp_File_Number]
cmp    [bp+Scrdmp_File_Number], 0Ah
jnb    short loc_1DBCC

mov    ax, offset cnst_Scrdmp_0
push    ax
lea    ax, [bp+Buffer]
push    ax
call    strcpy

pop    cx
pop    cx
mov    ax, 0Ah
push    ax
lea    ax, [bp+Conversion_String]
push    ax
push    [bp+Scrdmp_File_Number]
call    itoa

add    sp, 6
lea    ax, [bp+Conversion_String]
push    ax
lea    ax, [bp+Buffer]
push    ax
call    strcat

pop    cx
pop    cx
jmp    short loc_1DBE0

; ---------------------------------------------------------------------------

loc_1DBCC:
mov    ax, 0Ah
push    ax
lea    ax, [bp+Buffer]
push    ax
push    [bp+Scrdmp_File_Number]
call    itoa

add    sp, 6


loc_1DBE0:
mov    ax, offset cnst_Scrdmp_Base
push    ax
lea    ax, [bp+ScreenDump_File]
push    ax
call    strcpy

pop    cx
pop    cx
lea    ax, [bp+Buffer]
push    ax
lea    ax, [bp+ScreenDump_File]
push    ax
call    strcat

pop    cx
pop    cx
mov    ax, offset cnst_Scrdmp_Ext
push    ax
lea    ax, [bp+ScreenDump_File]
push    ax
call    strcat


loc_1DC0F:
pop    cx
pop    cx
lea    ax, [bp+Buffer]
push    ax
lea    ax, [bp+ScreenDump_File]
push    ax
call    DISK_FileFind

pop    cx
pop    cx
or    ax, ax
jz    short loc_1DC29

jmp    loc_1DB8C

; ---------------------------------------------------------------------------

loc_1DC29:
mov    ax, offset cnst_WB6
push    ax
lea    ax, [bp+ScreenDump_File]
push    ax
call    fopen

pop    cx
pop    cx
mov    si, ax
mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 0FDB0h
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0AF11h
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 1
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 320
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 200
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 8
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 5
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0
xor    di, di
jmp    short loc_1DD2F

; ---------------------------------------------------------------------------

loc_1DD19:
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
inc    di


loc_1DD2F:
cmp    di, 37h
jb    short loc_1DD19

mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 0FD20h
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0F1FAh
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 2
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 0
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 30Ah
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0Bh
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 1
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
xor    di, di
jmp    short loc_1DE50

; ---------------------------------------------------------------------------

loc_1DE2C:
lea    ax, [bp+Buffer]
push    ax
push    di
nop
push    cs
call    near ptr VGA_GetDACBlock

pop    cx
pop    cx
push    si
mov    ax, 1
push    ax
mov    ax, 0C0h
push    ax
lea    ax, [bp+Buffer]
push    ax
call    fwrite

add    sp, 8
inc    di


loc_1DE50:
cmp    di, 4
jb    short loc_1DE2C

mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 0FA06h
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 10h
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
xor    di, di
jmp    short loc_1DEB6

; ---------------------------------------------------------------------------

loc_1DE92:
lea    ax, [bp+Buffer]
push    ax
push    di
nop
push    cs
call    near ptr VGA_ReadScreenLine

pop    cx
pop    cx
push    si
mov    ax, 1
push    ax
mov    ax, 320
push    ax
lea    ax, [bp+Buffer]
push    ax
call    fwrite

add    sp, 8
inc    di


loc_1DEB6:
cmp    di, 200
jb    short loc_1DE92

mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 10h
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [bp+Word_Field_Value], 0F1FAh
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
mov    [word ptr bp+Dword_Field_Value+2], 0
mov    [word ptr bp+Dword_Field_Value], 0
mov    [bp+Word_Field_Value], 0
push    si
mov    ax, 1
push    ax
mov    ax, 2
push    ax
lea    ax, [bp+Word_Field_Value]
push    ax
call    fwrite

add    sp, 8
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
push    si
mov    ax, 1
push    ax
mov    ax, 4
push    ax
lea    ax, [bp+Dword_Field_Value]
push    ax
call    fwrite

add    sp, 8
push    si
call    _fclose

pop    cx
call    MOUSE_CDraw_Restore

call    VGA_SetDrawFrame


loc_1DF54:
pop    di
pop    si
mov    sp, bp
pop    bp
retf

endp DBG_ScreenDump

;ends seg024

end
