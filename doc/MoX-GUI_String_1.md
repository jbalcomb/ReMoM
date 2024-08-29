


GUI_String_1@




Down w CMB_ShowInfoWindow+6D                 mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w CMB_SetNearAllocs+78                  mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w Hall_Of_Fame_Screen+8D                mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w City_Built_Building_Message+3A        mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w GAME_ReturnDialog+20                  mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w GAME_OVER+C6                          mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w Hero_LevelUp_Popup+124                mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w WIZ_Conquest+115                      mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
     w USW_MemAlloc+D                        mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down w Surveyor_Screen:loc_7A411             mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Road_Build_Screen:loc_5BA32           mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Enemy_City_Screen_Load+10             mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w ArmyList_Screen_Load+50               mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Item_Screen_Load+F                    mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w IDK_AdvsrScr_Astrologer+11            mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w CityList_Screen_Load+27               mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w IDK_AdvsrScr_Historian:loc_654E2      mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w IDK_Cartographer_Screen+12            mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Magic_Screen_Load_Pictures+E          mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w IDK_MirrorScreen_s6343B+12            mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Outpost_Screen_Load+10                mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Production_Screen_Load+D              mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w IDK_CityScr_RsrcBrkdn+18              mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w Unit_List_Window_Pup+4F               mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   w City_Screen_Load+10                   mov     [GUI_String_1@], ax             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down r CMB_DrawFullScreen+3FB                mov     ax, [GUI_String_1@]             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down r CMB_DrawFullScreen:loc_810FE          mov     bx, [GUI_String_1@]             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down r CMB_DrawFullScreen:loc_810D3          mov     bx, [GUI_String_1@]             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down r CMB_DrawFullScreen+3A8                mov     bx, [GUI_String_1@]             ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Down r sub_7AD24+1BB                         push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r Unit_Statistics_Popup+709             push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r Unit_Statistics_Popup+719             push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r Unit_Statistics_Popup+6C8             push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r Unit_Statistics_Popup+699             push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r Unit_Statistics_Popup+512             push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r Unit_Statistics_Popup:loc_5EA49       push    [GUI_String_1@]                 ; 100 LBX_NearAlloc_First bytes or 20 _Next in combat
Up   r City_Built_Building_Message+22F       push    [GUI_String_1@]                 ; Msg@                                               
Up   r Magic_Screen+32B                      push    [GUI_String_1@]                 ; Msg@                                               
Up   r Enemy_City_Screen+3E4                 push    [GUI_String_1@]                 ; Msg@                                               
Up   r ITEM_Dismantle+104                    push    [GUI_String_1@]                 ; Msg@                                               
Up   r City_Screen__WIP+E49                  push    [GUI_String_1@]                 ; Msg@                                               
Up   r City_Screen__WIP+87E                  push    [GUI_String_1@]                 ; Msg@                                               
Up   r City_Screen__WIP+C8A                  push    [GUI_String_1@]                 ; Msg@                                               
Up   r City_Screen__WIP+9A1                  push    [GUI_String_1@]                 ; Msg@                                               
Up   r City_Screen__WIP+BE5                  push    [GUI_String_1@]                 ; Msg@                                               
Up   r Move_Item+32A                         push    [GUI_String_1@]                 ; Msg@                                               
Up   r Outpost_Screen+1D9                    push    [GUI_String_1@]                 ; Msg@                                               
Up   r Production_Screen+2C5                 push    [GUI_String_1@]                 ; Msg@                                               
Up   r Production_Screen+384                 push    [GUI_String_1@]                 ; Msg@                                               
Up   r ArmyList_Screen_Draw+624              push    [GUI_String_1@]                 ; String                                             
Up   r CityList_Screen_Draw+753              push    [GUI_String_1@]                 ; String                                             
Up   r Draw_Item_With_Name+49                push    [GUI_String_1@]                 ; String@                                            
Up   r IDK_CityScr_RsrcBrkdn+14C             push    [GUI_String_1@]                 ; base_seg                                           
Up   r IDK_CityScr_RsrcBrkdn+1BB             push    [GUI_String_1@]                 ; base_seg                                           
Up   r IDK_CityScr_RsrcBrkdn+22A             push    [GUI_String_1@]                 ; base_seg                                           
Up   r IDK_CityScr_RsrcBrkdn+298             push    [GUI_String_1@]                 ; base_seg                                           
Up   r IDK_CityScr_RsrcBrkdn+DD              push    [GUI_String_1@]                 ; base_seg                                           
Up   r sub_5BDCE+118                         push    [GUI_String_1@]                 ; dest                                               
Up   r sub_5BDCE+130                         push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+70A              push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+67F        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+19C        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+1C5        push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+6EA              push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+1EE        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+217        push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+6DB              push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+240        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+32A        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+353        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+37C        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+3A5        push    [GUI_String_1@]                 ; dest                                               
Up   r sub_5BDCE+162                         push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+62D        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+3CE        push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+665              push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+503        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+538        push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+245          push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+254          push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+263          push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+221                      push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+1FD                      push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+99           push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+A8           push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+1CF          push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+B7           push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+C6           push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+D5           push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+1B6          push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+1A7          push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest:loc_7918E                push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+1EE                      push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+181          push    [GUI_String_1@]                 ; dest                                               
Down r Hero_LevelUp_Popup_Draw+116           push    [GUI_String_1@]                 ; dest                                               
Down r Hall_Of_Fame_Screen_Draw+172          push    [GUI_String_1@]                 ; dest                                               
Down r GAME_ReturnDialog+3E                  push    [GUI_String_1@]                 ; dest                                               
Down r GAME_ReturnDialog+2F                  push    [GUI_String_1@]                 ; dest                                               
Down r Hero_LevelUp_Popup_Draw+125           push    [GUI_String_1@]                 ; dest                                               
Up   r sub_5BDCE+102                         push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+1B9                      push    [GUI_String_1@]                 ; dest                                               
Down r GAME_Draw_WIN_Anim+E7                 push    [GUI_String_1@]                 ; dest                                               
Down r Hero_LevelUp_Popup_Draw+28D           push    [GUI_String_1@]                 ; dest                                               
Down r GAME_Draw_WIN_Anim+CF                 push    [GUI_String_1@]                 ; dest                                               
Down r GAME_Draw_WIN_Anim+B7                 push    [GUI_String_1@]                 ; dest                                               
Down r Hero_LevelUp_Popup_Draw+29C           push    [GUI_String_1@]                 ; dest                                               
Down r Hero_LevelUp_Popup_Draw+2AB           push    [GUI_String_1@]                 ; dest                                               
Down r GAME_Draw_WIN_Anim+9F                 push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+1AA                      push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn:loc_875F1        push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+643                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+622                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+5D4                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+562                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+374                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+355                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+346                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+2E5                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+2C6                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+2B7                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+22E                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+208                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+1F9                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+1A6                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+192                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+183                push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen_Draw+798                 push    [GUI_String_1@]                 ; dest                                               
Up   r Production_Screen+379                 push    [GUI_String_1@]                 ; dest                                               
Up   r Production_Screen+2BA                 push    [GUI_String_1@]                 ; dest                                               
Up   r Outpost_Screen_Draw+1A4               push    [GUI_String_1@]                 ; dest                                               
Up   r Outpost_Screen_Draw+173               push    [GUI_String_1@]                 ; dest                                               
Up   r Outpost_Screen+1CE                    push    [GUI_String_1@]                 ; dest                                               
Up   r Outpost_Screen+1BF                    push    [GUI_String_1@]                 ; dest                                               
Up   r Outpost_Screen+19F                    push    [GUI_String_1@]                 ; dest                                               
Up   r Move_Item+31F                         push    [GUI_String_1@]                 ; dest                                               
Up   r Move_Item+310                         push    [GUI_String_1@]                 ; dest                                               
Up   r Move_Item+2ED                         push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+83D                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+814                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+805                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+7F6                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+7E7                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+71D                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+666                push    [GUI_String_1@]                 ; dest                                               
Up   r Mirror_Screen_Draw+657                push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+170                      push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+18C                      push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+19B                      push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+AB1        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+A88        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+A5F        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+A36        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+A0E        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+924        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+8FB        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+8D2        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+8A9        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+880        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+857        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+82E        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+805        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+6D1        push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw+6A8        push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen_Draw+789                 push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen_Draw+75F                 push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen_Draw+6CB                 push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen_Draw+695                 push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen_Draw+63B                 push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen+320                      push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen+311                      push    [GUI_String_1@]                 ; dest                                               
Up   r Magic_Screen+2D3                      push    [GUI_String_1@]                 ; dest                                               
Up   r ITEM_Dismantle+EA                     push    [GUI_String_1@]                 ; dest                                               
Up   r ITEM_Dismantle+DB                     push    [GUI_String_1@]                 ; dest                                               
Up   r ITEM_Dismantle+BB                     push    [GUI_String_1@]                 ; dest                                               
Up   r ITEM_Dismantle+AC                     push    [GUI_String_1@]                 ; dest                                               
Up   r ITEM_Dismantle+7B                     push    [GUI_String_1@]                 ; dest                                               
Down r IDK_Surveyor_s7A761+269               push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+138                      push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+147                      push    [GUI_String_1@]                 ; dest                                               
Down r WIZ_Conquest+161                      push    [GUI_String_1@]                 ; dest                                               
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4E06F  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+272            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+263            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+241            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+23             push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+22B            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+1F3            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+1E4            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+1B3            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+15B            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+12D            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message:loc_49A77 push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+D5        push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+95        push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+86        push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+60        push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+159       push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+14A       push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+11C       push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+987                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+873                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+864                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+855                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+846                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+830                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+821                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+812                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+7E8                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+FF             push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+12B                         push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+63             push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+13A                         push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+3C5            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+32             push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+149                         push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+2CB            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+2BC            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen_Draw2__WIP+2AD            push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+251                         push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawInfoWindow+1B5                push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawInfoWindow+1A6                push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+260                         push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawInfoWindow+175                push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawInfoWindow+166                push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawFullScreen+35D                push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawActiveUnitW:loc_814F2         push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw:loc_5926F        push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+2BB                         push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+2FB                         push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+57                          push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw+5DE              push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw+5C5              push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw+5AC              push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+8C                          push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw+54C              push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw+32               push    [GUI_String_1@]                 ; dest                                               
Up   r ArmyList_Screen_Draw+1B               push    [GUI_String_1@]                 ; dest                                               
Up   r City_Built_Building_Message+10D       push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+172                         push    [GUI_String_1@]                 ; dest                                               
Up   r CityList_Screen_Draw+64E              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+181                         push    [GUI_String_1@]                 ; dest                                               
Up   r CityList_Screen_Draw+3CA              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+190                         push    [GUI_String_1@]                 ; dest                                               
Up   r CityList_Screen_Draw+389              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+E4                          push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+F3                          push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+127                         push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+13                          push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+169                         push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+192                         push    [GUI_String_1@]                 ; dest                                               
Up   r CityList_Screen_Draw+18F              push    [GUI_String_1@]                 ; dest                                               
Up   r CityList_Screen_Draw+178              push    [GUI_String_1@]                 ; dest                                               
Up   r Cartographer_Screen_Draw+215          push    [GUI_String_1@]                 ; dest                                               
Up   r Cartographer_Screen_Draw+206          push    [GUI_String_1@]                 ; dest                                               
Down r CMB_DrawUnitDisplay:loc_86662         push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+1D6                         push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+38E              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+37F              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_5BDCE:loc_5BF21                   push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+27C              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+253              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+244              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+619              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+235              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+226              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+163              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+154              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+145              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+136              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+628              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+127              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_655C4+18E                         push    [GUI_String_1@]                 ; dest                                               
Up   r sub_655C4+1B4                         push    [GUI_String_1@]                 ; dest                                               
Up   r sub_655C4+1C3                         push    [GUI_String_1@]                 ; dest                                               
Up   r Enemy_City_Screen_Draw+181            push    [GUI_String_1@]                 ; dest                                               
Up   r Enemy_City_Screen_Draw+150            push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+5AD              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_5BDCE+178                         push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+58B              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+546              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+5EC              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+537              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+5FB              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+528              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+508              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+4F9              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_5BDCE+187                         push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+4B0              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+48A              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+47B              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+46C              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+45D              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+60A              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+3E4              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+3BB              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+3AC              push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+39D              push    [GUI_String_1@]                 ; dest                                               
Up   r Draw_Unit_List_Window_Pup+190         push    [GUI_String_1@]                 ; dest                                               
Up   r Draw_Unit_List_Window_Pup+177         push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP:loc_4878B            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP:loc_48526            push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+E34                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+E25                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+DF4                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+DE5                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+DB4                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+C7F                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+C70                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+C4E                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+C3F                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+C0E                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+BDA                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+B57                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+B48                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+B17                  push    [GUI_String_1@]                 ; dest                                               
Up   r City_Screen__WIP+996                  push    [GUI_String_1@]                 ; dest                                               
Up   r Enemy_City_Screen_Draw+141            push    [GUI_String_1@]                 ; dest                                               
Up   r Enemy_City_Screen+3D9                 push    [GUI_String_1@]                 ; dest                                               
Up   r Enemy_City_Screen+3CA                 push    [GUI_String_1@]                 ; dest                                               
Up   r Enemy_City_Screen+3AA                 push    [GUI_String_1@]                 ; dest                                               
Up   r Draw_Unit_List_Window_Pup+19F         push    [GUI_String_1@]                 ; dest                                               
Up   r Draw_Unit_List_Window_Pup:loc_6A897   push    [GUI_String_1@]                 ; dest                                               
Down r GAME_DrawScoreBrkdwn+637              push    [GUI_String_1@]                 ; dest                                               
Up   r sub_65D33+102                         push    [GUI_String_1@]                 ; dest                                               
Down r sub_7AD24+294                         push    [GUI_String_1@]                 ; dst_ofst                                           
Up   r CityList_Screen_Draw+747              push    [GUI_String_1@]                 ; dst_ofst                                           
Up   r CityList_Screen_Draw+671              push    [GUI_String_1@]                 ; dst_ofst                                           
Up   r Draw_Item_With_Name+3D                push    [GUI_String_1@]                 ; dst_ofst                                           
Up   r IDK_CityScr_RsrcBrkdn+2AB             push    [GUI_String_1@]                 ; src                                                
Up   r IDK_CityScr_RsrcBrkdn+F0              push    [GUI_String_1@]                 ; src                                                
Down r IDK_Surveyor_s7A761+28B               push    [GUI_String_1@]                 ; src                                                
Up   r IDK_CityScr_RsrcBrkdn+15F             push    [GUI_String_1@]                 ; src                                                
Down r IDK_Surveyor_s7A761+21D               push    [GUI_String_1@]                 ; src                                                
Down r IDK_Surveyor_s7A761+1B5               push    [GUI_String_1@]                 ; src                                                
Up   r IDK_CityScr_RsrcBrkdn+1CE             push    [GUI_String_1@]                 ; src                                                
Up   r IDK_CityScr_RsrcBrkdn+23D             push    [GUI_String_1@]                 ; src                                                
Down r CMB_DrawFullScreen:loc_810E7          push    [GUI_String_1@]                 ; str                                                
Down r IDK_Surveyor_s7A761+197               push    [GUI_String_1@]                 ; strP                                               
Up   r Magic_Screen_Draw+628                 push    [GUI_String_1@]                 ; strP                                               
Down r IDK_Surveyor_s7A761:loc_7A960         push    [GUI_String_1@]                 ; strP                                               
Up   r Magic_Screen_Draw+736                 push    [GUI_String_1@]                 ; strP                                               
Up   r CityList_Screen_Draw+36B              push    [GUI_String_1@]                 ; strP                                               
Up   r Magic_Screen_Draw+682                 push    [GUI_String_1@]                 ; strP                                               
Up   r Magic_Screen_Draw+6B8                 push    [GUI_String_1@]                 ; strP                                               
Up   r CityList_Screen_Draw+3AC              push    [GUI_String_1@]                 ; strP                                               
Down r sub_7AD24:loc_7AF21                   push    [GUI_String_1@]                 ; string                                             
Down r sub_7AD24+4A1                         push    [GUI_String_1@]                 ; string                                             
Down r sub_7AD24+2A0                         push    [GUI_String_1@]                 ; string                                             
Down r sub_7AD24+26B                         push    [GUI_String_1@]                 ; string                                             
Up   r sub_5BDCE+195                         push    [GUI_String_1@]                 ; string                                             
Up   r sub_65D33+19B                         push    [GUI_String_1@]                 ; string                                             
Up   r sub_65D33+154                         push    [GUI_String_1@]                 ; string                                             
Up   r sub_655C4+1EE                         push    [GUI_String_1@]                 ; string                                             
Up   r sub_65D33+10D                         push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawConqstAnim+E5                push    [GUI_String_1@]                 ; string                                             
Up   r Enemy_City_Screen_Draw+18C            push    [GUI_String_1@]                 ; string                                             
Up   r Draw_Unit_List_Window_Pup+30F         push    [GUI_String_1@]                 ; string                                             
Up   r Draw_Unit_List_Window_Pup+1CA         push    [GUI_String_1@]                 ; string                                             
Up   r City_Screen_Draw2__WIP:loc_4922D      push    [GUI_String_1@]                 ; string                                             
Up   r City_Screen_Draw2__WIP+97             push    [GUI_String_1@]                 ; string                                             
Up   r City_Screen_Draw2__WIP+3D0            push    [GUI_String_1@]                 ; string                                             
Up   r City_Screen_Draw2__WIP+2E0            push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw:loc_5836F        push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+75D              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+3D5              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+394              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+28C              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+255              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+224              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+210              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+1DF              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+1CB              push    [GUI_String_1@]                 ; string                                             
Up   r CityList_Screen_Draw+1B7              push    [GUI_String_1@]                 ; string                                             
Up   r Cartographer_Screen_Draw+220          push    [GUI_String_1@]                 ; string                                             
Down r CMB_DrawUnitDisplay+167               push    [GUI_String_1@]                 ; string                                             
Down r CMB_DrawInfoWindow+1C0                push    [GUI_String_1@]                 ; string                                             
Down r CMB_DrawInfoWindow+180                push    [GUI_String_1@]                 ; string                                             
Down r CMB_DrawActiveUnitW+1E6               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+F8               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+C7               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+B3               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+82               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+6E               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+62E              push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+5A               push    [GUI_String_1@]                 ; string                                             
Up   r ArmyList_Screen_Draw+12F              push    [GUI_String_1@]                 ; string                                             
Up   r Outpost_Screen_Draw+1AF               push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw:loc_63251          push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw:loc_6322D          push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw+848                push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw+37F                push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw+2F0                push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw+239                push    [GUI_String_1@]                 ; string                                             
Up   r Mirror_Screen_Draw+1B1                push    [GUI_String_1@]                 ; string                                             
Up   r Magic_Screen_Draw+7A3                 push    [GUI_String_1@]                 ; string                                             
Up   r Magic_Screen_Draw+6D6                 push    [GUI_String_1@]                 ; string                                             
Up   r Magic_Screen_Draw+6A0                 push    [GUI_String_1@]                 ; string                                             
Up   r Magic_Screen_Draw+646                 push    [GUI_String_1@]                 ; string                                             
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4E4D5  push    [GUI_String_1@]                 ; string                                             
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4E348  push    [GUI_String_1@]                 ; string                                             
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4E0F5  push    [GUI_String_1@]                 ; string                                             
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4DF5C  push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawReturnAnim+69                push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn+287              push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn+3EF              push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn+4CE              push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn+5CB              push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn+683              push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn+715              push    [GUI_String_1@]                 ; string                                             
Down r GAME_DrawScoreBrkdwn:loc_873EF        push    [GUI_String_1@]                 ; string                                             
Down r GAME_Draw_WIN_Anim:loc_79D8B          push    [GUI_String_1@]                 ; string                                             
Down r Hall_Of_Fame_Screen_Draw+18C          push    [GUI_String_1@]                 ; string                                             
Down r Hall_Of_Fame_Screen_Draw+1DA          push    [GUI_String_1@]                 ; string                                             
Down r Hall_Of_Fame_Screen_Draw+26E          push    [GUI_String_1@]                 ; string                                             
Down r Hall_Of_Fame_Screen_Draw+E0           push    [GUI_String_1@]                 ; string                                             
Down r Hero_LevelUp_Popup_Draw+130           push    [GUI_String_1@]                 ; string                                             
Down r Hero_LevelUp_Popup_Draw+2B6           push    [GUI_String_1@]                 ; string                                             
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4DC64  push    [GUI_String_1@]                 ; string                                             
Up   r IDK_CityScr_RsrcBrkdn_Draw:loc_4DDF2  push    [GUI_String_1@]                 ; string                                             
