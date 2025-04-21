


current_menu_screen AKA auto_input_field_idx



MAGIC.EXE *specific*
...some shared/overlapped with WIZARDS.EXE



Main Menu
Load
Hall of Fame
New Game



## screen_change

// DNE in WZD
// MGC dseg:64AE
int16_t screen_change;

Up   w Menu_Screen_Control+54    mov     [screen_changed], e_ST_TRUE                                           
Up   w Menu_Screen_Control+82    mov     [screen_changed], e_ST_FALSE                                          
Up   w Main_Menu_Screen+46       mov     [screen_changed], e_ST_TRUE                                           
Up   r Main_Menu_Screen+4C       cmp     [screen_changed], e_ST_FALSE                                          
Up   r Main_Menu_Screen+487      cmp     [screen_changed], e_ST_FALSE                                          
Up   w Main_Menu_Screen+4A3      mov     [screen_changed], e_ST_FALSE                                          
Up   r Main_Menu_Screen_Draw+86  cmp     [screen_changed], e_ST_FALSE                                          
Down w Load_Screen+D6            mov     [screen_changed], e_ST_FALSE                                          
Down w Load_Screen+EA            mov     [screen_changed], e_ST_TRUE                                           
Down r Load_Screen+3A9           cmp     [screen_changed], e_ST_FALSE    ; in WZD, uses IDK_Load_Screen_Fade_In
Down w Newgame_Screen_0__WIP+8   mov     [screen_changed], e_ST_TRUE                                           
Down r Newgame_Screen_0__WIP+E   cmp     [screen_changed], e_ST_NULL                                           
Down r Newgame_Screen_0__WIP+3FB cmp     [screen_changed], e_ST_FALSE                                          




## current_menu_screen
on Main Menu, in Main_Menu_Screen(), set to the *selected* menu item
for New Game,
    Newgame_Screen_0_Draw__WIP()
        current_menu_screen = Auto_Input();
        if(current_menu_screen != newgame_difficulty_button_field)
        if(current_menu_screen != newgame_opponents_button_field)
        if(current_menu_screen != newgame_landsize_button_field)
        if(current_menu_screen != newgame_magic_button_field)

Up   r Load_Screen_Draw+191       mov     ax, [current_menu_screen]                
Up   r Load_Screen_Draw+1C2       mov     ax, [current_menu_screen]                
Up   w Load_Screen_Draw+2F        mov     [current_menu_screen], ax                
Up   r Load_Screen_Draw:loc_3E0F3 mov     ax, [current_menu_screen]                
Up   w Main_Menu_Screen+32        mov     [current_menu_screen], e_ST_UNDEFINED    
Up   w Main_Menu_Screen+3BE       mov     [current_menu_screen], e_ST_UNDEFINED    
Up   w Main_Menu_Screen+415       mov     [current_menu_screen], scr_Continue      
Up   w Main_Menu_Screen+429       mov     [current_menu_screen], scr_Load_Screen   
Up   w Main_Menu_Screen+43D       mov     [current_menu_screen], scr_NewGame_Screen
Up   w Main_Menu_Screen+456       mov     [current_menu_screen], 3                 
Up   w Main_Menu_Screen+46A       mov     [current_menu_screen], scr_HoF_Screen    
Up   r Main_Menu_Screen+4C4       mov     ax, [current_menu_screen]                
Up   r Main_Menu_Screen:loc_13C5A cmp     [current_menu_screen], e_ST_UNDEFINED    
     w Newgame_Screen_0_Draw+136  mov     [current_menu_screen], ax                
Down r Newgame_Screen_0_Draw+148  mov     ax, [current_menu_screen]                
Down r Newgame_Screen_0_Draw+226  mov     ax, [current_menu_screen]                
Down r Newgame_Screen_0_Draw+304  mov     ax, [current_menu_screen]                
Down r Newgame_Screen_0_Draw+3E2  mov     ax, [current_menu_screen]                
Down r SCRN_Draw_NewScr5+1C9      mov     ax, [current_menu_screen]                
Down r SCRN_Draw_NewScr5+209      mov     ax, [current_menu_screen]                
Down w SCRN_Draw_NewScr5+31       mov     [current_menu_screen], ax                

Load_Screen_Draw()
    current_menu_screen = Auto_Input();
auto_input_field_idx



Main_Menu_Screen_Control()
    main_menu_selection = Main_Menu_Screen();
        switch(main_menu_selection)
        {
            case 0:
            {
                // Stop_Music__STUB();
                // j_GAME_WizardsLaunch(8);
            } break;
            case 1:
            {
                // j_GAME_LoadSaveScreen();
            } break; 
            case 2:
            {
                // j_GAME_New_Create();
            } break;
            case 3:
            {
                quit_flag = ST_TRUE;
            } break;
            case 4:
            {
                // j_GAME_Hall_of_Fame();
                // j_Init_Credits();
            } break;
        }
        // IDK_previous_screen = e_ST_UNDEFINED;
