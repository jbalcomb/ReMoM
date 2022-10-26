// ST_VGA.H
// v2 #include "ST_DEF.H"     /* _FAR */
// MoO2: Module: fonts

#include "ST_DEF.H"

#include "ST_SA.H"      /* farpeekw(), SA_Allocate_Space() */
#include "ST_VGA.H"     /* gsa_FontStyleData, etc. */

unsigned int UU_TextLine_Initd = 0;       // dseg:4318

SAMB_ptr p_TextLine_Starts;                 // dseg:A820
SAMB_ptr p_TextLine_Tops;                   // dseg:A824
SAMB_ptr p_TextLine_Rights;                 // dseg:A828
SAMB_ptr p_TextLine_Lefts;                  // dseg:A82C


// s19p11
int VGA_GetFontHeight(void)
{
    int Font_Height;
    // v2 int _FAR * pFont_Height;
    
    // SMLM Font_Height = farpeekw(gsa_FontStyleData, 16); // s_FONT_HEADER.Font_Height
    // v2 // Font_Height = fp_FontStyleData[16];
    // v2 // Font_Height = GET_LE_16(fp_FontStyleData,16);
    // v2 // Font_Height = (int far *)MK_FP(gsa_FontStyleData,16);
    // v2 // Font_Height = (int far *)MK_FP(FP_SEG(fp_FontStyleData),FP_OFF(fp_FontStyleData)+16);
    // v2 // Font_Height = GET_LE_16((unsigned char far *)MK_FP(FP_SEG(fp_FontStyleData),FP_OFF(fp_FontStyleData)+16);
    // v2 pFont_Height = (int _FAR *)MK_FP(gsa_FontStyleData,16);
    // v2 Font_Height = *pFont_Height;
    // Font_Height = 12;
    Font_Height = GET_2B_OFS(p_FontStyleData, 16);  // SMLM

    return Font_Height;
}

// s19p14
void TextLine_Init(void)
{
    UU_TextLine_Initd = 1;
    p_TextLine_Lefts   = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    p_TextLine_Rights  = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    p_TextLine_Tops    = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    p_TextLine_Starts  = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
}
/*
Module: paragrph
	array (8 bytes) exclusion_area_x1
	Address: 02:001B8AF4
	Num elements:    4, Type:		unsigned integer (4 bytes) 

	array (8 bytes) exclusion_area_x2
	Address: 02:001B8AFC
	Num elements:    4, Type:		unsigned integer (4 bytes) 

	array (8 bytes) exclusion_area_y1
	Address: 02:001B8B04
	Num elements:    4, Type:		unsigned integer (4 bytes) 

	array (8 bytes) exclusion_area_y2
	Address: 02:001B8B0C
	Num elements:    4, Type:		unsigned integer (4 bytes) 

	array (200 bytes) paragraph_line_y_start
	Address: 02:001B8B14
	Num elements:  100, Type:		unsigned integer (4 bytes) 

	array (200 bytes) paragraph_line_x_start
	Address: 02:001B8BDC
	Num elements:  100, Type:		unsigned integer (4 bytes) 

	array (200 bytes) paragraph_line_offset
	Address: 02:001B8CA4
	Num elements:  100, Type:		unsigned integer (4 bytes) 

	array (200 bytes) paragraph_line_x_end
	Address: 02:001B8D6C
	Num elements:  100, Type:		unsigned integer (4 bytes) 

	signed integer (2 bytes) loaded_paragraph_space_flag
	Address: 02:0017C9EC

	signed integer (2 bytes) paragraph_max_lines
	Address: 02:001B8E34

	signed integer (2 bytes) exclusion_count
	Address: 02:0017C9EE
*/