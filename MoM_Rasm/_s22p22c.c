// _s22p22a.c MEM_Copy_Far
// ST_DEF.H

#include "ST_HEAD.H"

#include "STU_DBG.H"

/*
    ?!? ~== movemem() / movedata() !?!

    Microsoft C:
        #include <memory.h>               Required only for function declarations
        void movedata( unsigned int srcseg, unsigned int srcoff, unsigned int destseg, unsigned int destoff, unsigned int count );
            srcseg                            Segment address of source
            srcoff                            Segment offset of source
            destseg                           Segment address of destination
            destoff                           Segment offset of destination
            count                             Number of bytes
    Borland C++:
        #include <mem.h>
        void movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg, unsigned dstoff, size_t n);
        movedata is a means of moving blocks of data that is independent of memory model.
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
    UU_HLP_DrawExpanding+1
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


int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes)
{
    unsigned int LoopCount;
    unsigned int itrLoopCount;
    unsigned char _FAR *fptrDestByte;
    unsigned char _FAR *fptrSrcByte;
    unsigned int _FAR *fptrDestWord;
    unsigned int _FAR *fptrSrcWord;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: ST_MoveData(destoff=0x%04X, destseg=0x%04X, srcoff=0x%04X, srcseg=0x%04X, nbytes=%u)\n", __FILE__, __LINE__, destoff, destseg, srcoff, srcseg, nbytes);
#endif


    if ( nbytes == 0 )
    {
        return ST_FAILURE;
    }

    if ( destseg == 0 )
    {
//        HERE("destoff = _DS");
        destseg = _DS;
    }
    if ( srcseg == 0 )
    {
//        HERE("srcseg = _DS");
        srcseg = _DS;
    }

    if ( (nbytes & 0x01) == 1 )
    {
//        HERE("( (nbytes & 0x01) == 1 )");
        fptrDestByte = (unsigned char _FAR *) MK_FP(destseg, destoff);
        fptrSrcByte = (unsigned char _FAR *) MK_FP(srcseg, srcoff);
        LoopCount = nbytes;
        for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        {
            //*fptrDestByte++ = *fptrSrcByte++;
            fptrDestByte[itrLoopCount] = fptrSrcByte[itrLoopCount];
        }
    }
    else
    {
//        HERE("( (nbytes & 0x01) != 1 )");
        // // fptrDestWord = (unsigned int _FAR *) MK_FP(destseg, destoff);
        // // fptrSrcWord = (unsigned int _FAR *) MK_FP(srcseg, srcoff);
        // // LoopCount = (nbytes / 2);
        // // for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        // // {
        // //     *fptrDestWord++ = *fptrSrcWord++;
        // // }
        // fptrDestByte = (unsigned char _FAR *) MK_FP(destseg, destoff);
        // fptrSrcByte = (unsigned char _FAR *) MK_FP(srcseg, srcoff);
        // dlvfprintf("DEBUG: [%s, %d] fptrDestByte: %Fp\n", __FILE__, __LINE__, fptrDestByte);
        // dlvfprintf("DEBUG: [%s, %d] fptrSrcByte: %Fp\n", __FILE__, __LINE__, fptrSrcByte);
        // LoopCount = nbytes;
        // for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        // {
        //     dlvfprintf("DEBUG: [%s, %d] fptrSrcByte[%u]: 0x%02X\n", __FILE__, __LINE__, itrLoopCount, fptrSrcByte[itrLoopCount]);
        //     //*fptrDestByte++ = *fptrSrcByte++;
        //     fptrDestByte[itrLoopCount] = fptrSrcByte[itrLoopCount];
        //     dlvfprintf("DEBUG: [%s, %d] fptrDestByte[%u]: 0x%02X\n", __FILE__, __LINE__, itrLoopCount, fptrDestByte[itrLoopCount]);
        // }
        fptrDestWord = (unsigned int _FAR *) MK_FP(destseg, destoff);
        fptrSrcWord = (unsigned int _FAR *) MK_FP(srcseg, srcoff);
//        dlvfprintf("DEBUG: [%s, %d] fptrDestWord: %Fp\n", __FILE__, __LINE__, fptrDestWord);
//        dlvfprintf("DEBUG: [%s, %d] fptrSrcWord: %Fp\n", __FILE__, __LINE__, fptrSrcWord);
        LoopCount = (nbytes / 2 );
        for ( itrLoopCount = 0; itrLoopCount < LoopCount; itrLoopCount++ )
        {
            // dlvfprintf("DEBUG: [%s, %d] fptrSrcWord[%u]: 0x%04X\n", __FILE__, __LINE__, itrLoopCount, fptrSrcWord[itrLoopCount]);
            //*fptrDestWord++ = *fptrSrcWord++;
            fptrDestWord[itrLoopCount] = fptrSrcWord[itrLoopCount];
            // dlvfprintf("DEBUG: [%s, %d] fptrDestWord[%u]: 0x%04X\n", __FILE__, __LINE__, itrLoopCount, fptrDestWord[itrLoopCount]);
        }

    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: ST_MoveData(destoff=0x%04X, destseg=0x%04X, srcoff=0x%04X, srcseg=0x%04X, nbytes=%u)\n", __FILE__, __LINE__, destoff, destseg, srcoff, srcseg, nbytes);
#endif

    return ST_SUCCESS;
}
