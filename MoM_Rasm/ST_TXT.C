
/*
    Initialized Data
*/

int g_VGA_Outline_Color = 0;                // dseg:4314
int g_VGA_Skip_AA_Pixels = 0;               // dseg:4316
// dseg:4318 UU_VGA_TextDraw_Enabled dw 0
// dseg:431A VGA_FltBlock_Count dw 0
// dseg:431C VGA_BlinkState dw 0FFFFh                ; 0 - normal, 1 - reverse, -1 - disabled

/*
    Uninitialized Data
*/

// dseg:A7F6 VGA_AAColor_Array db 16 dup(  0)
// dseg:A806 VGA_Current_AA_Color db 0
// dseg:A807 VGA_AA_Color_1 db 0
// dseg:A808 VGA_AA_Color_2 db 0
// dseg:A809 VGA_AA_Color_3 db 0
int g_VGA_Font_ColorIndex3; // dseg:A80A
int g_VGA_Font_ColorIndex2; // dseg:A80C
int g_VGA_Font_ColorIndex1; // dseg:A80E
int g_VGA_Font_Index; // dseg:A810
int g_VGA_TextDraw_Y; // dseg:A812
int g_VGA_TextDraw_X; // dseg:A814
// dseg:A816 gsa_FontStyleData dw 0                  ; single-loaded LBX entry set up in VGA_DAC_Init
// dseg:A818 gsa_BorderStyleData dw 0
// dseg:A81A gsa_Palette_Cursor_Data dw 0            ; 500h into the palette entry
// dseg:A81C UU_gsa_Palette_Data dw 0                ; 400h into the palette entry
// dseg:A81E gsa_Palette_Font_Colors dw 0            ; 300h into the palette entry, 16 arrays of 16 colors
// dseg:A820 VGA_TextLine_Starts dd 0                ; points to an array of starting string indexes for
// dseg:A824 VGA_TextLine_Tops dd 0                  ; points to an array of starting Y positions for
// dseg:A828 VGA_TextLine_Rights dd 0                ; points to an array of ending X positions for
// dseg:A82C VGA_TextLine_Lefts dd 0                 ; points to an array of starting X positions for
// dseg:A830 VGA_TextDraw_Lines dw 0                 ; the amount of elements in the VGA_TextLine_ arrays
// dseg:A832 VGA_FltBlock_Bottoms dw 4 dup(     0)   ; up to 4 bottom row positions used to reserve space
// dseg:A83A VGA_FltBlock_Rights dw 4 dup(     0)    ; up to 4 right column positions used to reserve space
// dseg:A842 VGA_FltBlock_Tops dw 4 dup(     0)      ; up to 4 top row positions used to reserve space
// dseg:A84A VGA_FltBlock_Lefts dw 4 dup(     0)     ; up to 4 left column positions used to reserve space
// dseg:A852 VGA_Blink_Speed dw 0
// dseg:A854 VGA_BlinkHue_C1 dw 0                    ; the component of C1 that holds the defining hue
