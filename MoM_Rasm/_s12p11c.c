// _s12p11a.c EMM_MapFourPages

#include <DOS.H>

#include "ST_HEAD.H"

#include "ST_EMM.H"

// NOTE: ST had not error-checking at all - which is handy, cause this does a lot of mapping invalid logical pages

void EMM_Map4(int EmmHandle, int EmmLogicalPage)
{
    // int tmp_Error;
    // unsigned char tmp_EMS_STATUS;
    int tmp_EmmLogicalPage;
    int tmp_EmmPhysicalPage;
//    printf("DEBUG: [%s, %d]: BEGIN: EMM_Map4(EmmHandle = %d, EmmLogicalPage = %d)\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage);

    // tmp_Error = 0;
    // tmp_EMS_STATUS = 0;

    tmp_EmmPhysicalPage = 0;
    tmp_EmmLogicalPage = EmmLogicalPage;
    // _AL = 0;
    // _BX = EmmLogicalPage + 0;
    _AL = tmp_EmmPhysicalPage;
    _BX = tmp_EmmLogicalPage;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    // // if ( _AH != 0 )
    // tmp_EMS_STATUS = _AH;
    // if ( tmp_EMS_STATUS != 0 )
    // {
    //     // // printf("DEBUG: [%s, %d]: FAILURE: EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage + 0);
    //     // printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, EmmLogicalPage + 0);
    //     printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmPhysicalPage = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, tmp_EmmPhysicalPage, tmp_EmmLogicalPage);
    //     tmp_Error = 1;
    // }
    tmp_EmmLogicalPage++;
    tmp_EmmPhysicalPage++;
    // _AL = 1;
    // _BX = EmmLogicalPage + 1;
    _AL = tmp_EmmPhysicalPage;
    _BX = tmp_EmmLogicalPage;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    // // if ( _AH != 0 )
    // tmp_EMS_STATUS = _AH;
    // if ( tmp_EMS_STATUS != 0 )
    // {
    //     // // printf("DEBUG: [%s, %d]: FAILURE: EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage + 1);
    //     // printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, EmmLogicalPage + 1);
    //     printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmPhysicalPage = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, tmp_EmmPhysicalPage, tmp_EmmLogicalPage);
    //     tmp_Error = 1;
    // }
    tmp_EmmLogicalPage++;
    tmp_EmmPhysicalPage++;
    // _AL = 2;
    // _BX = EmmLogicalPage + 2;
    _AL = tmp_EmmPhysicalPage;
    _BX = tmp_EmmLogicalPage;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    // // if ( _AH != 0 )
    // tmp_EMS_STATUS = _AH;
    // if ( tmp_EMS_STATUS != 0 )
    // {
    //     // // printf("DEBUG: [%s, %d]: FAILURE: EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage + 2);
    //     // printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, EmmLogicalPage + 2);
    //     printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmPhysicalPage = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, tmp_EmmPhysicalPage, tmp_EmmLogicalPage);
    //     tmp_Error = 1;
    // }
    tmp_EmmLogicalPage++;
    tmp_EmmPhysicalPage++;
    // _AL = 3;
    // _BX = EmmLogicalPage + 3;
    _AL = tmp_EmmPhysicalPage;
    _BX = tmp_EmmLogicalPage;
    _DX = EmmHandle;
    _AH = EMS_MAPPAGE;
    geninterrupt(EMS_INT);
    // // if ( _AH != 0 )
    // tmp_EMS_STATUS = _AH;
    // if ( tmp_EMS_STATUS != 0 )
    // {
    //     // // printf("DEBUG: [%s, %d]: FAILURE: EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage + 3);
    //     // printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, EmmLogicalPage + 3);
    //     printf("DEBUG: [%s, %d]: FAILURE: tmp_EMS_STATUS = 0x%02X EmmHandle = %d, EmmPhysicalPage = %d, EmmLogicalPage = %d\n", __FILE__, __LINE__, tmp_EMS_STATUS, EmmHandle, tmp_EmmPhysicalPage, tmp_EmmLogicalPage);
    //     //printf("DEBUG: [%s, %d]: FAILURE: EMS_STATUS: %s\n", __FILE__, __LINE__, EMS_STATUS_Message[EMS_STATUS_Code[tmp_EMS_STATUS]]);
    //     tmp_Error = 1;
    // }

    // if ( tmp_Error != 0 )
    //     Quit("EMM_Map4: Page mapping failed");
    
//    printf("DEBUG: [%s, %d]: END: EMM_Map4()\n", __FILE__, __LINE__);
}
