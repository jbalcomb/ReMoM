


MoO2

Message_Box_Startup_()
Message_Box_Cleanup_()
Push_Block()
Pop_Block()
Save_Pushed_Blocks()
Restore_Pushed_Blocks()

Message_Box_Startup_()
    Push_Block(_screen_seg)
Message_Box_Cleanup_()
    Pop_Block(_screen_seg)

Warning_Box_() sets _warning_box = ST_TRUE and falls through to Do_Message_Box_()

Do_Message_Box_()
    ...
    ...
    ...
    Assign_Auto_Function(Draw_Animating_Message_Box_, 2)




Save_ScreenSeg()
    Confirmation_Box()
    Warn()
    Selection_Box()
    Notify_Load()
    j_Save_ScreenSeg()
        Do_Lair_Confirm()
        Raze_City_Prompt()
        CMB_CallChaos()
        CMB_CallChaos()





Notification Message

ST: ~ "Notify"


Text Box
Message Box

Confirm
Select
Notify
Warn
Help


WIZARDS.EXE  ovr149


Notify1
Notify2

¿ BUGBUG Notify2() sets Notify2_Draw() as its auto function, but calls Notify1_Draw() directly ?



ush    [city_built_bldg_idx]
push    [_city_idx]
mov     ax, 101
push    ax
mov     ax, 5
push    ax
City_Built_Building_Message();


Next_Turn_Proc()
    |-> City_Built_Building_Message()
        |-> Notify2()

Notify2(160, 60, tb_Green, GUI_String_1, 0, city_new_build_notify_grass_seg, 0, 8, bldg_picts_seg[bldg_idx],    bitm_x, bitm_y, 0, 0);

void Notify2(int16_t UU_box_center_x, int16_t box_top_y, int16_t type, char * message_text, int16_t box_border, SAMB_ptr notiy_pict_seg, int16_t pict1_x, int16_t pict1_y, SAMB_ptr pict2, int16_t pict2_x, int16_t pict2_y, int16_t do_cslide, int16_t do_dim_back)

## Notify2()
    Parameters
        UU_box_center_x
        box_top_y
        type
        message_text
        box_border
        pict1
        pict1_x
        pict1_y
        pict2
        pict2_x
        pict2_y
        do_cslide
        do_dim_back

type and pict1 gets passed to Notify_Load()
    notify_color controls which is loaded - {G_NOTIFY, F_NOTIFY, B_NOTIFY, BRNOTIFY} "notify", "pict box", and "gem box"


In-Code Usage:

    message_box_text = message_text;
    notify_pict1 = pict1;
    notify_pict2 = pict2;
    notify_type = type;
    notify_box_border = box_border;
    notify_pict1_x = pict1_x;
    notify_pict1_y = pict1_y;
    notify_pict2_x = pict2_x;
    notify_pict2_y = pict2_y;
    notify_color_slide_cycle = 0;


CSlide is assigned to `notify_color_slide`, along with reseting the state (`notify_color_slide_cycle = 0`)
    the state is also reset a few lines later, in a block that looks like 'Notification Box Startup'

BGShade is hard-coded to ST_FALSE
    if ST_TRUE, Set_Page_Off(); Gradient_Fill(0,0,319,199,3);
    this is the same thing the Pop-Ups do when they want to use the current screen as their dimmied/darkened background
    ~== 'do darken darken background flag'
this would seem to overlap with `notify_draw offscr`, which controls whether Copy_On_To_Off_Page() is called (which is right before the darken)

m/g state:
    notify_color_slide is set in the do function
        in Notify1_Draw(), it tests against 1 then test against 2
        in Notify2_Draw(), it only tests against 1
        in Lair_Confirm(), it only tests against 1  it sets it to 1 if the lair_type is (1,3) {chaos node, nature node, sorcery node}
        EZ_TreasureDialog() calls Notify1() wtih CSlide = 2
        ...but, that just cycles %8 instead of the *normal* %11
        ¿ are the node auras 8 frames and/or also %8 ?
        ¿ what does this even actually apply to ?
        ¿ what does it actually do ?  I'm not getting much from "VGA_SlideColors" or even "slides the colors within the specified range by the shift amount"

assumes use of magic type icon?
    indexes GUI_Familiar_IMGLeft[] to get an x value
        if < 0 subtracts from 6, but negative, and subtracting, so actually adding
    default is 6
    related to over-drawing?

## Notify2_Draw()

message_box_x
    used to set the x for message text
    used  in the first draw, which is a Clipped_Draw() of the notify_seg[]


notify2_text_x1[5] = { 70,  73,  62,  68, 70};  (Notify1() does +50 if notify_type is 2) { 70,  73,  62,  68, 70} - 50 = { 20,  23,  12,  18, 20}
    ¿ just for Notify2 ?
    x offsets for start of text  (dependent on the start and width of the magic type icon?)
then, ...
    mov     _SI_xadd, ax                    ; ¿ text x2 ? box x + x offset to text + text width
    this is used as x2 for the subsequent call to Fill()
        after the Clipped_Draw() of the notify_seg[]
        after the FLIC_Draw() of the notify_magic_picts_seg[]


notify2_text_x1[5] = { 70,  73,  62,  68, 70};  
    Notify1() does +50 if notify_type is 2  (and does x-23,y-19 for the icon draw)
{ 70,  73,  62,  68, 70} - 50 = { 20,  23,  12,  18, 20}
GUI_Familiar_IMGLeft[5] =       { -2,   0, -22,   8,  2}
diff                            { 22,  23,  32,  10, 18}
if <0 6 -                       {  8,   6,  28,   6,  6}

text x2 + 7 is pict1/pict2 x1
as-is, pict1 is new build background and pict2 is bldg pict
pict1/pict2 x1 is message_box_y + pict1_y/pict2_y
for the background, pict1 x,y should be 0,0?  same as Fill()
for the bldg pict, pict2 x,y should be half the padding, to center the image in the space  ¿ "centered-offset" ?


So, ...
    notify_type is kinda more like a 'notify type'
    if == 2, Notify1()'s behavior includes the 'notify magic type icon'
        but, it doesn't use the icon x,y arrays
    if == 3, Notify1() uses different text outline/shadow colors
        also does alias color instead of notify_text_colors[]
    ...
    used to index
        notify_seg[4]  ... blue - red - green - brown
        notify_gem_box_seg[4]
        notify_pict_box_seg[4]
        notify_shadow_colors[4]
        notify_text_colors[4]

the alias color seems like a "brow", even for 2/green
the outline/shadow color is like the "dark turquoise" from the Prod Scr/UV

¿ ~ Notify2() is (just) to do extra-special handling for the 'notify magic type icons' ?

always draws the pict border, but both picts are optional?

Notify1() is centered, Notify2() is not
Notify1() does not call Update_Remap_Gray_Palette(), which is need for the shadow on the mascot



### Drawings
notification background
realm icon
pict border {pict box, gem box}
pict1
pict2

pict1 is what gets passed to Notify_Load()
so, it's special-different than pict2?
also, in the draw, it gets its palette  (in addition to the palette from notify_seg[])





##### Naming Things is Hard

familiar
creature
animal
representaive
avatar
emoji
mascot


Life magic      white ankhs
Death magic     purple skulls
Chaos magic     red fireballs
Nature magic    green trees
Sorcery magic   blue diamonds

Life magic      (white)
Death magic     (black)
Chaos magic     (red)
Nature magic    (green)
Sorcery magic   (blue)
Arcane magic    (gray)


spell picks screen
spell books (ranks)
eleven "spell picks"
"bookshelves"
bookshelf slots
magic types (colors)
spell book bindings


There are five types of magic in which wizards can specialize:
In addition to the five magic types noted above, there is a sixth type,
arcane magic, that requires no spell ranks to learn. Arcane magic is
available to all spell casters.

Spell ranks are selected or deselected by clicking on the "bookshelves"

six magic types (colors)

Every wizard in Master of Magic has a spell library which represents those spells that the wizard can research and learn.  
This library is composed of spell books (called ranks).  
Each spell rank contains cryptic information on spells of a specific type of magic (life, death, chaos, nature or sorcery).  
