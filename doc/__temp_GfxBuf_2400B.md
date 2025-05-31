
vs. other scratch buffers?
how to locate?
how to bound?

¿ spell_animation_seg ?



Elsewhere, ...
BU_CreateImage__SEGRAX() ... IMG_CMB_FX_Figure@  124 PR, 1984 B



Create_Picture()
Draw_Picture_To_Bitmap()
Copy_Bitmap_To_Bitmap()
Clipped_Copy_Bitmap()
Draw_Picture()
FLIC_Set_LoopFrame_1()
Get_Bitmap_Actual_Size()
Vanish_Bitmap()
Outline_Bitmap_Pixels()
Replace_Color()
Transparent_Color_Range()
Gray_Scale_Bitmap()
LBX_IMG_RevGrayscale__STUB()
Replace_Color_All()
Outline_Bitmap_Pixels_No_Glass()
Bitmap_Aura_Pixels()


// WZD dseg:C7B2
SAMB_ptr IMG_CMB_FX_Figure;
BU_CreateImage__SEGRAX()
    IMG_CMB_FX_Figure = Allocate_Next_Block(World_Data, 124);  // 124 PR, 1984 B
    Create_Picture(47, 42, IMG_CMB_FX_Figure);
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);
    for(itr_figures = 0; itr_figures < cur_fig; itr_figures++)
        Battle_Unit_Figure_Position(max_fig, itr_figures, &fig_x, &fig_y);
        Clipped_Copy_Bitmap(fig_x, fig_y - 4, IMG_CMB_FX_Figure, GfxBuf_2400B);
BU_CombatSummon__SEGRAX()
    Create_Picture(45, 42, GfxBuf_2400B);
    Copy_Bitmap_To_Bitmap(GfxBuf_2400B, IMG_CMB_FX_Figure);



## Copy_Bitmap_To_Bitmap()

¿ BU_Teleport(), BU_TunnelTo(), BU_CombatSummon__SEGRAX() are just mimicing compose battle unit picture ?

XREF:
    Cast_PlaneShift()
    BU_Teleport()
    BU_TunnelTo()
    BU_CombatSummon__SEGRAX()


## spell_animation_seg

BU_CombatSummon__SEGRAX()
    BU_CreateImage__SEGRAX(battle_unit_idx)
        IMG_CMB_FX_Figure = Allocate_Next_Block()
        Create_Picture()  IMG_CMB_FX_Figure
        Clipped_Copy_Bitmap()  IMG_CMB_FX_Figure, GfxBuf_2400B




BU_CombatSummon__SEGRAX()
Spell_Animation_Load_Graphics__WIP

    IMG_GUI_Chasm = spell_animation_seg;  // ; appended reserved EMM header in GFX_Swap_Seg ; 8 frame animation
    CMB_Chasm_Anim = 1;
CMB_DrawMap__WIP()
    if(
        (CMB_Chasm_Anim == 1)
        &&
        (CMB_Chasm_Anim_X == cgx)
        &&
        (CMB_Chasm_Anim_Y == cgy)
    )
    {
        FLIC_Set_CurrentFrame(IMG_GUI_Chasm, CMB_ChasmAnimStage);
        Clipped_Draw(screen_x, (screen_y - 18), IMG_GUI_Chasm);
    }



Up   w Learn_Spell_Animation+374              mov     [spell_animation_seg], ax                      
Up   r SBK_DrawSpellDecode__STUB+1D0          push    [spell_animation_seg]           ; pict_seg     
Up   r SBK_DrawSpellDecode__STUB+1DB          push    [spell_animation_seg]           ; picture      
Up   r SBK_DrawSpellDecode__STUB+245          push    [spell_animation_seg]           ; pict_seg     
Up   r SBK_DrawSpellDecode__STUB+250          push    [spell_animation_seg]           ; picture      
Up   w Call_Forth_The_Force_Of_Nature+A3      mov     [spell_animation_seg], ax                      
Up   r Call_Forth_The_Force_Of_Nature+FB      push    [spell_animation_seg]           ; picture      
Up   r Call_Forth_The_Force_Of_Nature+1CA     push    [spell_animation_seg]           ; picture      
Up   r Cast_ChangeTerain:loc_AFEF3            push    [spell_animation_seg]           ; pict_seg     
Up   r Cast_ChangeTerain+3BC                  push    [spell_animation_seg]           ; pict_seg     
Up   r Cast_ChangeTerain+3C7                  push    [spell_animation_seg]           ; picture      
Up   r Cast_Corruption:loc_B0F77              push    [spell_animation_seg]           ; pict_seg     
Up   r Cast_Corruption+42A                    push    [spell_animation_seg]           ; pict_seg     
Up   r Cast_Corruption+435                    push    [spell_animation_seg]           ; picture      
Up   r CMB_BattlefieldSpell+115               push    [spell_animation_seg]           ; pict_seg     
Up   r CMB_BattlefieldSpell+307               push    [spell_animation_seg]           ; pict_seg     
Up   r CMB_BattlefieldSpell+312               push    [spell_animation_seg]           ; picture      
Up   r Cast_PlaneShift_DrawSwitchPlane+9F     push    [spell_animation_seg]           ; pict_seg     
Up   w Cast_PlaneShift+1A0                    mov     [spell_animation_seg], ax                      
     r Cast_PlaneShift:loc_B54D3              push    [spell_animation_seg]           ; source_bitmap
Down r Cast_PlaneShift:loc_B56C8              push    [spell_animation_seg]           ; source_bitmap
Down r Cast_PlaneShift:loc_B571C              push    [spell_animation_seg]           ; source_bitmap
Down r Cast_PlaneShift:loc_B58A5              push    [spell_animation_seg]           ; source_bitmap
Down r WIZ_Wrack:loc_B693E                    push    [spell_animation_seg]           ; pict_seg     
Down r WIZ_Wrack:loc_B69A7                    push    [spell_animation_seg]           ; pict_seg     
Down r WIZ_Wrack+270                          push    [spell_animation_seg]           ; pict_seg     
Down r WIZ_Wrack+27B                          push    [spell_animation_seg]           ; picture      
Down r WIZ_Wrack:loc_B6A44                    push    [spell_animation_seg]           ; pict_seg     
Down r TILE_CracksCall+21                     mov     ax, [spell_animation_seg]                      
Down r TILE_CracksCall:loc_B71A9              push    [spell_animation_seg]           ; pict_seg     
Down r TILE_BoltFromAbove+138                 push    [spell_animation_seg]           ; pict_seg     
Down r TILE_BoltFromAbove+155                 push    [spell_animation_seg]           ; pict_seg     
Down r TILE_BoltFromAbove:loc_B733B           push    [spell_animation_seg]           ; pict_seg     
Down r TILE_BoltFromAbove+196                 push    [spell_animation_seg]           ; picture      
Down r TILE_BoltFromAbove:loc_B7372           push    [spell_animation_seg]           ; picture      
Down r TILE_CombatSpellAnim:loc_B7416         push    [spell_animation_seg]           ; pict_seg     
Down r TILE_CombatSpellAnim+5B                push    [spell_animation_seg]           ; picture      
Down r TILE_CombatSpellAnim+80                push    [spell_animation_seg]           ; picture      
Down r BU_CombatSummon__SEGRAX+7F             mov     ax, [spell_animation_seg]                      
Down r TILE_LightningBolt+8F                  push    [spell_animation_seg]           ; pict_seg     
Down r TILE_LightningBolt+9A                  push    [spell_animation_seg]           ; picture      
Down w Spell_Animation_Load_Graphics__WIP+4D  mov     [spell_animation_seg], ax                      
Down w Spell_Animation_Load_Graphics__WIP+F0  mov     [spell_animation_seg], ax                      
Down w Spell_Animation_Load_Graphics__WIP+231 mov     [spell_animation_seg], ax                      
Down w Spell_Animation_Load_Graphics__WIP+25B mov     [spell_animation_seg], ax                      
Down r Spell_Animation_Screen__WIP+2B         push    [spell_animation_seg]           ; pict_seg     
Down r Spell_Animation_Screen__WIP+AC         push    [spell_animation_seg]           ; picture      
Down r IDK_Spell_DisjunctOrBind__1+293        push    [spell_animation_seg]           ; picture      
Down r IDK_Spell_DisjunctOrBind__1:loc_BE5FC  push    [spell_animation_seg]           ; picture      
Down r IDK_Spell_DisjunctOrBind__2:loc_BE814  push    [spell_animation_seg]           ; picture      
Down w OVL_LoadGlobalAnim+BB                  mov     [spell_animation_seg], ax                      
Down w OVL_LoadGlobalAnim+FF                  mov     [spell_animation_seg], ax                      
Down r OVL_DrawGlobalAnim+17F                 push    [spell_animation_seg]           ; picture      
Down r OVL_DrawGlobalAnim+20B                 push    [spell_animation_seg]           ; picture      
Down r OVL_DrawGlobalAnim+2AF                 push    [spell_animation_seg]           ; picture      
Down r Spell_Target_Wizard_Screen+63A         push    [spell_animation_seg]           ; picture      
Down r Spell_Target_Wizard_Screen:loc_C0645   push    [spell_animation_seg]           ; picture      
