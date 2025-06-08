



Toggle_Pages()
    Save_Mouse_State()
    mx = Pointer_X();
    my = Pointer_Y();
    Draw_Fields();
    //
    Check_Mouse_Shape(mx, my);
    Save_Mouse_Off_Page(mx, my);
    Draw_Mouse_Off_Page(mx, my);
    Page_Flip();
    Restore_Mouse_Off_Page();
    Copy_Mouse_Off_To_Mouse_Back();
    Restore_Mouse_State();

GUI_PageFlipPrep()
    mov  ax, ds;  mov  [GUI_DS_Save], ax;
    Save_Mouse_State();

GUI_PageFlip()
    mov  ax, seg dseg;  mov  ds, ax;  mov  ax, [GUI_DS_Save];  mov  ds, ax;
    // 
    Check_Mouse_Shape();
    Save_Mouse_Off_Page();
    Draw_Mouse_Off_Page();
    Page_Flip();
    Restore_Mouse_Off_Page();
    Copy_Mouse_Off_To_Mouse_Back();
    Restore_Mouse_State();


dseg:7898 00 00                                           GUI_DS_Save dw 0                        ; DATA XREF: GUI_PageFlipPrep+3w ...





## Page_Flip()

XREF:
    GUI_PageFlip()
    UU_VGA_InterleavedFlip()
    UU_VGA_CrossSlideFlip()
    UU_VGA_WipeLeft()
    UU_VGA_WipeRight()
    Toggle_Pages()

¿ therefore, each is some form of special page-flip wrapper ?



## Toggle_Pages()

## PageFlip_FX
    switch(PageFlipEffect)
    1: RP_VGA_CutRight()  ...GUI_PageFlipPrep(); GUI_PageFlip();
    3: VGA_MosaicFlip__STUB()
    4: RP_VGA_GrowOutFlip()  ... Toggle_Pages()


## GUI_PageFlipPrep() & GUI_PageFlip()
~== Toggle_Pages(), except no Draw_Fields()

// WZD s26p10
GUI_PageFlipPrep()
// WZD s26p11
GUI_PageFlip()

XREF:
    VGA_MosaicFlip__STUB()
    UU_VGA_SlowMosaicFlip()
    RP_VGA_CutRight()
    UU_VGA_SplitCutOut()

// WZD s26p01
void VGA_MosaicFlip__STUB(void)
// WZD s26p02
// UU_VGA_SlowMosaicFlip()

// WZD s26p03
// RP_VGA_CutRight()
// WZD s26p08
// UU_VGA_SplitCutOut()

VGA_MosaicFlip__STUB()
XREF:
    PageFlip_FX()
    Chancellor_Screen()
    Hire_Hero_Popup()
    OVL_MosaicFlip__STUB()

OVL_MosaicFlip__STUB()
    Set_Page_Off()
    j_Reset_Map_Draw()
    j_Main_Screen_Draw()
    VGA_MosaicFlip__STUB()
XREF:
    Spellbook_Screen()
    Cast_Spell_Overland_Do()
    Cast_Spell_Overland_Do()
    j_OVL_MosaicFlip__STUB()
        EVNT_ShowMessage()
        Hero_LevelUp_Popup()
        Player_Process_Item_Pool()
        Player_Process_Item_Pool()
        Merchant_Popup()
        sAFA06_Anim_EarthLore()
        sub_B0692()
        Cast_EnchantRoad()
        IDK_Spell_DisjunctOrBind()
        IDK_SummonAnim()
        IDK_Spell_Cityscape_2()
        WIZ_GlobalSpellAnim()
        IDK_DiploScrn()


UU_VGA_InterleavedFlip()
    UU_VGA_InterleaveOut()
    UU_VGA_InterleaveIn()
    Page_Flip()



## MoO1

PageFlip_FX()
    PageFlipEffect: {0,1,2}
        0:
        1:  ui_draw_finish_wipe_anim()  AKA  UU_VGA_SquaresFlip()
        2:


uidraw.c
ui_draw_finish_wipe_anim()

// UU_VGA_CopyRect()
static void ui_draw_finish_wipe_anim_do(int x, int y, int f)

static void ui_draw_finish_wipe_anim(void)

// UU_VGA_CopyLine()
static void ui_draw_copy_line(int x0, int y0, int x1, int y1)



## MoO2
00117511 cmp     draw_fields_flag, e_ST_FALSE

Draw_Visible_Fields_Back()
    |-> Draw_Field()

e.g.,
Detailed_View_Ship_()
    Get_Auto_Func(&_k_save, 1)
    Set_Page_Back()
    _ship_frame--
    Execute_Auto_Function_(_k_save)
    Draw_Visible_Fields_Back()
    Copy_Back_To_Off()
    _TOGGLE_PAGES_()

Draw_Visible_Fields()
    |-> Draw_Field()








"draw_page_num"
?"draw" as in on-screen or off-screen?
Page_Flip() sets draw_page_num to (1 - draw_page_num) and uses this switched draw_page_num to calculate and set the address for the CRT Controller to *display* the video memory
so, ...
    "draw_page_num" is the *index* of the video page that is being *drawn* to the *screen*/*display*
    "draw" ~== "active" / "visible" / "on-screen"



main()
    Screen_Control()
        ..._Screen()
            ..._Screen_Draw()
            Toggle_Pages()
                Draw_Fields()
                Save_Mouse()
                Draw_Mouse()
                Page_Flip()
                    VBB = {vbb0,vbb1}

                Restore_Mouse()
                Copy_Mouse()



...each screen has a draw function that draws the base screen
...fields have their own draw functions to accomodate changes - up/down buttons, text inputs, etc.
...once the *final* screen is ready to be presented/displayed/rendered...
...the area of the screen where the mouse cursor image is to be drawn is saved to the mouse background buffer
...then the mouse cursor image is drawn
...finally, conclusively, the video pages are switched
...so, here, Restore_Mouse() is drawing on the *current* screen what was just saved from what was just made to be the *previous* screen



Copy_Mouse()
    copies CR_Save_DSP to CR_Save_RSP

Save_DSP(), Draw_DSP(), Restore_DSP() all use (1 - draw_page_num)


Save_DSP()
    dst: CR_Save_DSP

Draw_DSP()
    src: mouse_palette

Restore_DSP()
    src: CR_Save_RSP
    dst: (1 - draw_page_num)

Restore_RSP()
    src: CR_Save_RSP
    dst: (draw_page_num)
