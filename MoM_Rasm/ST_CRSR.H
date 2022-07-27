#ifndef ST_CRSR_H
#define ST_CRSR_H



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
// _s33p38
void CRH_Draw_DSP(int X_Pos, int Y_Pos);
// _s33p39
void CRL_Draw_RSP(void);
// _s33p40
void CRL_Draw_DSP(void);


#ifdef __cplusplus
}
#endif

#endif /* ST_CRSR_H */
