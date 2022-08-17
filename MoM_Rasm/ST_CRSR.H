#ifndef ST_CRSR_H
#define ST_CRSR_H


#define CURSOR_WIDTH    16
#define CURSOR_HEIGHT   16

#define CR_TYPE_NONE     0

enum Cursor_Types {
    Crsr_None,
    Crsr_Normal,
    Crsr_CastBase,
    Crsr_RedCross,
    Crsr_Ranged,
    Crsr_Melee,
    Crsr_Hourglas,
    Crsr_MoveHat,
    Crsr_Empty,
    Crsr_CastAnim,
    Crsr_CastAnim2,
    Crsr_CastAnim3,
    Crsr_CastAnim4,
    Crsr_CastAnim5
};

struct s_Cursor_Area_Copy {                  // sizeof=0x186  390 bytes (2+2+2+384)
    /*00h*/ int ScreenPageOffset;            // e.g., (X,Y)=(158,100); ColumnOffset=(158//4=39); RowOffset=(100*80=8000); PageOffset=(39+8000)=8039
    /*02h*/ int Width_Bytes;                 // Width in Addresses  (~=Pixels/4)
    /*04h*/ int Height_Lines;                // Height in Pixels  ... Rows ... "Lines" as in "Scan-Lines"
    /*06h*/ unsigned char Saved_Image[384];  // ? 384 / 16 = 24 ?
};
/*
?
    810 + 390 = 1200
    2 * 390 = 780
    3 * 390 = 1170
    1200 - 1170 = 30
    mayhap it is actually 400 * 3 = 1200?
?
*/


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


// _s33p30.asm
void CRL_Save_RSP(int X_Pos, int Y_Pos);
// _s33p31.asm
void CRL_Save_DSP(int X_Pos, int Y_Pos);

// _s33p32a.c
void IN_CRL_Save_RSP(int X_Pos, int Y_Pos);

// _s33p33.asm
void CRL_Copy_DSP2RSP(void);

// _s33p34
void CRL_Restore_RSP(void);
// _s33p35
void CRL_Restore_DSP(void);

// _s33p37
void CRH_Draw_RSP(int X_Pos, int Y_Pos);
void CRH_Draw_RSP_C(int X_Pos, int Y_Pos);
// _s33p38
void CRH_Draw_DSP(int X_Pos, int Y_Pos);
void CRH_Draw_DSP_C(int X_Pos, int Y_Pos);
// _s33p39
//void CRL_Draw_RSP(void);
void CRL_Draw_RSP(int X_Pos, int Y_Pos, int CrsrIdx);
// _s33p40
void CRL_Draw_DSP(int X_Pos, int Y_Pos, int CrsrIdx);

//void CR_Draw(int X_Pos, int Y_Pos, int ScreenPage_Idx);
void CR_Draw_C(int CR_X, int CR_Y, int ScreenPage_Idx);
void CR_Store_C(int CR_X, int CR_Y, int ScreenPage_Idx);
void CR_Restore_C(int ScreenPage_Idx);



#ifdef __cplusplus
}
#endif

#endif /* ST_CRSR_H */
