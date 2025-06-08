
GUI_Interaction_Done  ==>  _osc_leave_screen
IDK_WizTgt_SplCmpl_w434EE  ==>  _osc_need_target_flag
IDK_DiploScrn_scanned_field  ==>  _temp_sint_4
SBK_SliderAnimStage  ==>  _osc_anim_ctr
GAME_MP_SpellVar_3  ==>  _temp_sint_1

¿ both, but only ovr136 and ovr137 ?
No.
...Learn_Spell_Animation() is in ovr118
...Cast_Spell_Overland__WIP() is in ovr135
...SoM_Started()?

Tighter boundaries?
...all well within 'Overland Spell Casting'?
...in and around Animation, but not quite contained within?



LBX strings?



Cast_Spell_Overland__WIP()
    if(spell_idx == spl_Spell_Binding)
        Cast_Successful = Cast_SpellBinding(player_idx)
            IDK_Spell_DisjunctOrBind(spl_Spell_Binding, player_idx)

Cast_Spell_Overland__WIP()
switch(spell_data_table[spell_idx].type)
    case scc_Disjunction_Spell:  // 20
        if(player_idx == HUMAN_PLAYER_IDX)
            Cast_Successful = IDK_Spell_DisjunctOrBind(spell_idx, player_idx)
Spell Binding
Spell Blast
Disjunction       (Arcane)
Disjunction True  (Sorcery)
Disenchant Area   (Arcane)
Disenchant True   (Sorcery)

Disenchant Area:
    Arcane. Instant. Casting Cost: 50+ mana. Uncommon.
    Attempts to remove any enchantments within the target area (a
    map square, or the entire battlefield, when casting this spell
    overland or during combat, respectively) including enchantments
    affecting creatures, cities, nodes and land. The enchantments
    automatically targeted by this spell are detrimental to your cause.
Disenchant True:
    Sorcery. Instant. Casting Cost: 50+ mana. Uncommon.
    A much stronger version of the arcane disenchant area, with double
    the chance of removing target enchantments (each point of mana
    spent on casting the spell counts as two for the purposes of
    dispelling a target enchantment).

Disjunction:
    Arcane. Instant. Casting Cost: 100+ mana. Rare.
    Tries to remove one global enemy enchantment.
Disjunction True:
    Sorcery. Instant. Casting Cost: 100+ mana. Rare.
    Equivalent to a very strong version of the arcane disjunction, with
    double the chance of success. Each mana spent on casting this spell
    counts as two for the purposes of dispelling the target global
    enchantment.

Spell Binding:
    Sorcery. Instant. Casting Cost: 1000 mana;
    Upkeep: As For Target Enchantment. Very Rare.
    Steals a global enchantment from another wizard. The stolen
    overland spell becomes the property of the casting wizard in terms
    of maintenance and control.

Spell Blast:
    Sorcery. Instant. Casting Cost: 50+ mana. Uncommon.
    Attempts, through the direct manipulation of magic power, to
    counter an enemy wizard’s spell as it is being cast. When spell blast
    takes effect, a wizard must expend an amount of his or her own
    magic power (from mana reserves) equal to the amount already put
    into the enemy wizard’s spell to blast that spell out of existence. If
    the wizard does not have enough mana, spell blast fails.





## _temp_sint_4
_osc_scanned_field
AKA IDK_DiploScrn_scanned_field
#define _osc_scanned_field _temp_sint_4
#define _osc_summon_unit_type _temp_sint_4

¿ _temp_int ?
but, UNITS[].type is uint8_t?

IDK_SummonAnim()
IDK_SummonAnim_Draw()
set to unit_type, which spell_data_table[].Param0
but, also special cases of -1 for *summon* item and -2,-3,-20,-30 for summon hero/champion
¿ ~ summon_type ?

Used As:
    scanned_field_idx
    unit_type_idx

    City Enchantment Animation
        used for drawing name of scanned building

SoM_Started()
    IDK_DiploScrn_scanned_field = 67;
    ...no other XREF? therefore, unused?

Spell_Target_Wizard_Screen()
    IDK_DiploScrn_scanned_field = Scan_Input();
Spell_Target_Wizard_Screen_Draw()
    if(gem_player_nums[itr_gems] == IDK_DiploScrn_scanned_field)

IDK_SummonAnim()
    IDK_DiploScrn_scanned_field = _DI_unit_type
IDK_SummonAnim_Draw()
    if(IDK_DiploScrn_scanned_field > 0)    unit
    if(IDK_DiploScrn_scanned_field == -1)  item
    if(IDK_DiploScrn_scanned_field > -10)  hero or champion

Cast_Spell_City_Enchantment_Animation_1__WIP()
    IDK_DiploScrn_scanned_field = INVALID_FIELD;
Cast_Spell_City_Enchantment_Animation_2__WIP()

Cast_Spell_City_Enchantment_Animation_Draw()
    if(city_cityscape_fields[itr] == IDK_DiploScrn_scanned_field)

XREF:
    IDK_Spell_DisjunctOrBind_Draw+44                 cmp     [IDK_DiploScrn_scanned_field], 0              
    IDK_Spell_DisjunctOrBind_Draw+54                 cmp     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind_Draw+5E                 mov     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind_Draw+6C                 mov     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind_Draw+26A                mov     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind_Draw+278                mov     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind+1E4                     mov     [IDK_DiploScrn_scanned_field], ax             
    IDK_Spell_DisjunctOrBind+1E7                     mov     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind+1F1                     mov     ax, [IDK_DiploScrn_scanned_field]             
    IDK_Spell_DisjunctOrBind+207                     mov     [IDK_DiploScrn_scanned_field], 0              
    IDK_Spell_DisjunctOrBind+221                     cmp     [IDK_DiploScrn_scanned_field], 0              
    IDK_SummonAnim_Draw+F9                           cmp     [IDK_DiploScrn_scanned_field], 0              
    IDK_SummonAnim_Draw:loc_BEB43                    cmp     [IDK_DiploScrn_scanned_field], -1             
    IDK_SummonAnim_Draw:loc_BEB5B                    cmp     [IDK_DiploScrn_scanned_field], -10            
    IDK_SummonAnim+48                                mov     [IDK_DiploScrn_scanned_field], _DI_unit_type  
    Cast_Spell_City_Enchantment_Animation_Draw+49    cmp     ax, [IDK_DiploScrn_scanned_field]             
    Cast_Spell_City_Enchantment_Animation_1__WIP+58  mov     [IDK_DiploScrn_scanned_field], e_INVALID_FIELD
    Cast_Spell_City_Enchantment_Animation_2__WIP+12D mov     [IDK_DiploScrn_scanned_field], ax             
    Spell_Target_Wizard_Screen_Draw+104              cmp     ax, [IDK_DiploScrn_scanned_field]             
    Spell_Target_Wizard_Screen+1B1                   mov     [IDK_DiploScrn_scanned_field], ax             
    SoM_Started+5F                                   mov     [IDK_DiploScrn_scanned_field], 67             






## _osc_leave_screen
AKA GUI_Interaction_Done
¿ _temp_sint_ ?

Areas of Usage:
    Spell_Target_Wizard_Screen
    Sml/Cmb Spell Power Slider
    Cast Spell Overland/Global Enchantment Animation
    Cast Spell City Enchantment Animation
    IDK_Spell_Cityscape_Draw
    ¿ IDK_Spell_DisjunctOrBind ?
    ¿ IDK_SummonAnim ?

XREF:
    SBK_SpellSlider+D5                   mov     [_osc_leave_screen], ST_FALSE         
    SBK_SpellSlider+180                  mov     [_osc_leave_screen], e_ST_TRUE 
    SBK_SpellSlider:loc_BD233            cmp     [_osc_leave_screen], e_ST_FALSE
    SBK_SpellSlider:loc_BD252            cmp     [_osc_leave_screen], e_ST_FALSE
    CMB_SpellSlider__STUB+29E            mov     [_osc_leave_screen], ST_FALSE         
    CMB_SpellSlider__STUB+2BB            mov     [_osc_leave_screen], e_ST_TRUE 
    CMB_SpellSlider__STUB:loc_BD85E      cmp     [_osc_leave_screen], e_ST_FALSE
    CMB_SpellSlider__STUB:loc_BD87D      cmp     [_osc_leave_screen], e_ST_FALSE
    IDK_Spell_DisjunctOrBind:loc_BE06E   mov     [_osc_leave_screen], ST_FALSE         
    IDK_Spell_DisjunctOrBind+271         mov     [_osc_leave_screen], 1         
    IDK_Spell_DisjunctOrBind:loc_BE2C9   cmp     [_osc_leave_screen], ST_FALSE         
    IDK_Spell_DisjunctOrBind:loc_BE2DE   cmp     [_osc_leave_screen], ST_FALSE         
    IDK_SummonAnim:loc_BEC43             mov     [_osc_leave_screen], e_ST_FALSE
    IDK_SummonAnim+D4                    mov     [_osc_leave_screen], e_ST_TRUE 
    IDK_SummonAnim+104                   cmp     [_osc_leave_screen], e_ST_FALSE
    OVL_DrawGlobalAnim+B                 cmp     [_osc_leave_screen], e_ST_TRUE 
    WIZ_GlobalSpellAnim+E3               mov     [_osc_leave_screen], ST_FALSE         
    WIZ_GlobalSpellAnim+109              mov     [_osc_leave_screen], e_ST_TRUE 
    WIZ_GlobalSpellAnim:loc_BF9DF        cmp     [_osc_leave_screen], e_ST_TRUE 
    WIZ_GlobalSpellAnim+1AC              cmp     [_osc_leave_screen], ST_FALSE         
    Spell_Target_Wizard_Screen+19B       mov     [_osc_leave_screen], ST_FALSE         
    Spell_Target_Wizard_Screen+1BC       mov     [_osc_leave_screen], e_ST_TRUE 
    Spell_Target_Wizard_Screen:loc_C025E mov     [_osc_leave_screen], e_ST_TRUE 
    Spell_Target_Wizard_Screen:loc_C0266 mov     [_osc_leave_screen], 1         
    Spell_Target_Wizard_Screen:loc_C048D cmp     [_osc_leave_screen], e_ST_FALSE
    Spell_Target_Wizard_Screen:loc_C04A2 cmp     [_osc_leave_screen], ST_FALSE         




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



## _temp_sint_1
AKA GAME_MP_SpellVar_3
_temp_sint_1
#define _osc_player_idx _temp_sint_1
#define _osc_spell_Idx _temp_sint_1
#define _osc_city_idx _temp_sint_1
#define _osc_bldg_idx _temp_sint_1

Cast_Spell_Overland_Do() passes spellbook_page_spell_index to SBK_SpellSlider()
SBK_SpellSlider() sets GAME_MP_SpellVar_3 to spellbook_page_spell_index

Areas of Usage:
    Spell_Target_Wizard_Screen
    Sml/Cmb Spell Power Slider
    Cast Spell Overland/Global Enchantment Animation
    Cast Spell City Enchantment Animation
    IDK_Spell_Cityscape_Draw
    ¿ IDK_Spell_DisjunctOrBind ?
    ¿ IDK_SummonAnim ?

Used as:
    player_idx
    spell_Idx
    city_idx
    bldg_idx

XERF:
    SBK_SliderRedraw+8                                             cmp     [_temp_sint_1], e_NEUTRAL_PLAYER_IDX ; clicked spell label index during combat sliders              
    SBK_SliderRedraw:loc_BCE5B                                     mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    SBK_SpellSlider+B                                              mov     [_temp_sint_1], _SI_spellbook_page_spell_index; clicked spell label index during combat sliders
    CMB_SliderRedraw+8                                             cmp     [_temp_sint_1], e_NEUTRAL_PLAYER_IDX ; clicked spell label index during combat sliders              
    CMB_SliderRedraw:loc_BD353                                     mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    CMB_SpellSlider__STUB+E                                        mov     [_temp_sint_1], ax              ; clicked spell label index during combat sliders              
    IDK_Spell_DisjunctOrBind_Draw:loc_BDE02                        cmp     [_temp_sint_1], 1               ; clicked spell label index during combat sliders              
    IDK_Spell_DisjunctOrBind__1+146                                mov     [_temp_sint_1], ax              ; clicked spell label index during combat sliders              
    IDK_Spell_DisjunctOrBind__2+111                                mov     [_temp_sint_1], ax              ; clicked spell label index during combat sliders              
    IDK_SummonAnim_Draw+82                                         cmp     [_temp_sint_1], e_HUMAN_PLAYER_IDX ; clicked spell label index during combat sliders              
    IDK_SummonAnim_Draw+100                                        mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    IDK_SummonAnim+44                                              mov     [_temp_sint_1], _SI_player_idx  ; clicked spell label index during combat sliders              
    Cast_Spell_City_Enchantment_Animation_Load+5F                  mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    Cast_Spell_City_Enchantment_Animation_Load+94                  mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    Cast_Spell_City_Enchantment_Animation_Draw_Cityscape+2C        push    [_temp_sint_1]                  ; clicked spell label index during combat sliders              
    Cast_Spell_City_Enchantment_Animation_Draw_Cityscape:loc_BEF3C mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    Cast_Spell_City_Enchantment_Animation_1__WIP+55                mov     [_temp_sint_1], ax              ; clicked spell label index during combat sliders              
    Cast_Spell_City_Enchantment_Animation_2__WIP+E                 mov     [_temp_sint_1], ax              ; clicked spell label index during combat sliders              
    OVL_DrawGlobalAnim+49                                          cmp     [_temp_sint_1], e_HUMAN_PLAYER_IDX; clicked spell label index during combat sliders            
    OVL_DrawGlobalAnim:loc_BF63D                                   mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    OVL_DrawGlobalAnim:loc_BF82B                                   cmp     [_temp_sint_1], e_HUMAN_PLAYER_IDX; clicked spell label index during combat sliders            
    OVL_DrawGlobalAnim:loc_BF841                                   mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    WIZ_GlobalSpellAnim+80                                         mov     [_temp_sint_1], _DI_player_idx  ; clicked spell label index during combat sliders              
    Spell_Target_Wizard_Screen_Draw:@@DoCastDone                   mov     ax, [_temp_sint_1]              ; clicked spell label index during combat sliders              
    Spell_Target_Wizard_Screen:loc_C04B5                           mov     [_temp_sint_1], _DI_player_idx  ; clicked spell label index during combat sliders              



## GAME_MP_SpellVar_1

Areas of Usage:
    Spell_Target_Wizard_Screen
    Sml/Cmb Spell Power Slider
    Cast Spell Overland/Global Enchantment Animation

XREF:
    SBK_SliderRedraw+9F                 mov     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    SBK_SliderRedraw+B9                 push    [GAME_MP_SpellVar_1]            ; value                                  
    SBK_SliderRedraw+19E                add     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    SBK_SliderRedraw+1A2                mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    SBK_SliderRedraw+1AF                mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    SBK_SliderRedraw+1D2                mov     dx, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    SBK_SpellSlider+4D                  mov     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    SBK_SpellSlider+116                 mov     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    SBK_SpellSlider+12B                 add     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    SBK_SpellSlider+12F                 mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    SBK_SpellSlider+13C                 mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    SBK_SpellSlider+15F                 mov     dx, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    CMB_SliderRedraw+8E                 mov     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    CMB_SliderRedraw+A8                 push    [GAME_MP_SpellVar_1]            ; value                                  
    CMB_SliderRedraw+18D                add     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    CMB_SliderRedraw+19F                mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    CMB_SliderRedraw:loc_BD4FE          mov     dx, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    CMB_SliderRedraw:loc_BD50A          mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    CMB_SpellSlider__STUB+21C           mov     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders
    CMB_SpellSlider__STUB+300           mov     ax, [GAME_MP_SpellVar_1]        ; holds the spell strength during sliders
    OVL_LoadGlobalAnim+97               cmp     [GAME_MP_SpellVar_1], 20        ; holds the spell strength during sliders
    OVL_LoadGlobalAnim:loc_BF521        cmp     [GAME_MP_SpellVar_1], 30        ; holds the spell strength during sliders
    OVL_LoadGlobalAnim:loc_BF564        cmp     [GAME_MP_SpellVar_1], 10        ; holds the spell strength during sliders
    OVL_LoadGlobalAnim:loc_BF59B        cmp     [GAME_MP_SpellVar_1], 20        ; holds the spell strength during sliders
    OVL_DrawGlobalAnim:loc_BF620        cmp     [GAME_MP_SpellVar_1], 34        ; holds the spell strength during sliders
    OVL_DrawGlobalAnim:loc_BF67B        cmp     [GAME_MP_SpellVar_1], 14        ; holds the spell strength during sliders
    OVL_DrawGlobalAnim:loc_BF704        cmp     [GAME_MP_SpellVar_1], 20        ; holds the spell strength during sliders
    OVL_DrawGlobalAnim:loc_BF756        cmp     [GAME_MP_SpellVar_1], 34        ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BF94A       mov     [GAME_MP_SpellVar_1], 0         ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim+E9              mov     [GAME_MP_SpellVar_1], 0         ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BF9E9       cmp     [GAME_MP_SpellVar_1], 10        ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BFA0E       cmp     [GAME_MP_SpellVar_1], 20        ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BFA33       cmp     [GAME_MP_SpellVar_1], 30        ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BFA56       cmp     [GAME_MP_SpellVar_1], 34        ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BFA71       inc     [GAME_MP_SpellVar_1]            ; holds the spell strength during sliders
    WIZ_GlobalSpellAnim:loc_BFA75       cmp     [GAME_MP_SpellVar_1], 120       ; holds the spell strength during sliders
    Spell_Target_Wizard_Screen_Draw+2CB push    [GAME_MP_SpellVar_1]            ; value                                  
    Spell_Target_Wizard_Screen_Draw+302 push    [GAME_MP_SpellVar_1]            ; value                                  
    Spell_Target_Wizard_Screen+5F6      mov     [GAME_MP_SpellVar_1], ax        ; holds the spell strength during sliders



## _osc_anim_ctr
AKA SBK_SliderAnimStage
...but, also used for the combat spell power slider
...but, probably, really, just because it's ~== SmlSpellbook

¿ vs. Spellbook.C  SBK_NewSpellAnim_Stg ?

SBK_SpellSlider()
SBK_SliderRedraw()

CMB_SpellSlider__STUB()
CMB_SliderRedraw()

IDK_SummonAnim()
    _osc_anim_ctr = 0
    for(_osc_anim_ctr = 0; ((_osc_anim_ctr < var_6) && (_osc_leave_screen == ST_FALSE)); _osc_anim_ctr++)
IDK_SummonAnim_Draw()
    if ... (_osc_anim_ctr < 30)
        Clipped_Draw((start_x + 76), (start_y + 110 - (_osc_anim_ctr * 3)), IDK_MONSTER_seg);
    ...draws as *rising*, 3 more pixels per frame

Spell_Of_Mastery_Lose()
Spell_Of_Mastery_Lose_Draw()

Spell_Of_Mastery()
Spell_Of_Mastery_Draw()

CMB_CreateUndeadAnim()
    _osc_anim_ctr = 0
    for(_osc_anim_ctr = 0; ((_osc_anim_ctr < 100) && (leave_screen == ST_FALSE)); _osc_anim_ctr++)
CMB_DrawUndeadAnim()
    if(_osc_anim_ctr < Last_Rising_Frame)
        Clipped_Draw() ... (y_start + 17 - ((_osc_anim_ctr * 17) / Last_Rising_Frame))
    ...same concept as in IDK_SummonAnim()

XREF:
    SBK_SliderRedraw+2C                  mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    SBK_SliderRedraw+36                  mov     [_osc_anim_ctr], dx       ; steps 0 to 7 for sliders
    SBK_SliderRedraw+216                 add     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    SBK_SliderRedraw+232                 add     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    SBK_SpellSlider+35                   mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    CMB_SliderRedraw+2C                  mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_SliderRedraw+36                  mov     [_osc_anim_ctr], dx       ; steps 0 to 7 for sliders
    CMB_SliderRedraw+20E                 add     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_SliderRedraw+22A                 add     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_SpellSlider__STUB+205            mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    IDK_SummonAnim_Draw+74               cmp     [_osc_anim_ctr], 30       ; steps 0 to 7 for sliders
    IDK_SummonAnim_Draw+8D               mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    IDK_SummonAnim:loc_BEBD0             mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    IDK_SummonAnim+B7                    mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    IDK_SummonAnim+F8                    inc     [_osc_anim_ctr]           ; steps 0 to 7 for sliders
    IDK_SummonAnim:loc_BEC8E             mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+C7        cmp     [_osc_anim_ctr], 12       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+E2        mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw:loc_C088E cmp     [_osc_anim_ctr], 24       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+144       mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw:loc_C09B7 mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+3D0       mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+3ED       mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+430       mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw:loc_C0BDD cmp     [_osc_anim_ctr], 36       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose_Draw+462       mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose+8F             mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose+A9             mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose+CE             inc     [_osc_anim_ctr]           ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Lose:loc_C0CFA      cmp     [_osc_anim_ctr], 43       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw+1B             cmp     [_osc_anim_ctr], 6        ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw:loc_C1060      mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw+36             mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw:loc_C1092      mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw+68             mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw+AF             cmp     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery_Draw+BA             mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery:loc_C115E           mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    Spell_Of_Mastery+106                 mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    Spell_Of_Mastery:loc_C124B           mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery+15D                 cmp     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    Spell_Of_Mastery:loc_C12CA           cmp     [_osc_anim_ctr], 7        ; steps 0 to 7 for sliders
    Spell_Of_Mastery+1C9                 inc     [_osc_anim_ctr]           ; steps 0 to 7 for sliders
    Spell_Of_Mastery:loc_C1309           mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_DrawUndeadAnim+3E                mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_DrawUndeadAnim+4A                mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_DrawUndeadAnim+6B                mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_DrawUndeadAnim+9A                mov     ax, [_osc_anim_ctr]       ; steps 0 to 7 for sliders
    CMB_CreateUndeadAnim+F               mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    CMB_CreateUndeadAnim+51              mov     [_osc_anim_ctr], 0        ; steps 0 to 7 for sliders
    CMB_CreateUndeadAnim+8D              inc     [_osc_anim_ctr]           ; steps 0 to 7 for sliders
    CMB_CreateUndeadAnim:loc_C162E       cmp     [_osc_anim_ctr], 100      ; steps 0 to 7 for sliders



## _osc_need_target_flag
AKA IDK_WizTgt_SplCmpl_w434EE

IDK_Spell_DisjunctOrBind_Draw()
    if(_osc_need_target_flag == ST_TRUE)
        "Select a spell to " "disjunct." || "bind."
    else
        "Spell succesfully dispelled.", "Spell succesfully bound.", "Disjunction unsuccesful", "Spell Binding unsuccesful"

Spell_Target_Wizard_Screen_Draw()
    if(_osc_need_target_flag == ST_TRUE)
        Print_Paragraph() ..."Choose target for a " [SPELL NAME] " spell"
    else
        Spell Blast, Cruel Unminding, Drain Power, Subversion
        " has been spell blasted", " loses " " points of casting ability", " loses " " points of mana", " relations have been subverted"
    ... ~ 'need target'?

Cast_Spell_City_Enchantment_Animation_1__WIP()
    _osc_need_target_flag = ST_TRUE;
    _osc_need_target_flag = ST_FALSE;
    _osc_need_target_flag = ST_TRUE;
    _osc_need_target_flag = ST_FALSE;
    ...~unused

XREF:
    IDK_Spell_DisjunctOrBind_Draw+20A                      cmp     [_osc_need_target_flag], 1        
    IDK_Spell_DisjunctOrBind+1F                            mov     [_osc_need_target_flag], 1        
    IDK_Spell_DisjunctOrBind__1+1E3                        mov     [_osc_need_target_flag], ST_FALSE        
    IDK_Spell_DisjunctOrBind__2+167                        mov     [_osc_need_target_flag], ST_FALSE        
    Cast_Spell_City_Enchantment_Animation_1__WIP:loc_BF094 mov     [_osc_need_target_flag], 1        
    Cast_Spell_City_Enchantment_Animation_1__WIP+E2        mov     [_osc_need_target_flag], ST_FALSE        
    Cast_Spell_City_Enchantment_Animation_1__WIP+F6        mov     [_osc_need_target_flag], 1        
    Cast_Spell_City_Enchantment_Animation_1__WIP+105       mov     [_osc_need_target_flag], ST_FALSE        
    Spell_Target_Wizard_Screen_Draw+211                    cmp     [_osc_need_target_flag], e_ST_TRUE
    Spell_Target_Wizard_Screen+1D                          mov     [_osc_need_target_flag], 1        
    Spell_Target_Wizard_Screen+61F                         mov     [_osc_need_target_flag], ST_FALSE        



## _combat_wx

in SoM_Started... animation counter?
    SoM_Started()
        _combat_wx = 0;
        if(_combat_wx ==  17)
        if(_combat_wx ==  33)
        if(_combat_wx ==  49)
        if(_combat_wx == 200)
    SoM_Started_Draw()
        _combat_wx++;

XREF:
    Lair_Combat__WIP+A4                  mov     [_combat_wx], ax                        
    Lair_Combat__WIP+219                 mov     al, [byte ptr _combat_wx]               
    Lair_Generate_Treasure+2A5           push    [_combat_wx]                    ; X_Pos 
    Tactical_Combat__WIP+A0              mov     [_combat_wx], ax                        
    Tactical_Combat__WIP+DE5             push    [_combat_wx]                            
    Move_Units+55C                       cmp     ax, [_combat_wx]                        
    Move_Units+5D3                       push    [_combat_wx]                    ; X_Pos 
    Move_Units+651                       push    [_combat_wx]                    ; X_Pos 
    Move_Units:@@EndOfMoving             mov     [_combat_wx], e_ST_UNDEFINED            
    Combat__WIP+159                      mov     [_combat_wx], ax                        
    Combat__WIP+1B2                      mov     [_combat_wx], ax                        
    Combat__WIP+1FA                      cmp     ax, [_combat_wx]                        
    Combat__WIP+270                      mov     [_combat_wx], ax                        
    Combat__WIP+3E6                      push    [_combat_wx]                    ; XPos  
    Combat__WIP+416                      push    [_combat_wx]                            
    Combat__WIP+4A7                      mov     al, [byte ptr _combat_wx]               
    Combat__WIP+4F4                      mov     ax, [_combat_wx]                        
    UU_IDK_Main_Screen_Draw+1A           push    [_combat_wx]                    ; Tile_X
    AI_CMB_PlayTurn__WIP+1A              push    [_combat_wx]                            
    Retreat_From_Combat+303              push    [_combat_wx]                    ; XPos  
    Retreat_From_Combat:loc_7F631        mov     ax, [_combat_wx]                        
    Retreat_From_Combat+8EE              cmp     ax, [_combat_wx]                        
    CMB_Units_Init__WIP+186              cmp     ax, [_combat_wx]                        
    UNIT_PushOffTile+BE                  cmp     ax, [_combat_wx]                        
    GAME_NextHumanStack+F                mov     [_combat_wx], e_ST_UNDEFINED            
    Combat_Node_type+3D                  cmp     ax, [_combat_wx]                        
    Combat_Cast_Spell_With_Caster+88     push    [_combat_wx]                            
    Combat_Cast_Spell_With_Caster+B8     push    [_combat_wx]                            
    Combat_Cast_Spell_With_Caster+D6     push    [_combat_wx]                            
    Strategic_Combat__STUB+3E            mov     [_combat_wx], ax                        
    Do_Legal_Spell_Check__WIP:loc_8E548  mov     ax, [_combat_wx]                        
    Combat_Casting_Cost_Multiplier+62    sub     ax, [_combat_wx]                        
    AITP_RecallHero+23                   cmp     ax, [_combat_wx]                        
    AI_BU_ProcessAction__WIP+8A6         push    [_combat_wx]                            
    End_Of_Combat__WIP+5DA               mov     al, [byte ptr _combat_wx]               
    End_Of_Combat__WIP+7E4               mov     al, [byte ptr _combat_wx]               
    CTY_RampageVictory+267               mov     al, [byte ptr _combat_wx]               
    Undeployable_Battle_Units_On_Water+F push    [_combat_wx]                    ; XPos  
    CMB_Disenchant+2D7                   cmp     ax, [_combat_wx]                        
    SoM_Started_Draw+78                  inc     [_combat_wx]                            
    SoM_Started+10E                      mov     [_combat_wx], 0                         
    SoM_Started:loc_C0EC9                cmp     [_combat_wx], 17                        
    SoM_Started:loc_C0EE1                cmp     [_combat_wx], 33                        
    SoM_Started:loc_C0EF9                cmp     [_combat_wx], 49                        
    SoM_Started:loc_C0F2C                cmp     [_combat_wx], 200                       
