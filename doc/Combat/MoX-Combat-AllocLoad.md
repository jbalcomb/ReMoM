
SEEALSO:  MoX-EMM.md
SEEALSO:  MoX-EMM-CONTXXX.md
SEEALSO:  MoX-EMM-FIGUREX.md
SEEALSO:  MoX-EMM-TILEXXX.md



## EMM_FIGUREX_Init__HACK()
void EMM_FIGUREX_Init__HACK(int16_t bufpi)
    logical_page = ((bufpi * 3) / 2);
    FIGUREX_OFFSET
    EMS_PFBA = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));
    SET_2B_OFS((EMS_PFBA + offset), SAMB_MEMSIG1, _SA_MEMSIG1);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_MEMSIG2, _SA_MEMSIG2);
    SET_2B_OFS((EMS_PFBA + offset), SAMB_SIZE, 1591);  // 1591 PR, 25456 B
    SET_2B_OFS((EMS_PFBA + offset), SAMB_USED, 1);
...we treat EMS_PFBA as a pointer to whatever is loaded in to the EMS/EMM/"Buffer Space"
Up   r Draw_Picture_To_Bitmap+12E                       mov     ax, [EMS_PFBA]                  ; contains the segment address of the EMS page frame
Up   r BU_CreateImage__SEGRAX+249                       mov     ax, [EMS_PFBA]                  ; contains the segment address of the EMS page frame
Up   r USELESS_Combat_Figure_Load_Compose+2C            mov     ax, [EMS_PFBA]                  ; contains the segment address of the EMS page frame
     r EMM_FIGUREX_Init__HACK+46                        mov     ax, [EMS_PFBA]                  ; contains the segment address of the EMS page frame
Down r Combat_Figure_Compose_USEFULL+1E8                mov     ax, [EMS_PFBA]                  ; contains the segment address of the EMS page frame
Down r Combat_Figure_Load+32                            mov     ax, [EMS_PFBA]                  ; contains the segment address of the EMS page frame





ALLOC.C
    EmmHndl_FIGUREX = Allocate_Space(28672);  // 28 * 16384 EMM Page Size = 458752 B / 16 = 28672 PR
    EmmHndl_TILEXXX = Allocate_Space(3072);   //  3 * 16384 EMM Page Size =  49152 B / 16 =  3072 PR
    _screen_seg = Allocate_Space(gfx_buff_nparas + 515);
    GfxBuf_2400B = Allocate_Space(150);  // 150 PR  2400 B
    UnitDraw_WorkArea = Allocate_Space(60);  // 60 PR, 960 B
    // MoO2  global_combat_data
    global_battle_unit = (struct s_BATTLE_UNIT *)Allocate_Space(8);  // 8 PR, 128 B


void CMB_BaseAllocs__WIP(void)
    // MoO2  _combat_data = Allocate_First_Block()
    battlefield = (struct s_BATTLEFIELD *)Allocate_First_Block(_screen_seg, 348);  // 348 PR, 5568 B
    combat_grid_entities = (struct s_COMBAT_ENTITY *)Allocate_Next_Block(_screen_seg, 482);  // 482 PR, 7712 B
    CMB_Projectiles = Allocate_Next_Block(_screen_seg, 10);  // 10 PR, 160 B
    CMB_Vortex_Array = (struct s_MAGIC_VORTEX *)Allocate_Next_Block(_screen_seg, 9);  // 9 PR, 144 B
    // ¿ drake178:  ; WARNING: these are entirely redundant and will be  reallocated immediately after this! ?
    CMB_ActiveMoveMap = (int8_t *)Near_Allocate_First(504);
    CMB_Path_Costs = Near_Allocate_Next(504);
    CMB_NearBuffer_3 = (int16_t *)Near_Allocate_Next(1008);
    CMB_Path_Xs = Near_Allocate_Next(504);
    CMB_Path_Ys = Near_Allocate_Next(504);
void CMB_LoadResources__WIP(void)
    battle_units = (struct s_BATTLE_UNIT *)Allocate_Next_Block(_screen_seg, 249);
    CMB_IDK_4PR = (int16_t *)Allocate_Next_Block(_screen_seg, 4);  // header + sub-header + 2 PR ... or header + 3 sub-headers?
    CMB_HolyBonusArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    CMB_ResistAllArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    CMB_LeadershipArray = (int16_t *)Allocate_Next_Block(_screen_seg, 6);
    ...spell, wait, done, auto, info, flee buttons.... e.g., _cmbt_spell_button_seg = LBX_Reload_Next(compix_lbx_file__ovr103, 1, _screen_seg);
    combat_enchantments = (int8_t *)Allocate_Next_Block(_screen_seg, 3);
    ...combat_enchantment_icon_segs[15]... e.g, combat_enchantment_icon_segs[ 0] = LBX_Reload_Next(compix_lbx_file__ovr103,  5, _screen_seg);




Tactical_Combat()

    |-> CMB_SetNearAllocs()

CMB_LoadResources()
    |-> CMB_SetNearAllocs()



### CMB_LoadResources()

NOTE: starts with Allocate_Next_Block(), not Allocate_First_Block()

combat_enchantments
    allocates 3 PR 48 B
    zeroes 30 bytes



### CMB_SetNearAllocs()

Up J j_CMB_SetNearAllocs   jmp     CMB_SetNearAllocs               ; resets the dynamic near memory allocation (in the
Up p CMB_LoadResources+3F3 call    near ptr CMB_SetNearAllocs      ; resets the dynamic near memory allocation (in the


Down P Tactical_Combat__STUB+B75  call    j_CMB_SetNearAllocs             ; resets the dynamic near memory allocation (in the
Down P Tactical_Combat__STUB+D2A  call    j_CMB_SetNearAllocs             ; resets the dynamic near memory allocation (in the
Down P Tactical_Combat__STUB+EDF  call    j_CMB_SetNearAllocs             ; resets the dynamic near memory allocation (in the
Down P Tactical_Combat__STUB+1109 call    j_CMB_SetNearAllocs             ; resets the dynamic near memory allocation (in the




CMB_HolyBonusArray[]
CMB_ResistAllArray[]
CMB_LeadershipArray[]

two different function for the allocations
one with 3 PR, one with 6 PR
assuming 1 header, 2 or 5 useable
32 or 80 bytes
16 or 40 2-byte values


dseg:C896 00 00 00 00                                     CMB_LeadershipArray@ dd 0               ; DATA XREF: sub_7EC81+8r ...
dseg:C896                                                                                         ; 3 or 6 LBX_Alloc_Next paragraphs, sandbox segment
dseg:C89A 00 00 00 00                                     CMB_ResistAllArray@ dd 0                ; DATA XREF: CMB_PrepareTurn+195r ...
dseg:C89A                                                                                         ; 3 or 6 LBX_Alloc_Next paragraphs, sandbox segment
dseg:C89E 00 00 00 00                                     CMB_HolyBonusArray@ dd 0                ; DATA XREF: CMB_LoadResources+3Bw ...
dseg:C89E                                                                                         ; 3 or 6 LBX_Alloc_Next paragraphs, sandbox segment
dseg:C8A2 00 00 00 00                                     CMB_IDK_4PR dd 0                        ; DATA XREF: IDK_Combat_Allocate+6Cw ...


in CMB_GetAuras()
    zeroes 7 2-byte values in each
        no code touches CMB_IDK_4PR[]
