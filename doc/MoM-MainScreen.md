
/*
    WIZARDS.EXE
    ovr057
    ovr058
    ovr059
    ovr061
    ovr062
    ovr063
    ovr064
    ovr095
    ovr097

¿ MoO2  Module: SHIPSTK ?

*/



## Data Segments  {Strings, Initialized, Uninitialized}

dseg:C03E                                                 ¿ BEGIN: Main_Screen - Uninitialized Data ?
...
dseg:C098                                                     ¿ BEGIN: List / ArmyList / CityList - Uninitialized Data ?









Screen_Control()
    case scr_Main_Screen: { Main_Screen(); } break;
But, ...
    ¿ need everything from MGC & WZD, from main() through New_Game()/Load_Game() to Main_Screen() ?



        case scr_Main_Screen:
        {
            // BEGIN: WZD main()
            // Load_SAVE_GAM(8)
            // Load_WZD_Resources()
            Load_Palette(0, -1, 0);  // NOTE(JimBalcomb,20230111): this is the only Load_Palette() leading to the Main_Screen()
            // Calculate_Remap_Colors();
            Set_Button_Down_Offsets(1, 1);
            // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1)
            Apply_Palette();
            // Fade_In()
            // Clear_Fields()
            // Loaded_Game_Update_WZD()
            // END: WZD main()

            // IDK  Set_Outline_Color(0);
            // IDK  Set_Alias_Color(0);
            // IDK  Set_Font_Style_Shadow_Down(0, 0, 0, 0);

            Set_Page_Off();
            Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 7);
            Set_Page_On();
            Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 5);
            Set_Page_Off();

            Main_Screen();
            // MoO2  previous_screen = scr_Main_Screen
            previous_screen = scr_Main_Screen;
        } break;
