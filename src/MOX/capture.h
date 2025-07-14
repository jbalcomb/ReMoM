/*
    WIZARDS.EXE
        seg024

MoO2
    Module: capture
    Module: fields

*/
#ifndef CAPTURE_H
#define CAPTURE_H



#ifdef __cplusplus
extern "C" {
#endif



/*
    WZD  seg024
*/

// WZD s24p01
void F11_Key__WIP(void);

// WZD s24p02
void Screen_Flic_Capture__STUB(void);

// WZD s24p03
// VGA_GetDACBlock();

// WZD s24p04
// VGA_ReadScreenLine();

// WZD s24p05
void Release_Version(void);

// WZD s24p06
int Check_Release_Version(void);

// WZD s24p07
// Screen_Picture_Capture();

// WZD s24p08
// Capture_Screen_Block();



#ifdef __cplusplus
}
#endif

#endif /* CAPTURE_H */
