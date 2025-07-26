
Combat Enchantments
Enchantment Windows
Defender Enchantment Window
Attacker Enchantment Window

¿ diferent from what shows up in the Combat Information Window ?



TBL_CE_Display_Data ==> combat_enchantment_icon_data
s_CE_Display ==> s_COMBAT_ENCHANTMENT_ICON_DATA
s_Active_CE ==> s_COMBAT_ENCHANTMENT_ICON
IMG_CMB_TrueLight ==> combat_enchantment_icon_segs

CMB_DEFR_CE_Count ==> defender_enchantment_count
CMB_ATKR_CE_Count ==> attacker_enchantment_count
CMB_DEFR_CE_Wnd ==> defender_enchantments
CMB_ATKR_CE_Wnd ==> attacker_enchantments
CMB_DEFR_First_CE ==> defender_enchantment_first
CMB_ATKR_First_CE ==> attacker_enchantment_first
CMB_DEFR_CE_Labels == defender_enchantment_fields
CMB_ATKR_CE_Labels ==> attacker_enchantment_fields

CMB_CreateCELabels__WIP() ==> Add_Combat_Enchantment_Fields()


How to hack in some enchantments?
MoM.C
Screen_Control()
case scr_Continue:
/* HACK: need combat_enchantments[] */ _players[HUMAN_PLAYER_IDX].Globals[ETERNAL_NIGHT] = 1;



Where does combat_enchantments[] get populated?
    CMB_LoadResources__WIP()
        combat_enchantments = (int8_t *)Allocate_Next_Block(_screen_seg, 3);
        for(itr = 0; itr < 30; itr++)
            combat_enchantments[itr] = ST_FALSE;
    Init_Battlefield_Effects()
        if(battlefield->city_enchantments[CLOUD_OF_SHADOW] > 0)
            combat_enchantments[DARKNESS_DFNDR] = 2;
        if(battlefield->city_enchantments[HEAVENLY_LIGHT] > 0)
            combat_enchantments[TRUE_LIGHT_DFNDR] = 2;
        for(itr_players = 0; itr_players < _num_players; itr_players++)
            if(_players[itr_players].Globals[ETERNAL_NIGHT] > 0)
                YNM combat_enchantments[DARKNESS_ATTKR] = 3;
                YNM combat_enchantments[DARKNESS_ATTKR] = 3;
    Combat_Structure()
        if(_CITIES[OVL_Action_Structure].enchantments[HEAVENLY_LIGHT] > 0)
            combat_enchantments[TRUE_LIGHT_DFNDR] = 2;
        if(_CITIES[OVL_Action_Structure].enchantments[CLOUD_OF_SHADOW] > 0)
            combat_enchantments[DARKNESS_DFNDR] = 2;

Eternal Night
Cloud of Shadow
Heavenly Light
True Light
Darkness

***¿ the balance of the combat enchantments are combat-only ?***





combat enchantment icon index

combat enchantment help index

CMB_LoadResources__WIP()
    loads the LBX entries into combat_enchantment_icon_segs[]

CMB_CE_Refresh__WIP()
    checks combat_enchantments[]
    gets index from combat_enchantment_icon_data[] to index into combat_enchantment_icon_segs[]
    assigns the icon seg to CMB_ATKR_CE_Wnd[].icon_seg

icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data[].icon_idx]
help_idx = combat_enchantment_icon_data[].help_idx

Add_Combat_Enchantment_Fields()
    CMB_ATKR_CE_Labels
    Add_Picture_Field()
    CMB_ATKR_CE_Wnd[(CMB_ATKR_First_CE + itr)].icon_seg
    CMB_ATKR_CE_Wnd[(CMB_ATKR_First_CE + itr)].help_idx








Tactical_Combat_Draw()
_help_entries {9, ..., 16}
CE_Window_Left
First_CE_Help_Entry
CMB_ATKR_First_CE
CMB_ATKR_CE_Count
CMB_DEFR_First_CE
CMB_DEFR_CE_Count

if(_combat_attacker_player == _human_player_idx)

if(_combat_defender_player == _human_player_idx)

draws the attacker enchantments and sets the help entries
draws the defender enchantments and sets the help entries



## CMB_ATKR_CE_Count

    Tactical_Combat__WIP:loc_76689 cmp     _SI_itr, [CMB_ATKR_CE_Count]
    Tactical_Combat__WIP+7E4       cmp     [CMB_ATKR_CE_Count], 4      
    CMB_CE_Refresh__WIP+8          mov     [CMB_ATKR_CE_Count], 0      
    CMB_CE_Refresh__WIP+7F         mov     bx, [CMB_ATKR_CE_Count]     
    CMB_CE_Refresh__WIP+95         mov     bx, [CMB_ATKR_CE_Count]     
    CMB_CE_Refresh__WIP+A1         inc     [CMB_ATKR_CE_Count]         
    CMB_CreateCELabels+61          cmp     ax, [CMB_ATKR_CE_Count]     
    Tactical_Combat_Draw+CA        cmp     ax, [CMB_ATKR_CE_Count]     


## CMB_DEFR_CE_Count

    Tactical_Combat__WIP:loc_766CC cmp     _SI_itr, [CMB_DEFR_CE_Count]
    Tactical_Combat__WIP+827       cmp     [CMB_DEFR_CE_Count], 4      
    CMB_CE_Refresh__WIP+E          mov     [CMB_DEFR_CE_Count], 0      
    CMB_CE_Refresh__WIP+B7         mov     bx, [CMB_DEFR_CE_Count]     
    CMB_CE_Refresh__WIP+CD         mov     bx, [CMB_DEFR_CE_Count]     
    CMB_CE_Refresh__WIP+D9         inc     [CMB_DEFR_CE_Count]         
    CMB_CreateCELabels+C8          cmp     ax, [CMB_DEFR_CE_Count]     
    Tactical_Combat_Draw+146       cmp     ax, [CMB_DEFR_CE_Count]     



## Add_Combat_Enchantment_Fields()
depends on these already being populated
    CMB_ATKR_First_CE
    CMB_ATKR_CE_Count
    CMB_DEFR_First_CE
    CMB_DEFR_CE_Count
    CMB_ATKR_CE_Wnd[].icon_seg
    CMB_ATKR_CE_Wnd[].help_idx


## CMB_CE_Refresh__WIP()

struct  s_Active_CE CMB_DEFR_CE_Wnd[NUM_COMBAT_ENCHANTMENTS];
struct  s_Active_CE CMB_ATKR_CE_Wnd[NUM_COMBAT_ENCHANTMENTS];



## CMB_DEFR_CE_Wnd & CMB_ATKR_CE_Wnd

Where do CMB_DEFR_CE_Wnd and CMB_ATKR_CE_Wnd get populated?
CMB_CE_Refresh__WIP()

CMB_DEFR_CE_Wnd
    CMB_CE_Refresh__WIP+BF     mov     [word ptr CMB_DEFR_CE_Wnd.icon_seg+bx], ax      
    Add_Combat_Enchantment_Fields+9D push    [word ptr CMB_DEFR_CE_Wnd.icon_seg+bx]; pict_seg
    Tactical_Combat_Draw+102   push    [word ptr CMB_DEFR_CE_Wnd.icon_seg+bx]; picture 
    CMB_CE_Refresh__WIP+D5     mov     [CMB_DEFR_CE_Wnd.help_idx+bx], ax            
    Add_Combat_Enchantment_Fields+8B push    [CMB_DEFR_CE_Wnd.help_idx+bx] ; help         
    Tactical_Combat_Draw+129   mov     ax, [CMB_DEFR_CE_Wnd.help_idx+bx]            

CMB_ATKR_CE_Wnd
    CMB_CE_Refresh__WIP+87     mov     [word ptr CMB_ATKR_CE_Wnd.icon_seg+bx], ax      
    Add_Combat_Enchantment_Fields+36 push    [word ptr CMB_ATKR_CE_Wnd.icon_seg+bx]; pict_seg
    Tactical_Combat_Draw+86    push    [word ptr CMB_ATKR_CE_Wnd.icon_seg+bx]; picture 
    CMB_CE_Refresh__WIP+9D     mov     [CMB_ATKR_CE_Wnd.help_idx+bx], ax            
    Add_Combat_Enchantment_Fields+24 push    [CMB_ATKR_CE_Wnd.help_idx+bx] ; help         
    Tactical_Combat_Draw+AD    mov     ax, [CMB_ATKR_CE_Wnd.help_idx+bx]            

icon_seg = combat_enchantment_icon_segs[combat_enchantment_icon_data.icon_idx]
help_idx = combat_enchantment_icon_data.help_idx


CMB_LoadResources__WIP()
    ...
    // IMG_CMB_TrueLight = LBX_Reload_Next(compix_lbx_file__ovr103, 5, _screen_seg);
    combat_enchantment_icon_segs[0] = LBX_Reload_Next(compix_lbx_file__ovr103, 5, _screen_seg);
    ...
dseg:C4FE 00 00                                           IMG_CMB_TrueLight@ dw 0                 ; DATA XREF: CMB_CE_Refresh__WIP+7Br ...
dseg:C500 00 00                                           IMG_CMB_Darkness@ dw 0                  ; DATA XREF: CMB_LoadResources__WIP+15Ew
dseg:C502 00 00                                           IMG_CMB_WarpReality@ dw 0               ; DATA XREF: CMB_LoadResources__WIP+175w
dseg:C504 00 00                                           IMG_CMB_BlackPrayer@ dw 0               ; DATA XREF: CMB_LoadResources__WIP+18Cw
dseg:C506 00 00                                           IMG_CMB_Wrack@ dw 0                     ; DATA XREF: CMB_LoadResources__WIP+1A3w
dseg:C508 00 00                                           IMG_CMB_MetalFires@ dw 0                ; DATA XREF: CMB_LoadResources__WIP+1BAw
dseg:C50A 00 00                                           IMG_CMB_Prayer@ dw 0                    ; DATA XREF: CMB_LoadResources__WIP+1D1w
dseg:C50C 00 00                                           IMG_CMB_HighPrayer@ dw 0                ; DATA XREF: CMB_LoadResources__WIP+1E8w
dseg:C50E 00 00                                           IMG_CMB_Terror@ dw 0                    ; DATA XREF: CMB_LoadResources__WIP+1FFw
dseg:C510 00 00                                           IMG_CMB_CallLightng@ dw 0               ; DATA XREF: CMB_LoadResources__WIP+216w
dseg:C512 00 00                                           IMG_CMB_CounterMgc@ dw 0                ; DATA XREF: CMB_LoadResources__WIP+22Dw
dseg:C514 00 00                                           IMG_CMB_MassInvis@ dw 0                 ; DATA XREF: CMB_LoadResources__WIP+244w
dseg:C516 00 00                                           IMG_CMB_Entangle@ dw 0                  ; DATA XREF: CMB_LoadResources__WIP+25Bw
dseg:C518 00 00                                           IMG_CMB_ManaLeak@ dw 0                  ; DATA XREF: CMB_LoadResources__WIP+272w
dseg:C51A 00 00                                           IMG_CMB_Blur@ dw 0                      ; DATA XREF: CMB_LoadResources__WIP+289w
loaded in the same order as seen in the heap
nonconsequtive ordering in the LBX file
{ 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 41, 60, 79, 80 }
COMPIX.LBX, 005  "COMENCHI"   "light"
COMPIX.LBX, 006  "COMENCHI"   "darkness"
COMPIX.LBX, 007  "COMENCHI"   "warp reality"
COMPIX.LBX, 008  "COMENCHI"   "black curse"
COMPIX.LBX, 009  "COMENCHI"   "wrack"
COMPIX.LBX, 010  "COMENCHI"   "metal fires"
COMPIX.LBX, 011  "COMENCHI"   "prayer"
COMPIX.LBX, 012  "COMENCHI"   "high prayer"
COMPIX.LBX, 013  "COMENCHI"   "terror"
COMPIX.LBX, 014  "COMENCHI"   "lightning"
COMPIX.LBX, 015  "COMENCHI"   "counter magic"
COMPIX.LBX, 041  "MASINVIS"   "mass invis"
COMPIX.LBX, 060  "COMENCHI"   "entangle icon"
COMPIX.LBX, 079  "COMENCHI"   "mana leek"
COMPIX.LBX, 080  "COMENCHI"   "blur"
~ "combat enchantment icon"
combat_enchantment_icon_segs

struct s_CE_Display combat_enchantment_icon_data[NUM_COMBAT_ENCHANTMENTS] = 
{
    {  0, HLP_LIGHT             },
    {  1, HLP_DARKNESS          },
    {  2, HLP_WARP_REALITY      },
    {  3, HLP_BLACK_PRAYER      },
    {  4, HLP_WRACK             },
    {  5, HLP_METAL_FIRES       },
    {  6, HLP_PRAYER            },
    {  7, HLP_HIGH_PRAYER       },
    {  8, HLP_TERROR            },
    {  9, HLP_CALL_LIGHTNING    },
    { 10, HLP_COUNTER_MAGIC     },
    { 11, HLP_MASS_INVISIBILITY },
    { 12, HLP_ENTANGLE          },
    { 13, HLP_MANA_LEAK_2       },
    { 14, HLP_BLUR              }
};
same order as array and load

    HLP_LIGHT               = 600,
    HLP_DARKNESS            = 601,
    HLP_WARP_REALITY        = 602,
    HLP_BLACK_PRAYER        = 603,
    HLP_WRACK               = 604,
    HLP_METAL_FIRES         = 605,
    HLP_PRAYER              = 606,
    HLP_HIGH_PRAYER         = 607,
    HLP_TERROR              = 608,
    HLP_CALL_LIGHTNING      = 609,
    HLP_COUNTER_MAGIC       = 610,
    HLP_MASS_INVISIBILITY   = 611,
    ...
    HLP_ENTANGLE            = 632,
    ...
    HLP_MANA_LEAK_2         = 767,
    ...
    HLP_BLUR                = 797,
different out-of-order than the LBX entries





##### Naming Things is Hard


¿ COMENCHI ~== combat enchantment icon(s) ?


two enchantment windows
one on the left for the defender
one on the right for the attacker
Combat enchantments
Combat enchantments that are active for either side
active
...left, right, either/both...



COMPIX.LBX  "COMENCHI"



Page 90  (PDF Page 95)

Defender's enchantment window
Attacker's enchantment window


Page 91  (PDF Page 96)

Underneath the combat grid (the battlefield) are two enchantment windows:  
  one on the left for the defender (this can be a neutral city, node, enemy wizard, etc.)  
  and one on the right for the attacker.  
Combat enchantments that are active for either side (such as darkness and true light) are indicated in this window.  

Between the two enchantment windows are three areas: the spell information window, the action window and the active unit window.  
