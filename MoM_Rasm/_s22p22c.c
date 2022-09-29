// _s22p22a.c MEM_Copy_Far
// ST_DEF.H

#include "ST_HEAD.H"

#include "STU_DBG.H"

/*
    ?!? ~== movemem() / movedata() !?!

    Microsoft C:
        #include <memory.h>               Required only for function declarations
        void movedata( unsigned int src_sgmt, unsigned int src_ofst, unsigned int dst_sgmt, unsigned int dst_ofst, unsigned int count );
            src_sgmt                            Segment address of source
            src_ofst                            Segment offset of source
            dst_sgmt                           Segment address of destination
            dst_ofst                           Segment offset of destination
            count                             Number of bytes
    Borland C++:
        #include <mem.h>
        void movedata(unsigned src_sgmt, unsigned src_ofst, unsigned dstseg, unsigned dstoff, size_t n);
        movedata is a means of moving blocks of data that is independent of memory model.
        movedata is meant to be used to move far data in small data programs. In large model programs, you can use memcpy instead.
        #include <mem.h>
        void movmem(void *src, void *dest, unsigned length);
        J:\STU\DBWD\BORLANDC\CRTL\CLIB\MOVMEM.CAS
        J:\STU\DBWD\BORLANDC\CRTL\CLIB\MOVEDATA.CAS

XREF: (MGC)
    EMM_Sandbox2VGAFILEH
    EMM_VGAFILEH2Sandbox
    FLIC_BuildFrame
    GUI_DisplayHelp
    NEWG_CreateEZs
    UU_HLP_DrawExpanding
    UU_LBX_IMG_LoadPalette
    UU_LBX_Image_Copy
    UU_VGA_LoadDAC
    UU_VGA_RestorePalette
    UU_VGA_SavePalette
    VGA_DrawLBXImage
    VGA_WndDrawLBXImage

Far Pointer vs. Sgmt,Ofst
    e.g., 
         FLIC_BuildFrame()
            mov  ax, 16
            push ax                              ; nbytes
            push [bp+FlicHdr_SgmtAddr]
            xor  ax, ax
            push ax                              ; fptr_Src
            xor  ax, ax
            push ax
            lea  ax, [bp+Img_Header]
            push ax                              ; fptr_Dst
            call MEM_Copy_Far
        ...
            MEM_Copy_Far(&Img_Header, 0, 0, FlicHdr_SgmtAddr, 16)
            FlicHdr_SgmtAddr contains the segment address of a SAMB_data, which is the result of FP_SEG(farmalloc())
        UU_VGA_LoadDAC()
            mov  si, [bp+First_Color]
            mov  di, [bp+Color_Count]
            mov  ax, di
            mov  dx, 3
            imul dx
            push ax                              ; nbytes
            push [bp+Data_Seg]
            push [bp+Source]                    ; fptr_Src
            push [gsa_Palette]                   ; 40h LBX_Alloc_Space paragraphs
            mov  ax, si
            mov  dx, 3
            imul dx
            push ax                              ; fptr_Dst
            call ST_MoveData
        ...
            ST_MoveData( (First_Color * 3), gsa_Palette, Source, Data_Seg, (Color_Count * 3) )
    ...
    So, seems to be oviously, conclusively Sgmt,Ofst, not Far Pointer
    That said, the order being ofst,sgmt would actually suggest a Far Pointer.
    Not sure if/how you'd see MK_FP() being used directly in the functional call parameters.
*/


int ST_MoveData(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt, unsigned int nbytes)
{
    unsigned int LoopCount;
    unsigned int itrLoopCount;
    unsigned char _FAR *fptrDestByte;
    unsigned char _FAR *fptrSrcByte;
    unsigned int _FAR *fptrDestWord;
    unsigned int _FAR *fptrSrcWord;

    if ( nbytes == 0 ) { return ST_FAILURE; }
    if ( dst_sgmt == 0 ) { dst_sgmt = _DS; }
    if ( src_sgmt == 0 ) { src_sgmt = _DS; }

    if ( (nbytes & 0x01) == 1 )
    {
        fptrDestByte = (unsigned char _FAR *) MK_FP(dst_sgmt, dst_ofst);
        fptrSrcByte = (unsigned char _FAR *) MK_FP(src_sgmt, src_ofst);
        LoopCount = nbytes;
        for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        {
            fptrDestByte[itrLoopCount] = fptrSrcByte[itrLoopCount];
        }
    }
    else
    {
        fptrDestWord = (unsigned int _FAR *) MK_FP(dst_sgmt, dst_ofst);
        fptrSrcWord = (unsigned int _FAR *) MK_FP(src_sgmt, src_ofst);
        LoopCount = (nbytes / 2 );
        for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        {
            fptrDestWord[itrLoopCount] = fptrSrcWord[itrLoopCount];
        }

    }
    return ST_SUCCESS;
}
