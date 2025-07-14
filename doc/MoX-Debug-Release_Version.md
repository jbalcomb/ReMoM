

Release_Version()
Check_Release_Version()



## release_flag
XREF:
    F11_Key__WIP:loc_1E469              cmp     [release_flag], e_ST_TRUE
    Screen_Flic_Capture__STUB:loc_1E484 cmp     [release_flag], e_ST_TRUE
    Release_Version:loc_1E905           mov     [release_flag], e_ST_TRUE
    Check_Release_Version+3             mov     ax, [release_flag]       

## Release_Version()
    release_flag = ST_TRUE
OON XREF:
    _main+2C3 call    Release_Version

## Check_Release_Version()

XREF:
    Allocation_Error+B        call    Check_Release_Version
    ST_PSTRM+B                call    Check_Release_Version
    ST_Disp_PSTR_Wrapper+3    call    Check_Release_Version
    ST_PSTRS+3                call    Check_Release_Version
    ST_PSTR+6                 call    Check_Release_Version
    ST_PSTRL+6                call    Check_Release_Version
    ST_PSTRU+6                call    Check_Release_Version
    Interpret_Mouse_Input+D3  call    Check_Release_Version
    Interpret_Mouse_Input+F7  call    Check_Release_Version
    Init_Overland+1B8         call    Check_Release_Version
    PreInit_Overland+17       call    Check_Release_Version
    Main_Screen+3F8           call    Check_Release_Version
    Main_Screen+443           call    Check_Release_Version
    Main_Screen+465           call    Check_Release_Version
    Main_Screen+473           call    Check_Release_Version
    Main_Screen+48B           call    Check_Release_Version
    Cast_Spell_Target_Error+3 call    Check_Release_Version



MoO2
Module: capture
    signed integer (2 bytes) release_flag
        Address: 02:0017C374
    function (0 bytes) Screen_Flic_Capture
        Address: 01:001101F0
            Num params: 0
            Return type: void (1 bytes) 
    function (0 bytes) B_Key
        Address: 01:00110AFC
            Num params: 0
            Return type: void (1 bytes) 
            void (1 bytes) 
    function (0 bytes) Release_Version
        Address: 01:00110B34
            Num params: 0
            Return type: void (1 bytes) 
    function (0 bytes) Check_Release_Version
        Address: 01:00110B5C
            Num params: 0
            Return type: signed integer (2 bytes) 



jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM (current)
$ grep -I " Release_Version(" -r
doc/MoM-Init.md:MGC:    Release_Version(); j_MoM_Tables_Init(6100); Set_Global_Escape()
doc/MoM-Init.md:WZD:    Release_Version(); Set_Global_ESC(); j_MoM_Init_Tables(4600);
doc/MoM_main.c:    Release_Version();
doc/MoM_main_MGC.c:    Release_Version();
doc/MoM_main_WZD.c:    Release_Version();
doc/MoX-Input-InterpretMouseInput-Keyboard.md:Enable_Cancel() is called in main(), right after Release_Version()
src/MOX/DBG.c:// MoO2  Module: capture  Release_Version()  Address: 01:00110B34
src/MOX/DBG.c:void Release_Version(void)
src/MOX/DBG.h:void Release_Version(void);
src/ReMoM.c:    Release_Version();
src/win_MoM.cpp:    Release_Version();

jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM (current)
$ grep -I Check_Release_Version -r
doc/MoX-SimtexGameEngine.md:    Check_Release_Version()
doc/MoX-SimtexGameEngine.md:Allocation_Error() |-> Check_Release_Version()
src/AISPELL.c:    if(Check_Release_Version() == ST_TRUE)
src/LoadScr.c:    if(Check_Release_Version() == ST_FALSE)
src/LoadScr.c:    if(!Check_Release_Version())
src/MainScr.c:        /* Alt-A   */  /* if(input_field_idx == hotkey_idx_Alt_A) {if(Check_Release_Version()==ST_FALSE){DBG_Alt_A_State=1-DBG_Alt_A_State;}} */
src/MainScr.c:        /* Alt-N   */  /* if(input_field_idx == hotkey_idx_Alt_A) {if(Check_Release_Version()==ST_FALSE){DBG_ShowTileInfo=1-DBG_ShowTileInfo;}} */
src/MOX/Allocate.c:#include "DBG.h"  /* Check_Release_Version() */
src/MOX/Allocate.c:    if(Check_Release_Version() == ST_TRUE)
src/MOX/DBG.c:// MoO2  Module: capture  Check_Release_Version()  Address: 01:00110B5C
src/MOX/DBG.c:int Check_Release_Version(void)
src/MOX/DBG.h:int Check_Release_Version(void);
src/MOX/Fonts.c:MoO2 moved the Check_Release_Version() to Disp_PSTR(), removing it from all the wrapper functions.
src/MOX/Fonts.c:MoO2 does not have the function that wraps Disp_PSTR() in Check_Release_Version().
src/MOX/Fonts.c:    if(Check_Release_Version() != ST_TRUE)
src/MOX/Fonts.c:    if(Check_Release_Version() != ST_TRUE)
src/MOX/Fonts.c:    if(Check_Release_Version() != ST_TRUE)
src/MOX/Fonts.c:    if(Check_Release_Version() != ST_TRUE)
src/MOX/Fonts.c:    if(Check_Release_Version() != ST_TRUE)
src/MOX/Fonts.c:    if(Check_Release_Version() != ST_TRUE)
src/MOX/Input.c:        if((character == '`') && (Check_Release_Version() == ST_FALSE))
src/MOX/Input.c:        if((character == '~') && (Check_Release_Version() == ST_FALSE))
src/MOX/MOX_Lib.h:// #include "MOX_DBG.h"    /* Check_Release_Version() */
