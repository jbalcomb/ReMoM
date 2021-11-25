


unsigned int UU_g_VGA_TextDraw_Initd = 0;   // dseg:4318

unsigned int gsa_VGAFILEH_Header;           // dseg:A7D4
unsigned int gsa_PaletteFlags;              // dseg:A7D6
unsigned int gsa_ShadingColors;             // dseg:A7D8  ; 5,376 bytes (1500h) into the palette entry
unsigned int gsa_ReplacementColors;         // dseg:A7DA  
unsigned int gsa_PaletteSaved;              // dseg:A7DC
unsigned int gsa_Palette;                   // dseg:A7DE
char g_PaletteLbxFileName[16];              // dseg:A7E0  ; set in VGA_DAC_Init
unsigned int gsa_IntensityScaleTable;       // dseg:A7F0
extern unsigned int gsa_gsa_PaletteLbxEntry; // dseg:A7F2
unsigned int gsa_PaletteLbxEntry;           // dseg:A7F4
//dseg:A7F6 VGA_AAColor_Array db 16 dup(  0)
//dseg:A806 VGA_Current_AA_Color db 0
//dseg:A807 VGA_AA_Color_1 db 0
//dseg:A808 VGA_AA_Color_2 db 0
//dseg:A809 VGA_AA_Color_3 db 0
//dseg:A80A VGA_Font_ColorIndex3 dw 0
//dseg:A80C VGA_Font_ColorIndex2 dw 0
//dseg:A80E VGA_Font_ColorIndex1 dw 0
//dseg:A810 VGA_Font_Index dw 0
//dseg:A812 ; int VGA_TextDraw_Y
//dseg:A812 VGA_TextDraw_Y dw 0
//dseg:A814 ; int VGA_TextDraw_X
//dseg:A814 VGA_TextDraw_X dw 0
unsigned int gsa_FontStyleData;             // dseg:A816
unsigned int gsa_BorderStyleData;           // dseg:A818
unsigned int gsa_Palette_Cursor_Data;       // dseg:A81A  ; 500h into the palette entry
unsigned int UU_gsa_Palette_Data;           // dseg:A81C  ; 400h into the palette entry
unsigned int gsa_Palette_Font_Colors;       // dseg:A81E  ; 300h into the palette entry, 16 arrays of 16 colors
void far * gfp_VGA_TextLine_Starts;         // dseg:A820
void far * gfp_VGA_TextLine_Tops;           // dseg:A824
void far * gfp_VGA_TextLine_Rights;         // dseg:A828
void far * gfp_VGA_TextLine_Lefts;          // dseg:A82C
unsigned int gsa_VGA_TextDraw_Lines;        // dseg:A830
//dseg:A832 VGA_FltBlock_Bottoms dw 4 dup(     0)   ; up to 4 bottom row positions used to reserve space
//dseg:A83A VGA_FltBlock_Rights dw 4 dup(     0)    ; up to 4 right column positions used to reserve space
//dseg:A842 VGA_FltBlock_Tops dw 4 dup(     0)      ; up to 4 top row positions used to reserve space
//dseg:A84A VGA_FltBlock_Lefts dw 4 dup(     0)     ; up to 4 left column positions used to reserve space
//dseg:A852 VGA_Blink_Speed dw 0
//dseg:A854 VGA_BlinkHue_C1 dw 0                    ; the component of C1 that holds the defining hue
//dseg:A856 ; char gVGAFILEH_LBX_Name[]
//dseg:A856 gVGAFILEH_LBX_Name db 16 dup(  0)
//dseg:A866 gVGAFILEH_LBX_Entry dw 0
