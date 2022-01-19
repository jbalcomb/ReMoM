// _s12p08c.c EMM_MapnRead
// ST_EMM.H

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_EMM.H"

/*
    segment:offset  ->  EMM Logical Page
        asm mov ax, [Src_Ofst]
        asm mov dx, [Src_Sgmt]
        asm shl ax, 1
        asm rcl dx, 1
        asm shl ax, 1
        asm rcl dx, 1
        asm mov bx, dx
        asm mov [EmmLogicalPage], bx
        asm shr ax, 1
        asm shr ax, 1
        asm mov si, ax

    e.g., EMM_LBX_EntryLoader -> EMM_MapnRead(LbxFileType, 0, 6, 0, 2, EmmHandle);
    Src_Ofst = 6
    Src_Sgmt = 0
    .:. EMM Logical Page = 0
    SHL 11111111  ==> 11111110 & CF=1
    SHL 01111111  ==> 11111110 & CF=0
    RCL 11111111 & CF=0 ==> 11111110 & CF=1
    RCL 11111111 & CF=1 ==> 11111111 & CF=1
    RCL 01111111 & CF=0 ==> 11111110 & CF=0
    RCL 01111111 & CF=1 ==> 11111111 & CF=0
    .:.
    `:` DX:AX == segment:offset
    SHL AX ; RCL DX
    SHL AX 11111111  ==> 11111110 & CF=1
    RCL DX 00000000  ==> 00000001 & CF=0
    ~= if AX >= 10000000 then AX = (AX * 2) and DX = (DX + 1)
                (32,768)                             (paragraph)
    ...
    Now, why twice? Because 2*32768 = 65536 (64 K) (4 EMM Pages)?

        DX:AX  00000000 00000000 : 00000000 00000000
                                ^^ < ^^
                                    00000000 = 0000     0
                                    01000000 = 4000 16384
                                    10000000 = 8000 32768
                                    11000000 = C000 49152 = (32768 + 16384)
                                00 = 0
                                01 = 1
                                10 = 2
                                11 = 3
    ? In-Game Example ?
    ? need an entry that starts after 64 KB in a LBX that uses EMM_Load_LBX_File()?
    20211217: NOTE(JimBalcomb): MAINSCRN.LBX, Entry Index 5 triggers this, with '0002:5910' (I'm assuming 6 would as well)

                                        EduMat:
                                                J:\STU-EduMat\Asm\rotate\174l7.pdf
                                                https://web.archive.org/web/20200626222103/http://mathforum.org/dr.math/faq/faq.peasant.html
                                                J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_PM.C
                                                    PML_MapEMS(page / PMEMSSubPage,emspage);
                                                    ...
                                                    #define	PMEMSSubPage	(EMSPageSize / PMPageSize)
                                                    #define	EMSPageSize		16384
                                                    #define	PMPageSize		4096
                                                http://qbrpgs.blarg.ca/qbchronicles/issue4.html
                                                    Page = DataRead& \ 16384            'Determine the EMS page and
                                                    Offset = DataRead& - Page * 16384&  '  offset to load the data into.
                                                    Page = Page + (Slot - 1) * 4        'Adjust the page depending on which slot we're using.
                                                    ...
                                                    DataRead& + BufferSize
                                                    DataRead& = 0                       'We haven't read any data yet...
                                                    BufferSize = 1024
                                                    ...
                                                    So how do we know which logical pages to map to the page frame? Well, because each sound slot is 4 pages, we can use:
                                                    EMS.MapXPages 0, (Slot - 1) * 4, 4, Handle
                                                    (We have to subtract 1 from the slot number because the sound slots are numbered starting at 1, while the EMS pages are numbered starting from 0.)
                                                    SEE: Figure 2-4: Mapping the sound to play to the page frame

*/

// TODO(JimBalcomb): rename 'Src_Sgmt' - it's not quite right, as the actual 'Source Segment' is always g_EMM_PageFrame_Base_Address

void EMM_MapnRead(unsigned int Dst_Ofst, unsigned int Dst_Sgmt, unsigned int Src_Ofst, unsigned int Src_Sgmt, int nbytes, int EmmHandle)
{
    int EmmLogicalPage;
    unsigned int tmp_Src_Ofst;
    unsigned int tmp_Src_Sgmt;
    unsigned char _FAR *fptr_Dst;
    unsigned char _FAR *fptr_Src;
    int itr_nbytes;
    unsigned int bkup_DS;
    // int tmp_Error;
    // unsigned char tmp_EMS_STATUS;

    bkup_DS = _DS;  // IDK - JIC

    // NOTE: printf() clobbers AX, CX, and DX
    //dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_MapnRead(Dst_Ofst = 0x%04X, Dst_Sgmt = 0x%04X, Src_Ofst = 0x%04X, Src_Sgmt = 0x%04X, nbytes = %d, EmmHandle = %d)\n", __FILE__, __LINE__, Dst_Ofst, Dst_Sgmt, Src_Ofst, Src_Sgmt, nbytes, EmmHandle);
    
    // tmp_Error = 0;
    // tmp_EMS_STATUS = 0;

    // If a Destination SEGMENT is not specified, assume it is the DATA SEGMENT
    if ( Dst_Sgmt == 0 )
    {
        Dst_Sgmt = _DS;
        //Dst_Sgmt = _DATA;
        //Dst_Sgmt = DGROUP;
    }

    // /*
    //     Shift-Left, twice, Src_Sgmt
    //     Set bits 1 & 0 in Src_Sgmt, based on bits 16 & 15 in Src_Ofst
    // */
    // if ( Src_Ofst & 0xC000 )        // 1100000000000000
    // {
    //     dlvfprintf("DEBUG: [%s, %d]: bit #16 or bit #15 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //     // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0003    // 0000000000000011
    //     if ( ( Src_Ofst & 0x8000 ) && ( Src_Ofst & 0x4000 ) )        // 1100000000000000
    //     {
    //         dlvfprintf("DEBUG: [%s, %d]: bit #16 or bit #15 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //         // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0003    // 0000000000000011
    //     }
    //     else if ( Src_Ofst & 0x8000 )   // 1000000000000000
    //     {
    //         dlvfprintf("DEBUG: [%s, %d]: bit #16 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //         // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0002    // 0000000000000010
    //     }
    //     else if ( Src_Ofst & 0x4000 )   // 0100000000000000
    //     {
    //         dlvfprintf("DEBUG: [%s, %d]: bit #15 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //         // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0001    // 0000000000000001
    //     }
    //     // if DEBUG
    //     // getch();
    // }

    tmp_Src_Sgmt = Src_Sgmt;
    tmp_Src_Sgmt = (tmp_Src_Sgmt << 2);
    if ( Src_Ofst & 0x8000 )                        // bit #16 of Src_Ofst
    {
        tmp_Src_Sgmt = (tmp_Src_Sgmt & 0x0002);     // bit #2 of Src_Sgmt
    }
    if ( Src_Ofst & 0x4000 )                        // bit #15 of Src_Ofst
    {
        tmp_Src_Sgmt = (tmp_Src_Sgmt & 0x0001);    // bit #1 of Src_Sgmt
    }
    EmmLogicalPage = tmp_Src_Sgmt;
    // printf("DEBUG: [%s, %d]: Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, Src_Sgmt);
    // printf("DEBUG: [%s, %d]: tmp_Src_Sgmt: 0x%04X\n", __FILE__, __LINE__, tmp_Src_Sgmt);
    // printf("DEBUG: [%s, %d]: EmmLogicalPage: 0x%04X\n", __FILE__, __LINE__, EmmLogicalPage);
    
    _AL = 0;
    _BX = EmmLogicalPage + 0;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    _AL = 1;
    _BX = EmmLogicalPage + 1;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    _AL = 2;
    _BX = EmmLogicalPage + 2;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    _AL = 3;
    _BX = EmmLogicalPage + 3;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    // // if ( _AH != 0 )
    // tmp_EMS_STATUS = _AH;
    // if ( tmp_EMS_STATUS != 0 )
    // {
    //     // printf("DEBUG: [%s, %d]: FAILURE: EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage + 3);
    //     printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, EmmLogicalPage + 3);
    //     tmp_Error = 1;
    // }
    // if ( tmp_Error != 0 )
    //     Quit("EMM_Map4: Page mapping failed");
    

    // printf("DEBUG: [%s, %d]: g_EMM_PageFrame_Base_Address: 0x%08X\n", __FILE__, __LINE__, g_EMM_PageFrame_Base_Address);

    /*
    _ES = Dst_Sgmt;
    _DI = Dst_Ofst;
    _DS = g_EMM_PageFrame_Base_Address;
    _SI = Src_Ofst;
    */
    fptr_Dst = (unsigned char _FAR *) MK_FP(Dst_Sgmt, Dst_Ofst);
    fptr_Src = (unsigned char _FAR *) MK_FP(g_EMM_PageFrame_Base_Address, Src_Ofst);
    // printf("DEBUG: [%s, %d]: fptr_Dst: %Fp\n", __FILE__, __LINE__, fptr_Dst);
    // printf("DEBUG: [%s, %d]: fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);

    for ( itr_nbytes = 0; itr_nbytes < nbytes; itr_nbytes++ )
    {
        //printf("DEBUG: [%s, %d]: fptr_Dst[%d]: %d (0x%02X)\n", __FILE__, __LINE__, itr_nbytes, fptr_Dst[itr_nbytes], fptr_Dst[itr_nbytes]);
        //printf("DEBUG: [%s, %d]: fptr_Src[%d]: %d (0x%02X)\n", __FILE__, __LINE__, itr_nbytes, fptr_Src[itr_nbytes], fptr_Src[itr_nbytes]);
        // // *fptr_Dst++ = *fptr_Src++;
        fptr_Dst[itr_nbytes] = fptr_Src[itr_nbytes];
        //printf("DEBUG: [%s, %d]: fptr_Dst[%d]: %d (0x%02X)\n", __FILE__, __LINE__, itr_nbytes, fptr_Dst[itr_nbytes], fptr_Dst[itr_nbytes]);
    }

    //dlvfprintf("DEBUG: [%s, %d]: END: EMM_MapnRead(Dst_Ofst = 0x%04X, Dst_Sgmt = 0x%04X, Src_Ofst = 0x%04X, Src_Sgmt = 0x%04X, nbytes = %d, EmmHandle = %d)\n", __FILE__, __LINE__, Dst_Ofst, Dst_Sgmt, Src_Ofst, Src_Sgmt, nbytes, EmmHandle);
}
