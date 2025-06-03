




¿ both, but only ovr136 and ovr137 ?
No.
...Learn_Spell_Animation() is in ovr118
...Cast_Spell_Overland__WIP() is in ovr135



LBX strings?



## IDK_DiploScrn_scanned_field



## SBK_Spell_Index

XREF:
    Learn_Spell_Animation+C                   mov     [SBK_Spell_Index], ax                        
    Learn_Spell_Animation+3B3                 push    [SBK_Spell_Index]               ; Spell_Index
    Learn_Spell_Animation+3EF                 mov     ax, [SBK_Spell_Index]                        
    Cast_Spell_Overland__WIP:loc_BAF16        mov     [SBK_Spell_Index], _DI_spell_idx             
    SBK_SliderRedraw+76                       mov     ax, [SBK_Spell_Index]                        
    SBK_SliderRedraw+18C                      mov     ax, [SBK_Spell_Index]                        
    SBK_SliderRedraw+1B5                      push    [SBK_Spell_Index]               ; Spell_Index
    SBK_SpellSlider+59                        mov     [SBK_Spell_Index], ax                        
    SBK_SpellSlider+ED                        mov     ax, [SBK_Spell_Index]                        
    SBK_SpellSlider+119                       mov     ax, [SBK_Spell_Index]                        
    SBK_SpellSlider+142                       push    [SBK_Spell_Index]               ; Spell_Index
    CMB_SliderRedraw+17B                      mov     ax, [SBK_Spell_Index]                        
    CMB_SliderRedraw+1A5                      push    [SBK_Spell_Index]               ; Spell_Index
    CMB_SpellSlider__STUB+17                  mov     [SBK_Spell_Index], di                        
    CMB_SpellSlider__STUB:loc_BD5CB           push    [SBK_Spell_Index]               ; Spell_Index
    CMB_SpellSlider__STUB+3E                  mov     ax, [SBK_Spell_Index]                        
    CMB_SpellSlider__STUB+192                 mov     ax, [SBK_Spell_Index]                        
    CMB_SpellSlider__STUB+1A8                 mov     ax, [SBK_Spell_Index]                        
    CMB_SpellSlider__STUB+1C4                 mov     ax, [SBK_Spell_Index]                        
    IDK_Spell_DisjunctOrBind_Draw+220         cmp     [SBK_Spell_Index], 76                        
    IDK_Spell_DisjunctOrBind_Draw+244         cmp     [SBK_Spell_Index], 76                        
    IDK_Spell_DisjunctOrBind_Draw:loc_BDE65   cmp     [SBK_Spell_Index], 4Ch ; 'L'                 
    IDK_Spell_DisjunctOrBind+2B               mov     [SBK_Spell_Index], _DI_spell_idx             
    OVL_DrawGlobalAnim+223                    mov     ax, [SBK_Spell_Index]                        
    OVL_DrawGlobalAnim+2C7                    mov     ax, [SBK_Spell_Index]                        
    WIZ_GlobalSpellAnim+84                    mov     [SBK_Spell_Index], _SI_spell_idx             
    Spell_Target_Wizard_Screen_Draw:loc_BFD38 cmp     [SBK_Spell_Index], spl_Spell_Blast           
    Spell_Target_Wizard_Screen_Draw+227       mov     ax, [SBK_Spell_Index]                        
    Spell_Target_Wizard_Screen_Draw+281       mov     ax, [SBK_Spell_Index]                        
    Spell_Target_Wizard_Screen+1A             mov     [SBK_Spell_Index], ax                        



## GAME_MP_SpellVar_3

Used as:
    player_idx
    spell_Idx
    city_idx

XERF:
    SBK_SliderRedraw+8                           cmp     [GAME_MP_SpellVar_3], 5         ; clicked spell label index during combat sliders        
    SBK_SliderRedraw:loc_BCE5B                   mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    SBK_SpellSlider+B                            mov     [GAME_MP_SpellVar_3], si        ; clicked spell label index during combat sliders        
    CMB_SliderRedraw+8                           cmp     [GAME_MP_SpellVar_3], 5         ; clicked spell label index during combat sliders        
    CMB_SliderRedraw:loc_BD353                   mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    CMB_SpellSlider__STUB+E                      mov     [GAME_MP_SpellVar_3], ax        ; clicked spell label index during combat sliders        
    IDK_Spell_DisjunctOrBind_Draw:loc_BDE02      cmp     [GAME_MP_SpellVar_3], 1         ; clicked spell label index during combat sliders        
    IDK_Spell_DisjunctOrBind__1+146              mov     [GAME_MP_SpellVar_3], ax        ; clicked spell label index during combat sliders        
    IDK_Spell_DisjunctOrBind__2+111              mov     [GAME_MP_SpellVar_3], ax        ; clicked spell label index during combat sliders        
    IDK_SummonAnim_Draw+82                       cmp     [GAME_MP_SpellVar_3], 0         ; clicked spell label index during combat sliders        
    IDK_SummonAnim_Draw+100                      mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    IDK_SummonAnim+44                            mov     [GAME_MP_SpellVar_3], _SI_player_idx; clicked spell label index during combat sliders    
    IDK_CastCityEnchMsg+5F                       mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    IDK_CastCityEnchMsg+94                       mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    IDK_Spell_Cityscape_Draw+2C                  push    [GAME_MP_SpellVar_3]            ; clicked spell label index during combat sliders        
    IDK_Spell_Cityscape_Draw:loc_BEF3C           mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    IDK_Spell_Cityscape_1+55                     mov     [GAME_MP_SpellVar_3], ax        ; clicked spell label index during combat sliders        
    IDK_Spell_Cityscape_2:loc_BF2BD              mov     [GAME_MP_SpellVar_3], ax        ; clicked spell label index during combat sliders        
    OVL_DrawGlobalAnim+49                        cmp     [GAME_MP_SpellVar_3], e_HUMAN_PLAYER_IDX; clicked spell label index during combat sliders
    OVL_DrawGlobalAnim:loc_BF63D                 mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    OVL_DrawGlobalAnim:loc_BF82B                 cmp     [GAME_MP_SpellVar_3], 0         ; clicked spell label index during combat sliders        
    OVL_DrawGlobalAnim:loc_BF841                 mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    WIZ_GlobalSpellAnim+80                       mov     [GAME_MP_SpellVar_3], _DI_player_idx; clicked spell label index during combat sliders    
    Spell_Target_Wizard_Screen_Draw:@@DoCastDone mov     ax, [GAME_MP_SpellVar_3]        ; clicked spell label index during combat sliders        
    Spell_Target_Wizard_Screen:loc_C04B5         mov     [GAME_MP_SpellVar_3], _DI_player_idx; clicked spell label index during combat sliders    
