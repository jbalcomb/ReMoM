#ifndef STU_DBG_H
#define STU_DBG_H

#include <STDARG.H>  /* va_list; va_arg(), va_end(), va_start() */
#include <STDIO.H>   /* FILE; fclose(), fopen() */


extern char Debug_Log_FileName[];
extern FILE *Debug_Log_File;
extern char DBG_LBX_Filename[];
extern unsigned int DBG_LBX_EntryIndex;

// match gsa_MAINSCRN_0_AnimatedLogo, FlicHdr_SgmtAddr=0x1C03, frame index
// if (FLIC_Draw_EMM_C(tmp_SI, tmp_DI, FlicHdr_SgmtAddr, Frame_Index);)
extern unsigned int DBG_MAINSCRN_000;
extern unsigned int DBG_MAINSCRN_005;


// /*
//     ? use this to track what is load, and where ?
//     ? so as to test the hand-crafted, hard-coded, known bytes ?
//     ? ...including the compressed and decompressed FLIC Frame data ?
//     ? ...6bpp and 8bpp VGA DAC/color-map/palette ?
//     ? branches - EMM, FLIC/Frame-Type, chunks, chunk types, colors/changes ?
// */
// struct s_LBX_ENTRY
// {
//     char filename[13];
//     char lbxname[9];
//     unsigned int entry_count;
//     unsigned int lbx_type;
//     unsigned int entry;
//     unsigned char EmmHandle;
//     unsigned char EmmLogicalPage;
//     unsigned int EmmLogicalPageOffset;

// };
// typedef struct s_LBX_ENTRY LBX_ENTRY;

// LBX_ENTRY gLbxEntries[256];


//#define DEBUG 1
#ifdef DEBUG
// //#define DBGLOG(fmt, ...) // no-workie
// // #define DBGLOG(fmt, ...) dlvfprintf(fmt, __VA_ARGS__);
// // #define lDebug(level, fmt, ...) \
// //     do { \
// //         /* if (DEBUG_ENABLED && (debugLevel >= level)) */ \
// //         fprintf((debugFile ? debugFile : stderr), "DEBUG %s[%d] %s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
// //     } while(0)
// /// ...a sequence of comma-separated expressions in parentheses can be treated as a single argument
// //#define LOG(args) (printf("LOG: %s:%d ", __FILE__, __LINE__), printf args)
// //#define DBGLOG(args) dlvfprintf(args)
// #define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)
#define LOGA1(arg1) { \
    do { \
        if (DEBUG) fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, arg1 ); \
       } while(0) \
}
#else
#define DBGLOG  /* emptiness, when debugging is not enabled */
#endif

// #define DBGLOG() dlvprintf()
// #define LOG(fmt, ...)   dlvfprintf(fmt, __VA_ARGS__);
//#define printf dlvfprintf // no-workie

#ifdef DEBUG
#define HERE(argString) { \
    do { \
        fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString ); \
        fflush(Debug_Log_File); \
    } while(0); \
}
// e.g., DBGLOG_S16("FLIC_HDR.Current_Frame = %u", tmp_SI);
#define DBGLOG_S16(fmt, argFile, argLine, s_int_16) { \
    do { \
        fprintf(Debug_Log_File, fmt, s_int_16 ); \
        fflush(Debug_Log_File); \
    } while(0); \
}
#else
/* emptiness, when debugging is not enabled */
#define HERE  
#define DBGLOG_S16
#endif

#ifdef DEBUG
#define PUSH_WORLD() { \
    asm { \
        push ax \
        push bx \
        push cx \
        push dx \
        push si \
        push di \
        push ds \
        push es \
    } \
}
#define POP_WORLD() { \
    asm { \
        pop es \
        pop ds \
        pop di \
        pop si \
        pop dx \
        pop cx \
        pop bx \
        pop ax \
    } \
}
#else
/* emptiness, when debugging is not enabled */
#define PUSH_WORLD
#define POP_WORLD
#endif

// void test_here(const char *argString)
// {
//     do
//     {
// //        if (DEBUG)
// //        {
//             fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString );
// //        }
//     } while(0);
// }


// #define ASM_REGS_DECL() { \
//     unsigned int tmp_AX_Orig; \
//     unsigned int tmp_BX_Orig; \
//     unsigned int tmp_CX_Orig; \
//     unsigned int tmp_DX_Orig; \
//     unsigned int tmp_SI_Orig; \
//     unsigned int tmp_DI_Orig; \
//     unsigned int tmp_DS_Orig; \
//     unsigned int tmp_ES_Orig; \
//     unsigned int tmp_AX_Curr; \
//     unsigned int tmp_BX_Curr; \
//     unsigned int tmp_CX_Curr; \
//     unsigned int tmp_DX_Curr; \
//     unsigned int tmp_SI_Curr; \
//     unsigned int tmp_DI_Curr; \
//     unsigned int tmp_DS_Curr; \
//     unsigned int tmp_ES_Curr; \
// }

extern unsigned int dbg_AX_Orig;
extern unsigned int dbg_BX_Orig;
extern unsigned int dbg_CX_Orig;
extern unsigned int dbg_DX_Orig;
extern unsigned int dbg_SI_Orig;
extern unsigned int dbg_DI_Orig;
extern unsigned int dbg_DS_Orig;
extern unsigned int dbg_ES_Orig;
extern unsigned int dbg_AX_Curr;
extern unsigned int dbg_BX_Curr;
extern unsigned int dbg_CX_Curr;
extern unsigned int dbg_DX_Curr;
extern unsigned int dbg_SI_Curr;
extern unsigned int dbg_DI_Curr;
extern unsigned int dbg_DS_Curr;
extern unsigned int dbg_ES_Curr;

#define ASM_REGS_STOR_ORIG() { \
    dbg_AX_Orig = _AX; \
    dbg_BX_Orig = _BX; \
    dbg_CX_Orig = _CX; \
    dbg_DX_Orig = _DX; \
    dbg_SI_Orig = _SI; \
    dbg_DI_Orig = _DI; \
    dbg_DS_Orig = _DS; \
    dbg_ES_Orig = _ES; \
}

#define ASM_REGS_STOR_CURR() { \
    dbg_AX_Curr = _AX; \
    dbg_BX_Curr = _BX; \
    dbg_CX_Curr = _CX; \
    dbg_DX_Curr = _DX; \
    dbg_SI_Curr = _SI; \
    dbg_DI_Curr = _DI; \
    dbg_DS_Curr = _DS; \
    dbg_ES_Curr = _ES; \
}

#define ASM_REGS_LOAD_ORIG() { \
    _AX = dbg_AX_Orig; \
    _BX = dbg_BX_Orig; \
    _CX = dbg_CX_Orig; \
    _DX = dbg_DX_Orig; \
    _SI = dbg_SI_Orig; \
    _DI = dbg_DI_Orig; \
    _DS = dbg_DS_Orig; \
    _ES = dbg_ES_Orig; \
}

#define ASM_REGS_LOAD_CURR() { \
    _AX = dbg_AX_Curr; \
    _BX = dbg_BX_Curr; \
    _CX = dbg_CX_Curr; \
    _DX = dbg_DX_Curr; \
    _SI = dbg_SI_Curr; \
    _DI = dbg_DI_Curr; \
    _DS = dbg_DS_Curr; \
    _ES = dbg_ES_Curr; \
}

#define ASM_REGS_DUMP() { \
    ASM_REGS_STOR_CURR() \
    asm mov ax, seg DGROUP \
    asm mov ds, ax \
    asm assume ds:DGROUP \
    ASM_REGS_LOAD_ORIG() \
    dlvfprintf("DEBUG: [%s, %d] dbg_AX_Curr=0x%03X, dbg_BX_Curr=0x%03X, dbg_CX_Curr=0x%03X, dbg_DX_Curr=0x%03X, dbg_SI_Curr=0x%03X, dbg_DI_Curr=0x%03X, dbg_DS_Curr=0x%03X, dbg_ES_Curr=0x%03X, )\n", __FILE__, __LINE__, dbg_AX_Curr, dbg_BX_Curr, dbg_CX_Curr, dbg_DX_Curr, dbg_SI_Curr, dbg_DI_Curr, dbg_DS_Curr, dbg_ES_Curr); \
    ASM_REGS_LOAD_CURR() \
}

//J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_VL.H
//#define VGAMAPMASK(x) asm{cli;mov dx,SC_INDEX;mov al,SC_MAPMASK;mov ah,x;out dx,ax;sti;}
#ifdef DBG_ASM_HERE
#define DBG_ASM(argString) { \
    do { \
        fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString ); \
        fflush(Debug_Log_File); \
    } while(0); \
}
#else
/* emptiness, when debugging is not enabled */
#define DBG_ASM_HERE
#endif

#define ASM_HERE(argString) { \
    asm     push ax; \
    asm     push bx; \
    asm     push cx; \
    asm     push dx; \
    asm     push ds; \
    asm     push si; \
    asm     push es; \
    asm     push di; \
    asm     mov ax, seg DGROUP; \
    asm     mov ds, ax; \
    asm     assume ds:DGROUP; \
    fprintf(Debug_Log_File, "DEBUG: [%s, %d]: %s \n", __FILE__, __LINE__, argString ); \
    fflush(Debug_Log_File); \
    asm     pop di; \
    asm     pop es; \
    asm     pop si; \
    asm     pop ds; \
    asm     pop dx; \
    asm     pop cx; \
    asm     pop bx; \
    asm     pop ax; \
}

#ifdef DEBUG
// e.g., DBGLOG_r08("ah ... VGA_WriteMapMasks3[ScreenPage_X & 3]", _AH)
#define DBGLOG_r08(argString, argR08) { \
    do { \
        asm     push ax; \
        asm     push bx; \
        asm     push cx; \
        asm     push dx; \
        asm     push ds; \
        asm     push si; \
        asm     push es; \
        asm     push di; \
        asm     mov ax, seg DGROUP; \
        asm     mov ds, ax; \
        asm     assume ds:DGROUP; \
        fprintf(Debug_Log_File, "DEBUG: [%s, %d]: \"%s\" 0x%02X (%u) (%d)\n", __FILE__, __LINE__, argString, argR08, argR08, argR08); \
        fflush(Debug_Log_File); \
        asm     pop di; \
        asm     pop es; \
        asm     pop si; \
        asm     pop ds; \
        asm     pop dx; \
        asm     pop cx; \
        asm     pop bx; \
        asm     pop ax; \
    } while(0); \
}
#else
/* emptiness, when debugging is not enabled */
#define DBGLOG_r16
#endif

#ifdef DEBUG
// e.g., DBGLOG_r16("mov bx, [cs:cseg_Flic_Width]", _BX)
#define DBGLOG_r16(argString, argR16) { \
    do { \
        asm     push ax; \
        asm     push bx; \
        asm     push cx; \
        asm     push dx; \
        asm     push ds; \
        asm     push si; \
        asm     push es; \
        asm     push di; \
        asm     mov ax, seg DGROUP; \
        asm     mov ds, ax; \
        asm     assume ds:DGROUP; \
        fprintf(Debug_Log_File, "DEBUG: [%s, %d]: \"%s\" 0x%04X (%u) (%d)\n", __FILE__, __LINE__, argString, argR16, argR16, argR16); \
        fflush(Debug_Log_File); \
        asm     pop di; \
        asm     pop es; \
        asm     pop si; \
        asm     pop ds; \
        asm     pop dx; \
        asm     pop cx; \
        asm     pop bx; \
        asm     pop ax; \
    } while(0); \
}
#else
/* emptiness, when debugging is not enabled */
#define DBGLOG_r16
#endif


extern char DBG_CallingFunction[];

extern unsigned char DBG_gflag_MD_Enabled;
extern unsigned char DBG_gflag_CSR_Enabled;
extern unsigned char DBG_gflag_MD_INT_Handler_Installed;
extern unsigned char DBG_gflag_MD_INT_Handler_MvOnly;
extern unsigned char DBG_gflag_MD_INT_Handler_MvBtns;
// CDraw called from SCRN or MD_INT ?




#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

#if defined(__DOS16__)
int     _Cdecl          printf(const char _FAR *__format, ...);
#endif

void dbg_printf(const char *fmt, ...);

void Debug_Log_Startup(void);
void Debug_Log_Shutdown(void);

void dlvfprintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* STU_DBG_H */
