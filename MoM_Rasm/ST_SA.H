#ifndef ST_SA_H
#define ST_SA_H

/*
    "SPACE ALLOC" - STARLORD.EXE
*/


#include "ST_TYPE.H"


/*
    Simtex SPACE ALLOC
        - SAMB (Space Alloc Memory Block)
MGC seg007
MGC seg008
SEE: ST_SAMB.md
*/


/*
    Types
*/
/*
    SAMB Head
    SAMB Data
    'Allocation Type' {0,1,2}
    sah0, sah1, sah2
    sad0, sad1, sad2
*/
/*
    SA_Allocate_Space_No_Header() returns a SAMB Data Type 0
    SA_Allocate_Space() returns a SAMB Data Type 1
    SA_Alloc_First() returns a SAMB Data Type 2
    SA_Alloc_Next() returns a SAMB Data Type 2
    
    SA_Alloc_Validate() takes a SAMB Head Type 1
    SA_Alloc_First() takes a SAMB Head Type 1
    SA_Alloc_Next() takes a SAMB Head Type 1

    LBX_Load_Entry() takes no SAMB Head or, depending on 'Allocation Type', a SAMB Head Type 1 or a SAMB Head Type 2
        if 'Allocation Type' is allocation_type == 0, it calls SA_Allocate_Space_No_Header(), which calls farmalloc()


*/


/*
    Manifest Constants
*/

#define SAMB_RESERVED1   0
#define SAMB_RESERVED2   2
#define SAMB_MEMSIG1     4
#define SAMB_MEMSIG2     6
#define SAMB_SIZE        8
#define SAMB_USED       10
#define SAMB_unknown    12
#define SAMB_MARKED     14

#define SAMB_HDR_RESERVED1   0
#define SAMB_HDR_RESERVED2   2
#define SAMB_HDR_MEMSIG1     4
#define SAMB_HDR_MEMSIG2     6
#define SAMB_HDR_SIZE        8
#define SAMB_HDR_USED       10
#define SAMB_HDR_unknown    12
#define SAMB_HDR_MARKED     14

#define SA_MEMSIG1      0x12FA
#define SA_MEMSIG2      0x4ECF


/*
    Macros
*/

#define SA_Get_MemSig1(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_),  4) )

#define SA_Get_MemSig2(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_),  6) )

#define SA_Get_Size(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_),  8) )

#define SA_Get_Used(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), 10) )

#define SA_Set_MemSig1(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_),  4, SA_MEMSIG1) )

#define SA_Set_MemSig2(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_),  6, SA_MEMSIG2) )

#define SA_Set_Size(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_),  8, (_nparas_)) )

#define SA_Set_Used(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), 10, (_nparas_)) )


/*
    SimTex "SPACE ALLOC" Memory Block - Header
*/

//struct ST_Alloc_Space_Header_Struct {
//    int safety_malloc_bsize;
//    int safety_malloc_prevreal;
//    int MemSig1;
//    int MemSig2;
//    int Size_Paras;
//    int Used_Paras;
//    int unused_0C;
//    int Marked_Paras;
//};
//extern struct ST_Alloc_Space_Header_Struct LBX_Alloc_Space_Header;

struct s_SAMB  // sizeof(0x10)
{
    unsigned int safety_malloc_bsize;
    unsigned int safety_malloc_prevreal;
    unsigned int MemSig1;
    unsigned int MemSig2;
    unsigned int Size_Paras;
    unsigned int Used_Paras;
    unsigned int unused_0C;
    unsigned int Marked_Paras;
};
// typedef struct SAMB SAMB_head;
// typedef struct SAMB _FAR * FPTRMCB;    /* SPTRSAMB and FPTRSAMB is a type declared to be a far pointer to Simtex 'SPACE ALLOC' Memory Block   */
// typedef struct SAMB _SEG * SPTRMCB;
//extern struct s_SAMB SAMB;
typedef struct s_SAMB SAMB;


extern SAMB_ptr g_SAMB;


/*
    Near Heap
*/
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

/*
    Far Heap
*/
// //extern void _FAR * gfp_tmpSAMB;                     // MGC dseg:A5C0  WZD dseg:E5CA
// extern unsigned int g_RAM_Min_KB;                   // MGC dseg:A5C4  ; set to 583 in _main
// // _s08 SA_Alloc_Error+2C      push [g_RAM_Min_KB]
// // _s10 LBX_Error+9B           push [g_RAM_Min_KB]
// // _s10 RAM_SetMinKB:loc_16078 mov  [g_RAM_Min_KB], ax


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg007
*/
//int SA_Alloc_Validate(sgmt_addr SAMB_head)                                  // s07p04
//int SA_Alloc_Validate(SAMB_addr pSAMB_head)                                 // s07p04
int SA_Alloc_Validate(SAMB_ptr pSAMB_head);  // returns ST_STATUS: SUCCESS -1, FAILURE 0
unsigned char farpeekb(unsigned int sgmt, unsigned int ofst);               // s07p05
unsigned int farpeekw(unsigned int sgmt, unsigned int ofst);                // s07p06
unsigned long farpeekdw(unsigned int sgmt, unsigned int ofst);              // s07p07
void farpokeb(unsigned int sgmt, unsigned int ofst, unsigned char value);   // s07p08
void farpokew(unsigned int sgmt, unsigned int ofst, unsigned short value);  // s07p09
// UU? void farpokedw(unsigned int sgmt, unsigned int ofst, unsigned long val);  // s07p10

/*
    seg008
*/
SAMB_ptr SA_Allocate_Space(unsigned int nparas);                        // s08p07
SAMB_ptr SA_Allocate_MemBlk(unsigned int nparas);                       // s08p08/09
// unsigned int SA_Allocate_MemBlk_LbxHeader(unsigned int nparas);      // s08p08
// unsigned int SA_Allocate_MemBlk_EntrySize(unsigned int nparas);      // s08p09
SAMB_ptr SA_Alloc_First(SAMB_ptr SAMB_head, int nparas);                // s08p12
SAMB_ptr SA_Alloc_Next(SAMB_ptr SAMB_head, int nparas);                 // s08p14
unsigned int SA_GetFree(SAMB_ptr SAMB_head);                            // s08p15
SAMB_ptr SA_MK_FP0(SAMB_addr sgmt_addr);                                // s08p16
void SA_Alloc_Error(int errno, int value);                              // s08p19

#ifdef __cplusplus
}
#endif

#endif /* ST_SA_H */
