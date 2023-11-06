
Unit Statistics Window



Main Screen
Unit Window
Rick-Click Unit Picture

City Screen
Unit Window

Outpost Screen
Unit Window

ArmyList Screen
Unit List
Hero Portrait

Item Screen
Hero Portrait

Production Screen
Unit
Building

Combat



Unit Window
"Right-clicking on a unit displays the unit's statistics (see: Basic Unit Information)."

Main_Screen()
    BEGIN: Right-Click Unit Window Grid Field
        USW_FullDisplay()
            UNIT_Create_BURecord(unit_idx, BattleUnit)
            USW_LoadAndShow()
                GFX_Swap_Overland()
                USW_MemAlloc()
                USW_LoadFigureImage()  ||  IMG_USW_HeroPortrt = LBX_Reload_Next()
                UnitStatPup_Draw()
                GFX_Swap_Cities()



Unit Statistics Window


Main_Screen()
    USW_FullDisplay()

knows it is the index for the _unit_stack
knows the coordinates and dimensions for the 'Unit Window'
passes x1, y1, x2, y2 of screen coordinates
hard-codes x_start, y_start
passes unit_idx  (for _UNITS[], from _unit_stack[])

The call to _UNIT_Create_BURecord() only uses the unit_idx.

The call to USW_LoadAndShow() includes the unit_idx and hard-codes the 'View Type' to 1


USW_LoadAndShow()

    sets USW_In_Combat to ST_FALSE
    resets, marks, and releases _screen_seg
    swaps in USW/UV graphics, swaps back Main/City graphics
    calls Unit_Statistics_Popup()


Main_Screen()
    USW_FullDisplay()
        USW_LoadAndShow()
            Unit_Statistics_Popup(x_start, y_start, x1, y1, x2, y2, uv_type, unit_idx)
                HERE:
                    {x1, y1, x2, y2} and unit_idx came from Main_Screen()
                    {x_start, y_start} came from USW_FullDisplay(), hard-coded to {31, 6}
                    view_type came from USW_FullDisplay(), hard-coded to 1
                    USW_In_Combat has been set to ST_FALSE

_DI_ViewTypeFlag = ViewTypeFlag

    USW_DrawLeft = x_start;
    USW_DrawTop = y_start;
    USW_ViewType = ViewTypeFlag; // ; 2 if combat
    USW_Unit_Index = unit_idx;
    USW_Effect_Count = 0;

defaults to ST_TRUE for allowing left-clicks on the Effect List Items
disables it if View Type is 2
int16_t Enable_Remove_Unit_Enchantment;  /* T/F - if T, match left-clicks to unit ability/effect input fields */


USW_In_Combat == ST_FALSE
    j_HLP_DrawExpanding()

uv_type != 2
    OK button

uv_type == 1
    Dismiss button

uv_type != 2
    side-box (dismiss and ok button area)
    ¿ not test for input field match ?



rename
_DI_ViewTypeFlag
uv_type




mov     ax, offset UV_Effect_Count
push    ax                              ; Count@
push    [UV_Effect_List@]               ; Buffer@
push    [UV_Unit_Index]                 ; Unit_Index
call    j_UV_Build_Effect_List          ; creates a unit statistics window effect list

j_UV_Build_Effect_List()







USW_MemAlloc()
    GUI_String_1@ = Near_Allocate_First(100);
    USW_List_Structure@ = Near_Allocate_Next(1520);
    USW_ItemDraw_Seg = Allocate_Next_Block(_screen_seg, 30);





USW_LoadAndShow()
OON XREF:
    j_USW_LoadAndShow()

j_USW_LoadAndShow()
OON XREF:
    USW_FullDisplay()

USW_FullDisplay()
OON XREF:
    j_USW_FullDisplay()

j_USW_FullDisplay()
XREF:
    City_Screen+F2E     call    j_USW_FullDisplay               ; a double wrapper for USW_Display that does all the
    Main_Screen+A3F     call    j_USW_FullDisplay               ; a double wrapper for USW_Display that does all the
    ArmyList_Screen+441 call    j_USW_FullDisplay               ; a double wrapper for USW_Display that does all the
    ArmyList_Screen+612 call    j_USW_FullDisplay               ; a double wrapper for USW_Display that does all the
    Items_Screen+1F3    call    j_USW_FullDisplay               ; a double wrapper for USW_Display that does all the
    Outpost_Screen+39A  call    j_USW_FullDisplay               ; a double wrapper for USW_Display that does all the


USW_MemAlloc()
OON XREF:
    j_USW_MemAlloc()

j_USW_MemAlloc()
XREF:
    USW_CombatDisplay:loc_5E206     call    j_USW_MemAlloc                  ; resets the near allocation buffer, creates a 100
    USW_LoadAndShow+2B              call    j_USW_MemAlloc                  ; resets the near allocation buffer, creates a 100
    Unit_Statistics_Popup:loc_5E73C call    j_USW_MemAlloc                  ; resets the near allocation buffer, creates a 100
    Unit_Statistics_Popup+58B       call    j_USW_MemAlloc                  ; resets the near allocation buffer, creates a 100
    STK_CaptureCity+8               call    j_USW_MemAlloc                  ; resets the near allocation buffer, creates a 100
    USW_LoadHireScreen+25           call    j_USW_MemAlloc                  ; resets the near allocation buffer, creates a 100
