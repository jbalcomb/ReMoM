
/*


Current State:
Debug

TST_MGC.C
MGC_MAIN.C
mgc_main()

MGC_DEF.H

_s01p03c.c
GAME_MainMenu()
    SCREEN_Menu()

_s01p05c.c
SCREEN_Menu()
    Loop:
        IN_GetInput()
        ...
        SCREEN_Menu_Draw()
        SCRN_SimplePageFlip()

_s01p06c.c
SCREEN_Menu_Draw()
    GUI_MousedControl()
    ...
    Loop: "MOM animated logo"
    j_GAME_DrawCredits()
    ...
    Loop:


? If/How ... just a single call to SCREEN_Menu_Draw() ?
    convoluted...because, checks input before drawing..
    so, set debug var to true, enter loop and set to false

PoI - Render Screen
? first happens in IN_GetInput ?
? on account of handling mouse movement, mouse-over control highlights, and right-click pop-up help ?

Next-Up?
    _s01p06c.c SCREEN_Menu_Draw()
        _s34p26     GUI_MousedControl()
            _s34p25     GUI_MouseOverControl()

    _s01p05c.c SCREEN_Menu()
        [?] GAME_LoadSettings()

    _s34p26     GUI_MousedControl()
    _s34p71     GUI_DrawControls()
    _s34p66c.c  IN_GetInput()

GUI_MousedControl()
    e_GUI_Controls
    s_GUI_CTRL
    gfp_GUI_Control_Table:DWORD
    g_GUI_CursorOffset:WORD
    g_GUI_FocusedControl:WORD
    GUI_FindWindow:PROC
    GUI_GetCursorOffset:PROC
    GUI_MouseOverControl:PROC
    MD_GetX:PROC
    MD_GetY:PROC

GUI_MouseOverControl()    _s34p25.asm
    short-circuits on (g_GUI_Control_Count == 1) .:. returns 0
        ? meaning gfp_GUI_Control_Table[0] is special ? and can not be a /real/ control ? or, is it just "number" vs. "index" ?
        ...seems so...SCREEN_Menu() calls IN_GetInput() and then checks for 0 to set flag_input to ST_TRUE
        ...however, it does them go on to check it against all control indexes
        Also, IN_GetInput() returns 0 if ( g_GUI_Control_Count <= 1 )
    s_GUI_CTRL
    gfp_GUI_Control_Table:DWORD
    g_GUI_Control_Count:WORD
    g_GUI_CursorOffset:WORD
    GUI_FindWindow:PROC
    GUI_GetCursorOffset:PROC
    MD_GetX:PROC
    MD_GetY:PROC


GUI_FindWindow()
    short-circuits on (g_GUI_WindowCount == 1)
    sets g_GUI_PrevCursor = g_GUI_CurrentCursor
    sets g_GUI_CursorOffset = GUI_WINDOW[].Cursor_Offset
    sets g_GUI_CurrentCursor = GUI_WINDOW[].Cursor_Index



_s34p22 GUI_NoTriggerKeys
_s34p25 GUI_MouseOverControl
_s34p26 GUI_MousedControl
_s34p71 GUI_DrawControls
_s34p72 GUI_DrawControl

*/


/*
    BCpp31PG, pg 175/190
        #pragma warn -yyy
    BCpp31UG, pg 162/171
    BCpp30UG, pg 158/167
        -waus*  Identifier is assigned a value that is never used.
        -wccc*  Condition is always true/false.
        -weff*  Code has no effect.
        -wpar*  Parameter parameter is never used.
        -wuse   Identifier is declared but never used.
        -wrpt*  Nonportable pointer conversion.
*/
#pragma warn -aus
#pragma warn -eff
#pragma warn -par

//#include <stdlib.h>
#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
#include <DOS.H>

//#include "MGC_HEAD.H"
#include "MGC_DEF.H"
//#include "MOM_HEAD.H"
#include "MOM_DEF.H"
#include "ST_HEAD.H"
#include "ST_TYPE.H"

#include "ST_CRSR.H"
#include "ST_CTRL.H"
#include "ST_FLIC.H"
#include "ST_GUI.H"
#include "ST_LBX.H"
#include "ST_SA.H"
#include "ST_SCRN.H"
#include "ST_VGA.H"

#include "TST_MGC.H"

#include "STU_DBG.H"
#include "STU_VGA.H"


#ifdef __cplusplus
extern "C" {
#endif
void Y4_Set_Pixel(int x, int y, unsigned char color);
#ifdef __cplusplus
}
#endif



#define FONT_FILE_NAME "FONTS.LBX"
char *g_FontFileName = "FONTS.LBX";

// typedef struct SAMB_struct SAMB;
// typedef far * SAMB fp_SAMB;
// s_SAMB far * gfp_SAMB;

// extern byte far gamepal;
// extern	pictabletype	_seg *pictable;
// extern	pictabletype	_seg *picmtable;
// extern	spritetabletype _seg *spritetable;


void test_defines(void);
void test_binary_math_multivalue(void);

void test_set_mem_min_ram(void);
void test_set_mem_min_emm(void);
/*
    seg009
*/
void test_lbx_open(void);
void test_lbx_close(void);
void test_lbx_seek(void);
//void test_lbx_read_sgmt(void);
//void test_lbx_read_ofst(void);
void test_strcpyfar(void);

void test_coreleft(void);
void test_farcoreleft(void);
void test_dos_allocmem(void);
void test_dos_freemem(void);
//void test_allocmem(void);
//void test_freemem(void);
void test_malloc(void);
void test_farmalloc(void);

void test_pointers(void);
unsigned int __FAR * make_ui_fptr(unsigned long ul_fptr_address);
char __FAR * SgmtAddr_FarPtr1(unsigned int segment_address);
char __FAR * SgmtAddr_FarPtr2(unsigned int segment_address);
char __FAR * SgmtAddr_FarPtr3(unsigned int segment_address);
void __FAR * MK_FP0(unsigned int sgmt_addr);

// void test_LbxLdEt1(void);  // NOTE(JimBalcomb,20220907): moved out to 'NoGit'

void test_asciz_character_array(void);

void test_SA_Allocate_Space(void);
void test_SA_Allocate_MemBlk(void);

void test_Hardware_Init(void);
void test_VGA_DAC_Init(void);

void test_VGA_SaveCursorArea_RSP(void);

void test_DISK_GetFileSize(void);

void test_EMM_Load_LBX_File(void);

//void test_VGA_LoadPalette(void);

void test_SimTexGameEngineFramework(void);

void test_FLIC_Draw_XY(void);
void test_Screen_MainMenu(void);
void test_SCREEN_Menu(void);

void test_CRL_Draw_RSP(void);

void test_CR_Draw_C(void);



/*
    J:\STU\DBWD\developc\Animator\Vpaint\COMMON\ptr.h
    norm_pointer() / enorm_pointer() / pj_enorm_pointer()
    J:\STU\DBWD\developc\Animator\Vpaint\PJ\inc\compiler.h
    J:\STU\DBWD\developc\Animator\Vpaint\PJ\util\ptrenorm.c
    J:\STU\DBWD\developc\Animator\Vpaint\V\peekpoke.asm

*/
typedef long intptr_t;
//void *make_ptr(void *pt);
////void *enorm_pointer(void *p);
////void *norm_pointer(void *p);
//unsigned int ptr_offset(int offset, int seg);
//unsigned int ptr_seg(int offset, int seg);
//void *ptr_next_seg(void *p);
//long pt_to_long(unsigned offset, unsigned seg);
//void *long_to_pt(unsigned long l);
void *make_ptr();
void *norm_pointer(void *p);
unsigned int ptr_offset();
unsigned int ptr_seg();
void *ptr_next_seg(void *p);
long pt_to_long();
void *long_to_pt();


int main(void)
{
    Debug_Log_Startup();

    // test_defines();

    // test_lbx_open();
    // test_lbx_close();
    // test_lbx_seek();

//void test_lbx_read_sgmt(void);
//void test_lbx_read_ofst(void);
//void test_strcpyfar(void);

    // test_coreleft();
    // test_farcoreleft();
    // test_dos_allocmem();
    // test_dos_freemem();
    // test_malloc();
    // test_farmalloc();

    //test_pointers();

    //test_LbxLdEt1();  // NOTE(JimBalcomb,20220907): moved out to 'NoGit'

    //test_asciz_character_array();

    // test_SA_Allocate_Space();
    // test_SA_Allocate_MemBlk();

    //test_Hardware_Init();
    //test_VGA_DAC_Init();
    //test_VGA_SaveCursorArea_RSP();

    //test_DISK_GetFileSize();

    // test_EMM_Load_LBX_File();

    // TBD: test_VGA_LoadPalette();

    //test_SimTexGameEngineFramework();

// ? test_FLIC_Draw_XY();

    //test_Screen_MainMenu();

    //test_binary_math_multivalue();

//    test_CRL_Draw_RSP();

//    test_draw_flic_frame();

//    test_CR_Draw_C();  // TST_CRSR.C

    MGC_Main();

    
    Debug_Log_Shutdown();
    return 0;
}

void test_defines(void)
{
    /*
        Compiler
                Borland Turbo C
                Borland C++
                Microsoft C
                Mars C
                Watcom
    */
#if defined(__TURBOC__)
    printf("__TURBOC__ = %d\n", __TURBOC__);
#else
    printf("__TURBOC__ = <undefined>\n");
#endif

    /*
        Memory Models
    */

#if defined(__COMPACT__)
    printf("__COMPACT__ = %d\n", __COMPACT__);
#else
    printf("__COMPACT__ = <undefined>\n");
#endif

#if defined(__HUGE__)
    printf("__HUGE__ = %d\n", __HUGE__);
#else
    printf("__HUGE__ = <undefined>\n");
#endif

#if defined(__LARGE__)
    printf("__LARGE__ = %d\n", __LARGE__);
#else
    printf("__LARGE__ = <undefined>\n");
#endif

#if defined(__MEDIUM__)
    printf("__MEDIUM__ = %d\n", __MEDIUM__);
#else
    printf("__MEDIUM__ = <undefined>\n");
#endif

#if defined(__SMALL__)
    printf("__SMALL__ = %d\n", __SMALL__);
#else
    printf("__SMALL__ = <undefined>\n");
#endif

#if defined(__TINY__)
    printf("__TINY__ = %d\n", __TINY__);
#else
    printf("__TINY__ = <undefined>\n");
#endif

#if defined(LDATA)
    printf("LDATA = %d\n", LDATA);
#else
    printf("LDATA = <undefined>\n");
#endif

#if defined(LPROG)
    printf("LPROG = %d\n", LPROG);
#else
    printf("LPROG = <undefined>\n");
#endif


    /*
        FAR's
    */

#if defined(_FAR)
    // printf("_FAR = %s\n", _FAR);
    printf("_FAR is defined\n");
#else
    printf("_FAR = <undefined>\n");
#endif

#if defined(_FARCALL)
    // printf("_FARCALL = %d\n", _FARCALL);
    printf("_FARCALL is defined\n");
#else
    printf("_FARCALL = <undefined>\n");
#endif

#if defined(_FARFUNC)
    // printf("_FARFUNC = %d\n", _FARFUNC);
    printf("_FARFUNC is defined\n");
#else
    printf("_FARFUNC = <undefined>\n");
#endif

    /*
        Platform / Operating System (OS)
    */

#if defined(__OS2__)
    printf("__OS2__ = %d\n", __OS2__);
#else
    printf("__OS2__ = <undefined>\n");
#endif

#if defined(_WIN32)
    printf("_WIN32 = %d\n", _WIN32);
#else
    printf("_WIN32 = <undefined>\n");
#endif

#if defined(_Windows)
    printf("_Windows = %d\n", _Windows);
#else
    printf("_Windows = <undefined>\n");
#endif

    /*
        C / C++
                ? ANSI C ?
    */

#if defined(__STDC__)
    printf("__STDC__ = %d\n", __STDC__);
#else
    printf("__STDC__ = <undefined>\n");
#endif

#if defined(__cplusplus)
    printf("__cplusplus = %d\n", __cplusplus);
#else
    printf("__cplusplus = <undefined>\n");
#endif

    /*
        IDK
    */

#if defined(_BUILDRTLDLL)
    printf("_BUILDRTLDLL = %d\n", _BUILDRTLDLL);
#else
    printf("_BUILDRTLDLL = <undefined>\n");
#endif

#if defined(_CLASSDLL)
    printf("_CLASSDLL = %d\n", _CLASSDLL);
#else
    printf("_CLASSDLL = <undefined>\n");
#endif

#if defined(_CLASSTYPE)
    // printf("_CLASSTYPE = %d\n", _CLASSTYPE);
    printf("_CLASSTYPE is defined\n");
#else
    printf("_CLASSTYPE = <undefined>\n");
#endif

#if defined(__DLL__)
    printf("__DLL__ = %d\n", __DLL__);
#else
    printf("__DLL__ = <undefined>\n");
#endif

}

void test_binary_math_multivalue(void)
{
    /*
        e.g.,
            EMM Page and Offset
            MAINSCRN,5
            153872 / 16384 =    9 (0x0009)
            153872 % 16384 = 6416 (0x1910)
            00025910
            0002
            5910
            00000000 00000010 01011001 00010000
            00000000 00000010
            01011001 00010000
            How many 16384's? ...32768's? ...65536's?
            low-word
                01011001 00010000
                |\-16384's
                \-32768's
            high-word:
                00000000 00000010 - 65536's


    */
    unsigned char tmp_16384s;
    unsigned char tmp_32768s;
    unsigned char tmp_65536s;
    unsigned char tmp_16384s_high;
    unsigned char tmp_16384s_low;
    unsigned char tmp_16384s_total;
    unsigned long value;
    unsigned int value_high;
    unsigned int value_low;
    long DataOffset_Begin;
    unsigned char Entry_Begin_Logical_Page_Number;
    unsigned int Entry_Begin_Logical_Page_Offset;
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    unsigned int tmp_Src_Ofst;
    unsigned int tmp_Src_Sgmt;
    int EmmLogicalPage;

    value = 153872;
    value_high = (unsigned int)((value & 0xFFFF0000) >> 16);
    value_low = (unsigned int)(value & 0x0000FFFF);
    printf("value: 0x%08lX\n", value);
    printf("value_high: 0x%04X\n", value_high);
    printf("value_low: 0x%04X\n", value_low);

    printf("value: %u\n", ( (value & 0x00000001) != 0 ));
    printf("value_high: %u\n", ( (value_high & 0x0002) != 0 ));
    printf("value_low: %u\n", ( (value_low & 0x4000) != 0 ));
    printf("value_low: %u\n", ( (value_low & 0x1000) != 0 ));

    tmp_65536s = value_high;
    printf("How many 65536's in the high-word of the long value? %u\n", tmp_65536s);

    if ( (value_low & 0x8000) != 0 )  // 1000 0000 0000 0000
    {
        tmp_32768s = 1;
    }
    else
    {
        tmp_32768s = 0;
    }
    printf("How many 32768's in the low-word of the long value? %u\n", tmp_32768s);

    if ( (value_low & 0x4000) != 0 )  // 0100 0000 0000 0000
    {
        tmp_16384s = 1;
    }
    else
    {
        tmp_16384s = 0;
    }

    tmp_16384s_high = (tmp_65536s * 4);
    printf("How many 16384's in the high-word of the long value? %u\n", tmp_16384s_high);
    tmp_16384s_low = ( (tmp_32768s * 2) + (tmp_16384s * 1) );
    printf("How many 16384's in the low-word of the long value? %u\n", tmp_16384s_low);
    tmp_16384s_total = ( tmp_16384s_high + tmp_16384s_low);
    printf("How many 16384's in the long value? %u\n", tmp_16384s_total);

    DataOffset_Begin = 153872;
    printf("DataOffset_Begin: %ld\n", DataOffset_Begin);
    Entry_Begin_Logical_Page_Number = DataOffset_Begin / 16384;  // e_EMM_Page_Size (0x4000)
    Entry_Begin_Logical_Page_Offset = DataOffset_Begin % 16384;  // e_EMM_Page_Size (0x4000)
    printf("Entry_Begin_Logical_Page_Number: %u\n", Entry_Begin_Logical_Page_Number);
    printf("Entry_Begin_Logical_Page_Offset: %u\n", Entry_Begin_Logical_Page_Offset);
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);         // Hi-Word of Double-Word
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);  // Lo-Word of Double-Word
    printf("tmp_DataOffsetBegin_Sgmt: %u (0x%04X)\n", tmp_DataOffsetBegin_Sgmt, tmp_DataOffsetBegin_Sgmt);
    printf("tmp_DataOffsetBegin_Ofst: %u (0x%04X)\n", tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Ofst);

    tmp_Src_Sgmt = tmp_DataOffsetBegin_Sgmt;
    tmp_Src_Ofst = tmp_DataOffsetBegin_Ofst;
    printf("tmp_Src_Sgmt: %u (0x%04X)\n", tmp_Src_Sgmt, tmp_Src_Sgmt);
    printf("tmp_Src_Ofst: %u (0x%04X)\n", tmp_Src_Ofst, tmp_Src_Ofst);
    tmp_Src_Sgmt = (tmp_Src_Sgmt << 2);
    printf("tmp_Src_Sgmt: %u (0x%04X)\n", tmp_Src_Sgmt, tmp_Src_Sgmt);
    if ( (tmp_Src_Ofst & 0x8000) != 0 )     // 1000 0000 0000 0000 - test bit #16
    {
        tmp_Src_Sgmt = (tmp_Src_Sgmt | 0x0002);     // 0000 0000 0000 0010 - set bit #2
    }
    if ( (tmp_Src_Ofst & 0x4000) != 0 )         // 0100 0000 0000 0000 - test bit #15
    {
        tmp_Src_Sgmt = (tmp_Src_Sgmt | 0x0001);     // 0000 0000 0000 0010 - set bit #1
    }
    printf("tmp_Src_Sgmt: %u (0x%04X)\n", tmp_Src_Sgmt, tmp_Src_Sgmt);
    EmmLogicalPage = tmp_Src_Sgmt;
    printf("EmmLogicalPage: %d (0x%04X)\n", EmmLogicalPage, EmmLogicalPage);
    tmp_Src_Ofst = (tmp_Src_Ofst & 0x3FFF);         //  - clear bits #16 and #15
    printf("tmp_Src_Ofst: %d (0x%04X)\n", tmp_Src_Ofst, tmp_Src_Ofst);

}

void test_set_mem_min_ram(void)
{

}

void test_set_mem_min_emm(void)
{

}
// _s09p01
void test_lbx_open(void)
{
    int fhandle;

    fhandle = 0;

    fhandle = lbx_open(g_FontFileName);

    if ( fhandle == 0 )
    {
        printf("DEBUG: FAILURE: lbx_open() fhandle == %d\n", fhandle);
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_open() fhandle == %d\n", fhandle);
    }
}
// _s09p01
void test_lbx_close(void)
{
    int fhandle;
    int result;
    char result_hi;

    fhandle = 0;
    result = 0;
    result_hi = 0;

    fhandle = lbx_open(g_FontFileName);

    result = lbx_close(fhandle);

    result_hi = (char)(result >> 8);

    if ( result_hi == 0x3E )
    {
        printf("DEBUG: SUCCESS: lbx_close() result_hi == 0x%02X\n", result_hi);
    }
    else
    {
        printf("DEBUG: FAILURE: lbx_close() result_hi == 0x%02X\n", result_hi);
    }
}

void test_lbx_seek(void)
{
    int fhandle;
    int result;
    char result_hi;
    char result_lo;
    char *ptr; 

    fhandle = 0;
    result = 0;

    fhandle = lbx_open(g_FontFileName);

    result = lbx_seek(512, fhandle);

    if ( result == -1 )
    {
        printf("DEBUG: SUCCESS: lbx_seek() result == %d\n", result);
    }
    else
    {
        printf("DEBUG: FAILURE: lbx_seek() result == %d\n", result);
    }

    lbx_close(fhandle);
}

// void test_lbx_read_sgmt(void)
// {
// int fhandle;
//     long filelength;

//     fhandle = 0;
//     filelength = 0;

//     fhandle = lbx_open(g_FontFileName);

//     //lbx_read_sgmt(fhandle);

//     lbx_close(fhandle);
// }

// void test_lbx_ofst(void)
// {

// }

void test_strcpyfar(void)
{

}

/*
    BCpp31LR, pg 89/102
        #include <alloc.h>
        unsigned long coreleft(void);
            printf("The difference between the highest allocated block and\n");
            printf("the top of the heap is: %lu bytes\n", (unsigned long) coreleft());
*/
void test_coreleft(void)
{
    unsigned long heap_size;

    heap_size = coreleft();

    printf("DEBUG: test_coreleft() heap_size == %lu\n", heap_size);
    printf("DEBUG: test_coreleft() heap_size == 0x%08X\n", heap_size);

}
/*
    BCpp31LR, pg 147/160
        #include <alloc.h>
        unsigned long farcoreleft(void);
            printf("The difference between the highest allocated block in the far\n");
            printf("heap and the top of the far heap is: %lu bytes\n", farcoreleft());
*/
void test_farcoreleft(void)
{
    unsigned long farheap_size;

    farheap_size = farcoreleft();
    
    printf("DEBUG: test_farcoreleft() farheap_size == %lu\n", farheap_size);
    printf("DEBUG: test_farcoreleft() farheap_size == 0x%08X\n", farheap_size);
}
/*
    J:\STU\DBWD\BORLANDC\CRTL\CLIB\DOSALLOC.C
    BCpp31LR, pg 18/29
        #include <dos.h>
        unsigned _dos_allocmem(unsigned size, unsigned *segp);
            unsigned int segp;
            unsigned int size = 64; / * (64*16) = 1024 bytes * /
            if (_dos_allocmem(size, &segp) == 0)
                printf("Allocated memory at segment: %x\n", segp);
            else {
                perror("Unable to allocate block.");
                printf("Maximum no. of paragraphs"
                       " available is %u\n", segp);
                return 1;
            }
*/
void test_dos_allocmem(void)
{
    unsigned int size;
    unsigned int segp;
    unsigned int result;

    size = 64; /* (64 x 16) = 1024 bytes */

    result = _dos_allocmem(size, &segp);

    if ( result == 0 )
    {
        printf("DEBUG: SUCCESS: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() segp == 0x%04X\n", segp);
    }
    else
    {
        printf("DEBUG: FAILURE: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() segp == 0x%04X\n", segp);
    }
}
/*
    J:\STU\DBWD\BORLANDC\CRTL\CLIB\DOSFREE.C
    BCpp31LR, pg 191/206
        #include <dos.h>
        unsigned _dos_freemem(unsigned segx);
        unsigned int size, segp, err;
        size = 64; / * (64 x 16) = 1024 bytes * /
        err = _dos_allocmem(size, &segp);
        if (err == 0)
            printf ("Allocated memory at segment: %x\n", segp);
        else {
            perror("Unable to allocate block");
            printf("Maximum no. of paragraphs available is %u\n", segp);
            return 1;
        }
        _dos_freemem(segp);
*/
void test_dos_freemem(void)
{
    unsigned int size;
    unsigned int segp;
    unsigned int result;

    size = 64; /* (64 x 16) = 1024 bytes */

    _dos_allocmem(size, &segp);

    printf("DEBUG: _dos_freemem() segp == 0x%04X\n", segp);

    result = _dos_freemem(segp);

    if ( result == 0 )
    {
        printf("DEBUG: SUCCESS: _dos_freemem() result == %d\n", result);
        printf("DEBUG: _dos_freemem() segp == 0x%04X\n", segp);
    }
    else
    {
        printf("DEBUG: FAILURE: _dos_freemem() result == %d\n", result);
        printf("DEBUG: _dos_freemem() segp == 0x%04X\n", segp);
    }
}
/*
    BCpp31LR, pg 351/366
        #include <stdlib.h> or #include<alloc.h>
        void *malloc(size_t size);
            char *str;
            / * allocate memory for string * /
            if ((str = (char *) malloc(10)) == NULL) {
                printf("Not enough memory to allocate buffer\n");
                exit(1); / * terminate program if out of memory * /
            }
            / * free memory * /
            free(str);
...
...heap...dynamic allocation...variable-sized block of memory...
*/
void test_malloc(void)
{
    size_t size;
    char *MemBlk;

    //size = 32768;  // 32768 bytes, 32 KB, 2048 paragraphs, 0.5 pages
    //size = 1024;
    size = 1;

    MemBlk = (char *) malloc(size);

    printf("DEBUG: test_malloc() MemBlk == 0x%08X\n", MemBlk);

    if ( MemBlk == NULL )
    {
        printf("DEBUG: FAILURE: test_malloc() MemBlk == %lu\n", MemBlk);
        printf("DEBUG: test_malloc() MemBlk == 0x%08X\n", MemBlk);
    }
    else
    {
        printf("DEBUG: SUCCESS: test_malloc() MemBlk == %p\n", MemBlk);
        printf("DEBUG: test_malloc() MemBlk == 0x%08X\n", MemBlk);
    }

    free(MemBlk);
}
/*
    BCpp31LR, pg 155/168
        #include <alloc.h>
        void far *farmalloc(unsigned long nbytes);
            char far *fptr;
            / * allocate memory for the far pointer * /
            fptr = (char far *) farmalloc(10);
            movedata (FP_SEG(str), FP_OFF(str), FP_SEG(fptr), FP_OFF(fptr), strlen(str));
*/
void test_farmalloc(void)
{
    unsigned long nbytes;
    char __FAR *MemBlk;

    //nbytes = 32768;  // 32768 bytes, 32 KB, 2048 paragraphs, 0.5 pages
    //nbytes = 1024;
    //nbytes = 1;
    nbytes = ( ( 32 * 16 ) + 16 );
    //nbytes = 16;
    
    MemBlk = (char __FAR *) farmalloc(nbytes);
    
    printf("DEBUG: MemBlk = (char __FAR *) farmalloc(%lu); [%s, %d]\n", nbytes, __FILE__, __LINE__);
    
    if ( MemBlk == NULL )
    {
        printf("DEBUG: FAILURE: test_farmalloc(%lu) MemBlk == %Fp\n", nbytes, MemBlk);
    }
    else
    {
        printf("DEBUG: SUCCESS: test_farmalloc() MemBlk == %Fp\n", MemBlk);
    }

    farfree(MemBlk);
}
/*
    FP_OFF, FP_SEG
    BCpp31LR, pg 184/

    MK_FP
    BCpp31LR, pg 364/379
        #include <dos.h>
        void far * MK_FP(unsigned seg, unsigned ofs);
            unsigned int far *screen;
            screen = MK_FP(0xB800, 0);
*/
void test_pointers(void)
{
    unsigned int nparas;
    unsigned int sgmt_addr;
    unsigned int result;
    char __FAR * FarPtr1;
    char __FAR * FarPtr2;
    char __FAR * FarPtr3;
    char __FAR * FarPtr4;

    /*
        segment size: 64K, 1024*64, 65536, 0x10000
        MAX WORD (2 bytes) segment size 65535, 0xFFFF, 4095 paragraphs
        ~= AX:0000 - AX:FFFF
        DOS Allocate Memory
            AX = segment address of allocated memory block (MCB + 1 paragraph (16 bytes))
            - each allocation requires a 16 byte overhead for the MCB
                MCB - DOS Memory Control Block Format
                ...MCB chain
        
    */
    nparas = 64; /* (64 x 16) = 1024 bytes */
    nparas = 4095; // 65535 bytes == MAX UNSIGNED INTEGER

    result = _dos_allocmem(nparas, &sgmt_addr);

    printf("DOS Maximum Memory Available: %u Paragraphs, %u Bytes\n", sgmt_addr, (sgmt_addr * 16));

    nparas = sgmt_addr;

    result = _dos_allocmem(nparas, &sgmt_addr);

    if ( result == 0 )
    {
        printf("DEBUG: SUCCESS: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() sgmt_addr == 0x%04X\n", sgmt_addr);
    }
    else
    {
        printf("DEBUG: FAILURE: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() sgmt_addr == 0x%04X\n", sgmt_addr);
    }

    FarPtr1 = SgmtAddr_FarPtr1(sgmt_addr);
    FarPtr2 = SgmtAddr_FarPtr2(sgmt_addr);
    FarPtr3 = SgmtAddr_FarPtr3(sgmt_addr);
    FarPtr4 = MK_FP0(sgmt_addr);

}
/*
    void far * fp => unsigned long fp_addr
    unsigned long fp_addr => void far * fp
    void far * fp => unsigned long fp_addr_sgmt
    void far * fp => unsigned long fp_addr_ofst

    MK_FP(segment_address,0)

*/
char __FAR * SgmtAddr_FarPtr1(unsigned int segment_address)
{
    char __FAR * FarPtr;
    FarPtr = (char __FAR *) ( (long) segment_address << 16 );
    return FarPtr;
}
char __FAR * SgmtAddr_FarPtr2(unsigned int segment_address)
{
    char __FAR * FarPtr;
    long farptr_sgmt;

    farptr_sgmt = ( (long) segment_address << 16 );

    FarPtr = (char __FAR *) (farptr_sgmt | 0);

    return FarPtr;
}
char __FAR * SgmtAddr_FarPtr3(unsigned int segment_address)
{
    char __FAR * FarPtr;
    long farptr_sgmt;
    unsigned int temp;
    
    temp = segment_address;

    FarPtr = (char __FAR *) ( (long) temp << 16 );

    return FarPtr;
}
unsigned int __FAR * make_ui_fptr(unsigned long ul_fptr_address)
{
    unsigned int __FAR *ui_fptr;

    ui_fptr = (unsigned int __FAR *) ul_fptr_address;

    return ui_fptr;
}
void __FAR * MK_FP0(unsigned int sgmt_addr)
{
    return MK_FP(sgmt_addr,0);// ? Nonportable pointer conversion ?
}

/*
        unsigned FP OFF(void far *p);
        unsigned FP SEG(void far *p);
        void far * MK_FP<unsigned seg, unsigned ofs);
    J:\STU\DBWD\BORLANDC\INCLUDE\DOS.H
    Line 474
    #define MK_FP( seg,ofs )( (void _seg * )( seg ) +( void near * )( ofs ))
    #define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
    #define FP_OFF( fp )( (unsigned )( fp ))

    int  inline _Cdecl peek( unsigned __segment, unsigned __offset )  
                            { return( *( (int  far* )MK_FP( __segment, __offset )) ); }

    #define peek( a,b )( *( (int  far* )MK_FP( (a ),( b )) ))

    void inline _Cdecl poke( unsigned __segment, unsigned __offset, int __value )  
                            {( *( (int  far* )MK_FP( __segment, __offset )) = __value ); }

    #define poke( a,b,c )( *( (int  far* )MK_FP( (a ),( b )) ) =( int )( c ))

*/
/*
    J:\STU\DBWD\developc\Animator\Vpaint\COMMON\ptr.c
*/
/* fool C into thinking a long is a pointer */
void * make_ptr(void *pt)
{
	return pt;
}

#if !defined(__TURBOC__)
void * enorm_pointer(void *p)
{
	intptr_t x = (intptr_t)p;
	return (void *) ((x + 1) & ~0x1);
}
#endif /* __TURBOC__ */

unsigned int ptr_offset(int offset, int seg)
{
	return offset;
}

unsigned int ptr_seg(int offset, int seg)
{
	return seg;
}

// Error: Too few parameters in call to 'ptr_seg' in function ptr_next_seg
// Error: Extra parameter in call to make_ptr in function ptr_next_seg
//void * ptr_next_seg(void *p)
//{
//	return make_ptr(0, ptr_seg(p) + 1);
//}

long pt_to_long(unsigned offset, unsigned seg)
{
	long result;

	result = seg;
	result <<= 4;
	result += offset;
	return result;
}

// Error: Type mismatch in parameter 'pt' in call to 'make_ptr' in function long_to_pt
// Error: Extra parameter in call to make_ptr in function long_to_pt
//void * long_to_pt(unsigned long l)
//{
//	unsigned segment, offset;
//
//	offset = (l&15);
//	l >>= 4;
//	segment = l;
//	return make_ptr(offset, segment);
//}
/*
    https://github.com/yzh/yzhack/blob/master/japanese/emalloc.c
*/
/* 正規化されたポインタを作成する */ // JP->EN /* Create a normalized pointer */
static 	void * mk_fp ( unsigned  int seg, unsigned  int off)
{
	unsigned long temp;

	temp = (((unsigned long)seg) << 4) + ((unsigned long)off);
	return (void *)(((temp & 0xffff0L) << 12) + (temp & 0x0000fL));
}
// _s08p16
void __FAR * MK_FAR_0(unsigned int Seg_Address)
{
    //unsigned int Return_Segment;
    //unsigned int Return_Offset;
    //unsigned int Segment_Pointer;
    //Segment_Pointer = Seg_Address;
    //Return_Segment = Segment_Pointer;
    //Return_Offset = 0;
    ///*
    //    _DX = Return_Segment
    //    _AX = Return_Offset
    //    return DX:AX
    //*/
    unsigned int __FAR * segp;
    segp = MK_FP(Seg_Address, 0);
    return segp;
}

// NOTE(JimBalcomb,20220907): moved out to 'NoGit'
//void test_LbxLdEt1(void)
//{
//    unsigned int LBX_EntryData_SgmtAddr;
//    //LbxLdEt1(FONT_FILE_NAME);
//    //LbxLdEt1(g_FontFileName);
//    LBX_EntryData_SgmtAddr = LbxLdEt1("BACKGRND.LBX", 0);
//    printf("DEBUG: LBX_EntryData_SgmtAddr == 0x%08X\n", LBX_EntryData_SgmtAddr);
//}

void test_asciz_character_array(void)
{
    char chracter_array[20];

    strcpy(chracter_array, "BACKGRND");
    strcat(chracter_array, ".LBX");
    
}

void test_SA_Allocate_Space(void)
{
    // sgmt_addr sa_SAMB_data;
    // sa_SAMB_data = SA_Allocate_Space(32);
    // printf("DEBUG: test_SA_Allocate_Space(): sa_SAMB_data == 0x%08X\n", sa_SAMB_data);
    SAMB_ptr pSAMB_data;
    pSAMB_data = SA_Allocate_Space(32);
    printf("DEBUG: test_SA_Allocate_Space(): pSAMB_data == 0x%08X\n", pSAMB_data);
}

void test_SA_Allocate_MemBlk(void)
{
    // SAMB_addr sa_SAMB_data;
    // sa_SAMB_data = SA_Allocate_MemBlk(32);
    // printf("DEBUG: test_SA_Allocate_MemBlk(): sa_SAMB_data == 0x%08X\n", sa_SAMB_data);
    SAMB_ptr pSAMB_data;
    pSAMB_data = SA_Allocate_MemBlk(32);
    dlvfprintf("DEBUG: [%s, %d] pSAMB_head: %Fp\n", __FILE__, __LINE__, pSAMB_data);
}

/*
sub-tests
    CRP_Empty_Exit_Fn2()
    EMM_Startup();
    VGA_SetModeY();
    VGA_DAC_Init();
    SND_Init();
    IN_Init();
    RNG_TimerSeed();
    VGA_Set_DSP_Addr();
*/
void test_Hardware_Init(void)
{
    Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
}

void test_VGA_DAC_Init(void)
{
    VGA_DAC_Init(GAME_FONT_FILE);
}

/*
sub-tests
    GUI_SetWindows()
    MOUSE_Init()
    CRL_Save_RSP()
    CTRL_Clear()
*/
void test_GUI_Init(void)
{
    int Input_Type = 1;
    IN_Init(Input_Type);
}

void test_VGA_SaveCursorArea_RSP(void)
{
    int X_Pos;
    int Y_Pos;
    X_Pos = 158;
    Y_Pos = 100;
    CRL_Save_RSP(X_Pos, Y_Pos);

}

void test_VGA_LoadPalette(void)
{
    //VGA_LoadPalette(0, -1);
}
void test_VGA_DAC_Write(void)
{
    //VGA_DAC_Write();
}

void test_DISK_GetFileSize(void)
{
    char LBX_FileName[20];
    long LBX_FileSize;
    strcpy(LBX_FileName, "MAINSCRN.LBX");

    LBX_FileSize = DOS_GetFileSize(LBX_FileName);

    printf("DEBUG: [%s, %d]: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);

    if (LBX_FileSize != 196511 )
    {
        printf("DEBUG: [%s, %d]: FAILURE: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
    }
    else
    {
        printf("DEBUG: [%s, %d]: SUCCESS: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
    }

}

void test_EMM_Load_LBX_File(void)
{
    // GAME_LoadMainImages(void)
    //   |-> EMM_Load_LBX_1(g_LbxNm_MAINSCRN);
    //       EMM_Load_LBX_1(char *argLbxFileName)
    //         |-> EMM_Load_LBX(argLbxFileName, 1);
    //         int EMM_Load_LBX(char *LbxFileName, int argReserved)
    // EMM_Record g_EMM_Table[];
    char EmmHndlNm[9] = {0};
    int EmmHndlNbr = 0;
    unsigned int LBX_Header_Preamble_EntryCount;
    unsigned int LBX_Header_Preamble_MagSig;
    unsigned int LBX_Header_Preamble_Unknown;
    unsigned int LBX_Header_Preamble_Type;

    EMM_Startup();

    strcpy(EmmHndlNm, "MAINSCRN");

    EmmHndlNbr = -1;
    EmmHndlNbr = EMM_Load_LBX_File(EmmHndlNm, 1);

    if ( EmmHndlNbr == 0 )
    {
        printf("DEBUG: [%s, %d]: FAILURE: EMM_Load_LBX_File()\n", __FILE__, __LINE__);
    }
    else
    {
        printf("DEBUG: [%s, %d]: SUCCESS: EMM_Load_LBX_File()\n", __FILE__, __LINE__);
    }

    EmmHndlNbr = -1;
    EmmHndlNbr =  EMM_CheckHandleOpen(EmmHndlNm);

    if ( EmmHndlNbr == 0 )
    {
        printf("DEBUG: [%s, %d]: FAILURE: EMM_CheckHandleOpen()\n", __FILE__, __LINE__);
    }
    else
    {
        printf("DEBUG: [%s, %d]: SUCCESS: EMM_CheckHandleOpen()\n", __FILE__, __LINE__);
    }

    // TODO(JimBalcomb): add another test for an entry/record that starts past 64 KB
    EMM_Map4(EmmHndlNbr, 0);

    LBX_Header_Preamble_EntryCount = farpeekw(g_EMM_PageFrame_Base_Address, 0);
    LBX_Header_Preamble_MagSig = farpeekw(g_EMM_PageFrame_Base_Address, 2);
    LBX_Header_Preamble_Unknown = farpeekw(g_EMM_PageFrame_Base_Address, 4);
    LBX_Header_Preamble_Type = farpeekw(g_EMM_PageFrame_Base_Address, 6);
    printf("DEBUG: [%s, %d]: LBX_Header_Preamble_EntryCount: 0x%04X\n", __FILE__, __LINE__, LBX_Header_Preamble_EntryCount);
    printf("DEBUG: [%s, %d]: LBX_Header_Preamble_MagSig: 0x%04X\n", __FILE__, __LINE__, LBX_Header_Preamble_MagSig);
    printf("DEBUG: [%s, %d]: LBX_Header_Preamble_Unknown: 0x%04X\n", __FILE__, __LINE__, LBX_Header_Preamble_Unknown);
    printf("DEBUG: [%s, %d]: LBX_Header_Preamble_Type: 0x%04X\n", __FILE__, __LINE__, LBX_Header_Preamble_Type);

    if ( (LBX_Header_Preamble_EntryCount != 0x0006) || (LBX_Header_Preamble_MagSig != 0xFEAD) || (LBX_Header_Preamble_Unknown != 0x0000) || (LBX_Header_Preamble_Type != 0x0000) )
    {
        printf("DEBUG: [%s, %d]: FAILURE: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
        printf("DEBUG: [%s, %d]: FAILURE: LBX_Header_Preamble_EntryCount: %d;  LBX_Header_Preamble_MagSig: %d;  LBX_Header_Preamble_Unknown: %d;  LBX_Header_Preamble_Type: %d;\n", __FILE__, __LINE__, (LBX_Header_Preamble_EntryCount != 0x006), (LBX_Header_Preamble_MagSig != 0xFEAD), (LBX_Header_Preamble_Unknown != 0x0000), (LBX_Header_Preamble_Type != 0x0000));
    }
    else
    {
        printf("DEBUG: [%s, %d]: SUCCESS: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
    }

}

// void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
/*
    Prerequisites:
        ST_VGA.H
        gsa_PaletteLbxEntry
        gsa_Palette

        _s14p03c.c
        void VGA_DAC_Init(char *PaletteLbxFileName)
        ...
        gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes

*/

// void test_VGA_LoadPalette(void)
// {
//     g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
//     EMM_SetMinKB(EMM_MIN_KB);
//     RAM_SetMinKB(RAM_MIN_KB);
//     Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'

//     VGA_LoadPalette(0, -1, 0);

// }

void test_SimTexGameEngineFramework(void)
{
    Debug_Log_Startup();

    // main()
    // g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    // EMM_SetMinKB(EMM_MIN_KB);
    // RAM_SetMinKB(RAM_MIN_KB);
    // Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
    //     |-> EMM_Startup()
    //     |-> VGA_SetModeY()
    //     |-> VGA_DAC_Init()
    //         |-> VGA_TextDraw_Init()
    //     |-> SND_Init()
    //     |-> IN_Init()
    //     |-> RNG_TimerSeed()
    //     |-> VGA_Set_DSP_Addr()
    // LBX_Tables_Init(6100);   _o57p01  void LBX_Tables_Init(int gfx_buff_nparas)
    //      |-> ?
    //      |-> ...
    // GUI_SetEscOverride();
    // VGA_LoadPalette(0, -1, 0);
    // VGA_DAC_Write();
    VGA_SetModeY();         // _s15p01.asm  ST_VGA.H
    VGA_Set_DSP_Addr();     // _s26p02.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_RenderScreenPage
    // ...
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_Set_DSP_Addr();
    VGA_PageFlip();
        // |-> VGA_WaitSync()
        // |-> VGA_Set_DSP_Addr()


    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_Set_DSP_Addr();

    // while ! KBD_CheckBuffer()
    getch();

    VGA_PageFlip();

    getch();

    VGA_DrawFilledRect(0, 0, 319, 199, 2);
    VGA_PageFlip();
    getch();

    VGA_DrawFilledRect(0, 0, 319, 199, 1);
    VGA_PageFlip();
    getch();

    VGA_DrawFilledRect(4, 4, 315, 195, 4);
    VGA_PageFlip();
    getch();

    VGA_DrawFilledRect(42, 42, 209, 89, 5);
    VGA_PageFlip();
    getch();

    // // takes @@YayDivFour, and crashes ... maybe because LODSB for VGA_Left/RightBits, but no _DS = dseg/DGROUP - module scope var?
    // VGA_DrawFilledRect(64, 64, 68, 68, 11);
    // VGA_PageFlip();
    // getch();

    VGA_DrawFilledRect(0, 14,  4, 19, 8);
    VGA_DrawFilledRect(0, 30,  8, 35, 8);
    VGA_DrawFilledRect(0, 46, 12, 51, 8);
    VGA_DrawFilledRect(0, 62, 16, 67, 8);
    VGA_PageFlip();
    getch();

    // //void 4Y_Set_Pixel(int x, int y, unsigned char color)
    // Y4_Set_Pixel(0, 0, 7);
    // Y4_Set_Pixel(1, 0, 7);
    // Y4_Set_Pixel(0, 1, 7);
    // Y4_Set_Pixel(1, 1, 7);
    // VGA_PageFlip();
    // getch();

    // Quit()
    VGA_SetTextMode();

    Debug_Log_Shutdown();
}

void test_FLIC_Draw_XY(void)
{
    Debug_Log_Startup();

    // Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
    //     |-> EMM_Startup()
    //     |-> VGA_SetModeY()
    //     |-> VGA_DAC_Init()
    //         |-> VGA_TextDraw_Init()
    //     |-> SND_Init()
    //     |-> IN_Init()
    //     |-> RNG_TimerSeed()
    //     |-> VGA_Set_DSP_Addr()
    VGA_SetModeY();         // _s15p01.asm  ST_VGA.H
    VGA_Set_DSP_Addr();     // _s26p02.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_RenderScreenPage
    // ...
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    VGA_Set_DSP_Addr();
    // ...
    // VGA_LoadPalette(0, -1, 0);
    // VGA_DAC_Write();
    // ...
    // VGA_PageFlip();
    //     // |-> VGA_WaitSync()
    //     // |-> VGA_Set_DSP_Addr()

    VGA_DrawFilledRect(0, 0, 319, 199, 180);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    getch();
    VGA_PageFlip();
    getch(); 

    // ST_VGA.H  g_PaletteLbxFileName <-| VGA_DAC_Init(); <-| Hardware_Init() <-| main()
    //     VGA_DAC_Init(argFontFileName);
    //         strcpy(g_PaletteLbxFileName, PaletteLbxFileName);
    //         gsa_FontStyleData = LBXE_LoadSingle(PaletteLbxFileName, 0);
    //         gsa_BorderStyleData = LBXE_LoadSingle(PaletteLbxFileName, 1);
    //         gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes
    //         gsa_Palette = SA_Allocate_Space(64);               // 64 paragraphcs = 64 * 16 bytes = 1024 bytes
    //         gsa_PaletteFlags = gsa_Palette + 48;               // 48 paragaphs = 48 * 16 = 768

    strcpy(g_PaletteLbxFileName, GAME_FONT_FILE);
    gsa_PaletteLbxEntry = FP_SEG(SA_Allocate_Space(348));
    gsa_Palette = FP_SEG(SA_Allocate_Space(64));
    gsa_PaletteFlags = gsa_Palette + 48;
    //  main()  line 140
    VGA_LoadPalette(0, -1, 0);
    //     gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);
    //     gsa_Palette_Font_Colors = gsa_gsa_PaletteLbxEntry +  48;  //  48 pr  768 b   0x0300  16 arrays of 16 colors
    //     UU_gsa_Palette_Data     = gsa_Palette_Font_Colors +  16;  //  16 pr  256 b   0x0400  ? UnUsed ? DNE in MoO1 ?
    //     gsa_Palette_Cursor_Data = UU_gsa_Palette_Data     +  16;  //  16 pr  256 b   0x0500  16 16x16 cursor bitmap images
    //     gsa_ShadingColors       = gsa_Palette_Cursor_Data + 256;  // 256 pr 4096 b  0x1500  5,376 bytes array of 24 color fractions (B-G-R-Percent)
    //     gsa_Palette[] = gsa_gsa_PaletteLbxEntry[]
    //     gsa_PaletteFlags[] = 1;
    //  main()  line 141
    VGA_DAC_Write();

    getch();

    // // // GAME_LoadMainImages()
    // //      MGC_DEF.H   g_LbxNm_MAINSCRN
    // //      gsa_MAINSCRN_0_AnimatedLogo
    // gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    // // ? VGA_SetOutlineColor(0); ?
    // SCREEN_Menu() line 73
    VGA_LoadPalette(2, -1, 0); // ARCANUS - Magic Castle View
    // FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);

    //  GAME_LoadMainImages()
    //      MGC_DEF.H
    //      gsa_VORTEX_1_MenuContinue = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    //      gsa_VORTEX_2_MenuHallOfFame = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    //      gsa_VORTEX_5_MenuLoadGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);
    //      gsa_VORTEX_4_MenuNewGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    //      gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    //      gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);

    //  GAME_LoadMainImages()
    //      MGC_DEF.H   g_LbxNm_MAINSCRN
    //      gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
        // if ( Moused_Control != GUI_Quit_Lbl_Index )
        // {
        //     FLIC_SetFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
        // }
        // else
        // {
        //     FLIC_ResetFrame(gsa_VORTEX_3_MenuQuitToDOS);
        // }
    //  GAME_LoadMainImages()
    gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    //  SCREEN_Menu() line 73
    VGA_LoadPalette(2, -1, 0); // ARCANUS - Magic Castle View
    FLIC_SetFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
    //FLIC_ResetFrame(gsa_VORTEX_3_MenuQuitToDOS);
    FLIC_Draw_XY(123, (141 + 48), gsa_VORTEX_3_MenuQuitToDOS);

    getch();

    //  GUI_Redraw_WaitTime()
    //  GUI_Redraw_WaitOne()
    //      VGA_DAC_Write()
    //      SCRN_SimplePageFlip()

    VGA_PageFlip();

    getch();

    VGA_DAC_Write();

    getch();

    // // EMPERATO
    // // LOADSAVE
    // // ARCANUS
    // // WIZLAB
    // // TEST
    // // BACKGRND
    // // LOSE

    // VGA_LoadPalette(0, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(1, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(2, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(3, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(4, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(5, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(6, -1, 0);
    // VGA_DAC_Write();
    // getch();

    // Quit()
    VGA_SetTextMode();

    Debug_Log_Shutdown();
}


void test_Screen_MainMenu(void)
{
    char VGA_DAC_Filename0[] = "VGADAC00.BIN";
    char VGA_DAC_Filename1[] = "VGADAC01.BIN";
    char VGA_DAC_Filename2[] = "VGADAC02.BIN";
    char VGA_DAC_Filename3[] = "VGADAC03.BIN";
    char VGA_DAC_Filename4[] = "VGADAC04.BIN";
    char VGA_DAC_FilenameN[] = "VGADAC0N.BIN";
    unsigned int itr_gsa_Palette;
    unsigned int ofstPaletteFlags;
    unsigned int itr_ofstPaletteFlags;
    unsigned char _FAR *ptr_Palette;

    Debug_Log_Startup();

    STU_VGA_DAC_Dump(VGA_DAC_Filename0);

    // ptr_Palette = (unsigned char _FAR *)MK_FP(gsa_Palette, 0);
    // for ( itr_gsa_Palette = 0; itr_gsa_Palette < 1024; itr_gsa_Palette++ )
    // {
    //     dlvfprintf("ptr_Palette[%u]: 0x%02X %u\n", itr_gsa_Palette, ptr_Palette[itr_gsa_Palette], ptr_Palette[itr_gsa_Palette]);
    // }
    // ofstPaletteFlags = 768;
    // for ( itr_ofstPaletteFlags = 0; itr_ofstPaletteFlags < 256; itr_ofstPaletteFlags++ )
    // {
    //     dlvfprintf("ptr_Palette[%u]: 0x%02X %u\n", itr_ofstPaletteFlags, ptr_Palette[itr_ofstPaletteFlags], ptr_Palette[itr_ofstPaletteFlags]);
    // }

    VGA_SetModeY();         // _s15p01.asm  ST_VGA.H
    VGA_Set_DSP_Addr();     // _s26p02.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_RenderScreenPage
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // _s16p01.asm  ST_VGA.H    // g_DrawScreenPage_SgmtAddr, g_VGA_LeftBits, g_VGA_RightBits
    VGA_Set_DSP_Addr();

    strcpy(g_PaletteLbxFileName, GAME_FONT_FILE);
    gsa_PaletteLbxEntry = FP_SEG(SA_Allocate_Space(348));
    gsa_Palette = FP_SEG(SA_Allocate_Space(64));
    gsa_PaletteFlags = gsa_Palette + 48;
    VGA_LoadPalette(0, -1, 0);
    VGA_DAC_Write();

    STU_VGA_DAC_Dump(VGA_DAC_Filename1);

    /*
        END: ~=main()
    */

    // // // GAME_LoadMainImages()
    // //      MGC_DEF.H   g_LbxNm_MAINSCRN
    // //      gsa_MAINSCRN_0_AnimatedLogo
    // // ? VGA_SetOutlineColor(0); ?
    // SCREEN_Menu() line 73
    VGA_LoadPalette(2, -1, 0);
    ptr_Palette = (unsigned char _FAR *)MK_FP(gsa_Palette, 0);
    for ( itr_gsa_Palette = 0; itr_gsa_Palette < 768; itr_gsa_Palette++ )
    {
        dlvfprintf("ptr_Palette[itr_gsa_Palette=%u]: 0x%02X %u\n", itr_gsa_Palette, ptr_Palette[itr_gsa_Palette], ptr_Palette[itr_gsa_Palette]);
    }
    ofstPaletteFlags = 768;
    for ( itr_ofstPaletteFlags = 0; itr_ofstPaletteFlags < 256; itr_ofstPaletteFlags++ )
    {
        dlvfprintf("ptr_Palette[%u + %u]: 0x%02X %u\n", ofstPaletteFlags, itr_ofstPaletteFlags, ptr_Palette[ofstPaletteFlags + itr_ofstPaletteFlags], ptr_Palette[ofstPaletteFlags + itr_ofstPaletteFlags]);
    }
    
    VGA_DAC_Write();

    ofstPaletteFlags = 768;
    for ( itr_ofstPaletteFlags = 0; itr_ofstPaletteFlags < 256; itr_ofstPaletteFlags++ )
    {
        dlvfprintf("ptr_Palette[%u + %u]: 0x%02X %u\n", ofstPaletteFlags, itr_ofstPaletteFlags, ptr_Palette[ofstPaletteFlags + itr_ofstPaletteFlags], ptr_Palette[ofstPaletteFlags + itr_ofstPaletteFlags]);
    }

    STU_VGA_DAC_Dump(VGA_DAC_Filename2);
    
    //  GAME_LoadMainImages()
    //      MGC_DEF.H
    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    gsa_VORTEX_1_MenuContinue = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    gsa_VORTEX_2_MenuHallOfFame = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    gsa_VORTEX_5_MenuLoadGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);
    gsa_VORTEX_4_MenuNewGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);

    // NOTE: MAINSCRN, 0 is the only one that has a FLIC_Header.Palette_Data_Offset
    // NOTE: None have shading/replacement colors

    //  SCREEN_Menu_Draw()
    VGA_Set_DSP_Addr();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);

    FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);
    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);

    FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);

    // FLIC_SetFrame(gsa_VORTEX_5_MenuLoadGame, 1);
    FLIC_ResetFrame(gsa_VORTEX_5_MenuLoadGame);
    FLIC_Draw_XY(123, (141 + 12), gsa_VORTEX_5_MenuLoadGame);

    FLIC_ResetFrame(gsa_VORTEX_1_MenuContinue);
    // FLIC_SetFrame(gsa_VORTEX_1_MenuContinue, 1);
    FLIC_Draw_XY(123, (141 + (12 * 0)), gsa_VORTEX_1_MenuContinue);

    FLIC_ResetFrame(gsa_VORTEX_4_MenuNewGame);
    // FLIC_SetFrame(gsa_VORTEX_4_MenuNewGame, 1);
    FLIC_Draw_XY(123, (141 + 24), gsa_VORTEX_4_MenuNewGame);

    FLIC_ResetFrame(gsa_VORTEX_2_MenuHallOfFame);
    // FLIC_SetFrame(gsa_VORTEX_2_MenuHallOfFame, 1);
    FLIC_Draw_XY(123, (141 + 36), gsa_VORTEX_2_MenuHallOfFame);

    FLIC_ResetFrame(gsa_VORTEX_3_MenuQuitToDOS);
    // FLIC_SetFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
    FLIC_Draw_XY(123, (141 + 48), gsa_VORTEX_3_MenuQuitToDOS);

    getch();

    //  GUI_Redraw_WaitTime()
    //  GUI_Redraw_WaitOne()
    //      VGA_DAC_Write()
    //      SCRN_SimplePageFlip()
    
    VGA_PageFlip();

    getch();

    VGA_DAC_Write();

    STU_VGA_DAC_Dump(VGA_DAC_Filename3);

    getch();

    // // EMPERATO
    // // LOADSAVE
    // // ARCANUS
    // // WIZLAB
    // // TEST
    // // BACKGRND
    // // LOSE

    // VGA_LoadPalette(0, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(1, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(2, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(3, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(4, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(5, -1, 0);
    // VGA_DAC_Write();
    // getch();
    // VGA_LoadPalette(6, -1, 0);
    // VGA_DAC_Write();
    // getch();

    // Quit()
    VGA_SetTextMode();

    STU_VGA_DAC_Dump(VGA_DAC_FilenameN);

    Debug_Log_Shutdown();
}

// test_SCREEN_Menu_Draw
void test_SCREEN_Menu(void)
{
    unsigned int input_control_index;
    
    // ? main()

    // ? GAME_MainMenu()

    /*
        BEGIN: SCREEN_Menu()
    */
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_Set_DSP_Addr();
    VGA_LoadPalette(2, -1, 0);
    //g_GAME_ValidSaveCount
    g_GAME_HaveContSave = ST_TRUE;
    g_GAME_HaveSaves = ST_TRUE;
    CTRL_Clear();
    //Continue_Move_Down = ST_FALSE;
    // GUI_Continue_Label = GUI_CreateClickLabel(108, (138 + (12 * Continue_Move_Down)), 211, (149 + (12 * Continue_Move_Down)), 0, -1);
    // GUI_NewGame_Label = GUI_CreateClickLabel(108, 162, 211, 173, 0, -1);
    // GUI_HoF_Lbl_Index = GUI_CreateClickLabel(108, 174, 211, 185, 0, -1);
    // GUI_Quit_Lbl_Index = GUI_CreateClickLabel(108, 186, 211, 199);
    // Continue_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_C, -1);
    // Load_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_L, -1);
    // NewGame_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_N, -1);
    // HallofFame_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_H, -1);
    // Quit_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_Q, -1);
    // Escape_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_Esc, -1);
    GUI_SetWindows(1, g_GUI_MainMenuWindow);
    CRL_Save_RSP(MD_GetX(), MD_GetY());
    FLIC_ResetFrame(gsa_MAINSCRN_0_AnimatedLogo);
    SCRN_Set_Redraw_Function(SCREEN_Menu_Draw, 2);
    IN_Set_Skip(4);
//    while ( flag_done == ST_FALSE )
    CLK_SaveCounter();  // 1oom :: ui_delay_prepare()
    input_control_index = IN_GetInput();  // 1oom :: oi1 = uiobj_handle_input_cond();
//    if ( flag_done == ST_FALSE )
    SCREEN_Menu_Draw();
    SCRN_SimplePageFlip();  // 1oom :: uiobj_finish_frame();
    // j_VGA_Fade_In()
    VGA_Copy_RSP_DSP();  // Research(JimBalcomb)): 20220117: what does this accomplish?
    CLK_Wait(2);  // 1oom :: ui_delay_ticks_or_click(2);
//    }  /* while ( flag_done == ST_FALSE ) */
    SCRN_DisableRedraw();
    HLP_ClearHelp();
//    return g_GUI_MainMenuSelected;

    /*
        END: SCREEN_Menu()
    */

}
