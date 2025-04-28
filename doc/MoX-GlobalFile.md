
Eh?

Uses a global/shared file handle and global/shared byte count for read/write

Exists in MoO1, but not MoO2
No equivalent in BCPP or AIL, not sure about FLIClib or MS C
¿ think I saw something somewhere that has the same r,rb,w,wb logic as seen in Global_File_Open() ?

super weird that Global_File_Seek() takes an unused file pointer argument
looks like it is equivalent to BCPP's rewind() / lseek()
Did they override BCPP CRTL fseek()?
FUCK.
In load_global_timbre(), the call to Global_File_Read() pushes the GTL file pointer.



Why do it like this?
    Scopeless file handle?
    ..._gf_size is never shared or out of scope, used uniquely/independently in Global_File_Read() and Global_File_Write()
    Shared across EXE swaps?



MGC seg011
WZD seg011

gfopen();
gfread();
gfwrite();
gfseek();
gfclose();

## _gf_bsize
XREF:
    Global_File_Read+9   mov     [_gf_bsize], ax
    Global_File_Read+1C  mov     cx, [_gf_bsize]
    Global_File_Write+9  mov     [_gf_bsize], ax
    Global_File_Write+1C mov     cx, [_gf_bsize]

## _gf_fhandle
XREF:
    Global_File_Open+2F        mov     [_gf_fhandle], ax
    Global_File_Open+5F        mov     [_gf_fhandle], ax
    Global_File_Open+6C        mov     [_gf_fhandle], ax
    Global_File_Open:@@Done    mov     ax, [_gf_fhandle]
    Global_File_Read+18        mov     bx, [_gf_fhandle]
    Global_File_Write+18       mov     bx, [_gf_fhandle]
    Global_File_Seek:loc_16A7E mov     bx, [_gf_fhandle]
    Global_File_Close+5        mov     bx, [_gf_fhandle]

## Global_File_Open()
MGC XREF:
    UU_GUI_LoadClickFile+15
    UU_GUI_SaveClickFile+12
    Play_Sound__STUB+EC
    Save_SAVE_GAM+54
    Load_SAVE_GAM+6B
WZD XREF:
    UU_GUI_LoadClickFile+15
    UU_GUI_SaveClickFile+12
    Play_Sound__STUB+EC
    Save_SAVE_GAM+54
    Load_SAVE_GAM+6B
    Combat_Cache_Read+2D
    Combat_Cache_Write+2D



MGC dseg:40DA                                                 BEGIN:  seg011 - Initialized Data
MGC dseg:40DA 72 00                                           str_r__MGC_seg011 db 'r',0
MGC dseg:40DC 72 62 00                                        str_rb__MGC_seg011 db 'rb',0
MGC dseg:40DF 77 00                                           str_w__MGC_seg011 db 'w',0
MGC dseg:40E1 77 62 00                                        str_wb__MGC_seg011 db 'wb',0
MGC dseg:40E1                                                 END:  seg011 - Initialized Data

MGC dseg:A5DC                                                 BEGIN:  seg011 - Uninitialized Data
MGC dseg:A5DC 00 00                                           _gf_bsize dw 0
MGC dseg:A5DE 00 00                                           _gf_fhandle dw 0
MGC dseg:A5DE                                                 END:  seg011 - Uninitialized Data

WZD dseg:7602                                                 BEGIN:  seg011 - Initialized Data
WZD dseg:7602 72 00                                           str_r__WZD_seg011 db 'r',0
WZD dseg:7604 72 62 00                                        str_rb__WZD_seg011 db 'rb',0
WZD dseg:7607 77 00                                           str_w__WZD_seg011 db 'w',0
WZD dseg:7609 77 62 00                                        str_wb__WZD_seg011 db 'wb',0
WZD dseg:7609                                                 END:  seg011 - Initialized Data

WZD dseg:E5E6                                                 BEGIN:  seg011 - Unitialized Data
WZD dseg:E5E6 00 00                                           _gf_bsize dw 0
WZD dseg:E5E8 00 00                                           _gf_fhandle dw 0
WZD dseg:E5E8                                                 END:  seg011 - Unitialized Data
