#ifndef ST_SA_H
#define ST_SA_H
/*
    Simtex SPACE ALLOC
        - SAMB (Space Alloc Memory Block)
seg007
seg008
*/

//#define LBXHEADER {if(!g_LBX_Header_Allocd){g_LBX_Header_Allocd = 1;g_LBX_Header = AllocMem_LbxHeader(32);}}
#define SA_FarMalloc(nparas) {(void far *) farmalloc( ((unsigned long)nparas * 16) + 16 )}
#define SA_GetSize(samb) {farpeekw(samb, 0x08)}
#define SA_GetUsed(samb) {farpeekw(samb, 0x0A)}
#define SA_GetMarked(samb) {farpeekw(samb, 0x0E)}
#define SA_SetSize(samb,nparas) {farpokew(samb, 0x08, nparas)}
#define SA_SetUsed(samb,nparas) {farpokew(samb, 0x0A, nparas)}
#define SA_SetMarked(samb,nparas) {farpokew(samb, 0x0E, nparas)}

#define LBX_MemSig1 0x12FA
#define LBX_MemSig2 0x4ECF
#define SA_MEMSIG1 0x12FA
#define SA_MEMSIG2 0x4ECF
#define SA_MemBlkSig 0x12FA4ECF


/*
    Simtex 'SPACE ALLOC' Memory Block - Header
*/

struct ST_Alloc_Space_Header_Struct {
    int safety_malloc_bsize;
    int safety_malloc_prevreal;
    int MemSig1;
    int MemSig2;
    int Size_Paras;
    int Used_Paras;
    int unused_0C;
    int Marked_Paras;
};

extern struct ST_Alloc_Space_Header_Struct LBX_Alloc_Space_Header;

struct SAMB {
    unsigned int safety_malloc_bsize;
    unsigned int safety_malloc_prevreal;
    unsigned int MemSig1;
    unsigned int MemSig2;
    unsigned int Size_Paras;
    unsigned int Used_Paras;
    unsigned int unused_0C;
    unsigned int Marked_Paras;
};
typedef struct SAMB SAMB_head;
typedef struct SAMB far * FPTRMCB;    /* SPTRSAMB and FPTRSAMB is a type declared to be a far pointer to Simtex 'SPACE ALLOC' Memory Block   */
typedef struct SAMB _seg * SPTRMCB;
#define SAMB_RESERVED1   0
#define SAMB_RESERVED2   2
#define SAMB_MEMSIG1     4
#define SAMB_MEMSIG2     6
#define SAMB_SIZE        8
#define SAMB_USED       10
#define SAMB_unknown    12
#define SAMB_MARKED     14


/*
extern char *cnst_Alloc_Error01;            // dseg:3D56
extern char *cnst_Alloc_Error02;            // dseg:3D74
extern char *cnst_Alloc_Error51;            // dseg:3D7B
extern char *cnst_Alloc_Error52;            // dseg:3DA3
extern char *cnst_Alloc_Error11;            // dseg:3DC3
extern char *cnst_Alloc_Error12;            // dseg:3DF5
extern char *cnst_Alloc_Error13;            // dseg:3DFA
extern char *cnst_Alloc_Error21;            // dseg:3E02
extern char *cnst_Alloc_Error22;            // dseg:3E13
extern char *cnst_Alloc_Error23;            // dseg:3E2A
extern char *cnst_Alloc_Error3;             // dseg:3E36
extern char *cnst_Alloc_Error4;             // dseg:3E5E
*/

extern unsigned int g_NearHeap_Mark;                // dseg:948E g_NearHeap_Mark dw 0
extern char Temp_String[];                          // dseg:9490
extern char Tmp_Conv_Str_1[];                       // dseg:94F4
extern char Tmp_Conv_Str_2[];                       // dseg:9508
extern char Tmp_Conv_Str_3[];                       // dseg:9526
extern unsigned char SA_NearHeap_Buffer[];          // dseg:9590 SA_NearHeap_Buffer db 4144 dup(0)
extern void far * gfp_tmpSAMB;                      // dseg:A5C0
extern unsigned int g_RAM_Min_KB;                   // dseg:A5C4


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg007
*/

/*
    seg008
*/
//void _seg *
unsigned int SA_Allocate_Space(unsigned int nparas);            // _s08p07c.c
unsigned int SA_Allocate_MemBlk(unsigned int nparas); // _s08p08c.c
// unsigned int SA_Allocate_MemBlk_LbxHeader(unsigned int nparas); // _s08p08c.c
// unsigned int SA_Allocate_MemBlk_EntrySize(unsigned int nparas); // _s08p09c.c

unsigned int SA_Alloc_GetFree(unsigned int Alloc_Seg);          // _s08p15.asm
// Error: Size of '__FAR' is known or zero
void far * SA_MK_FP0(unsigned int sgmt_addr);                   // _s08p16.asm

void SA_Alloc_Error(int errno, int value);                      // _s08p19c.c

#ifdef __cplusplus
}
#endif

#endif /* ST_SA_H */
