TITLE _o57p01.asm LBX_Tables_Init
; in MOM_DEF.H

.MODEL LARGE, C

.CODE
;segment ovr057 para public 'OVERLAY' use16
;    assume cs:ovr057
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_Tables_Init

proc LBX_Tables_Init far

    GFX_Buffer_Size = word ptr 6

    push bp
    mov  bp, sp
    push si

    mov ax, 1
    push ax
    mov ax, offset cnst_EMM_FigHndName
    push ax
    mov ax, 1Ch
    push ax
    call EMM_CreateHandle
    add sp, 6
    mov [EMM_FIGUREX_Handle], ax
    mov ax, 1
    push ax
    mov ax, offset cnst_EMM_TileHndName
    push ax
    mov ax, 3
    push ax
    call EMM_CreateHandle
    add sp, 6
    mov [EMM_TILEX_Handle], ax
    mov [World_Data_Paras], 0DBFh
    mov ax, [bp+GFX_Buffer_Size]
    add ax, 203h
    push ax
    call LBX_Alloc_Space
    pop cx
    mov [LBX_Sandbox_Segment], ax
    mov ax, 96h
    push ax
    call LBX_Alloc_Space
    pop cx
    mov [GUI_SmallWork_IMG@], ax
    mov ax, 27Bh
    push ax
    call LBX_Alloc_Space
    pop cx
    mov [GFX_Swap_Seg], ax
    push [World_Data_Paras]
    call LBX_Alloc_Space
    pop cx
    mov si, ax
    mov [World_Data@], si
    mov ax, 2CAh
    push ax
    push si
    call LBX_Alloc_First
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Cities@+2], dx
    mov [word ptr TBL_Cities@], ax
    mov ax, 25Ah
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Maps@+2], dx
    mov [word ptr TBL_Maps@], ax
    mov ax, 0Eh
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr UU_TBL_2_@+2], dx
    mov [word ptr UU_TBL_2_@], ax
    mov ax, 0Eh
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr UU_TBL_1_@+2], dx
    mov [word ptr UU_TBL_1_@], ax
    mov ax, 12Eh
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Landmasses@+2], dx
    mov [word ptr TBL_Landmasses@], ax
    mov ax, 12Eh
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Terr_Specials@+2], dx
    mov [word ptr TBL_Terr_Specials@], ax
    mov ax, 12Eh
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_TerrainFlags@+2], dx
    mov [word ptr TBL_TerrainFlags@], ax
    mov ax, 12Eh
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Scouting@+2], dx
    mov [word ptr TBL_Scouting@], ax
    mov ax, 13h
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_Visibility_Arcanus@+2], dx
    mov [word ptr CRP_Visibility_Arcanus@], ax
    mov ax, 13h
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_Visibility_Myrror@+2], dx
    mov [word ptr CRP_Visibility_Myrror@], ax
    push si
    call LBX_Alloc_GetFree
    pop cx
    dec ax
    push ax
    push si
    call LBX_Alloc_Next
    pop cx
    pop cx
    mov [World_Data_Left@], ax
    mov ax, 278h
    push ax
    call EMM_LBX_AllocFirst
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_TBL_TempMoveMap_EMS@+2], dx
    mov [word ptr CRP_TBL_TempMoveMap_EMS@], ax
    mov ax, 70Ah
    push ax
    call EMM_LBX_AllocNext
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_MoveMaps_EMS@+2], dx
    mov [word ptr TBL_MoveMaps_EMS@], ax
    mov ax, 12Dh
    push ax
    call EMM_LBX_AllocNext
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_TBL_SharedTiles_EMS@+2], dx
    mov [word ptr CRP_TBL_SharedTiles_EMS@], ax
    mov ax, 12Dh
    push ax
    call EMM_LBX_AllocNext
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_TBL_Catchments_EMS@+2], dx
    mov [word ptr CRP_TBL_Catchments_EMS@], ax
    mov ax, 409h
    push ax
    call EMM_LBX_AllocNext
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_TBL_OvlMovePathsEMS@+2], dx
    mov [word ptr CRP_TBL_OvlMovePathsEMS@], ax
    mov ax, 1Ch
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_HeroStats_P0_@+2], dx
    mov [word ptr TBL_HeroStats_P0_@], ax
    mov ax, 1Bh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_HeroStats_P1_@+2], dx
    mov [word ptr TBL_HeroStats_P1_@], ax
    mov ax, 1Bh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_HeroStats_P2_@+2], dx
    mov [word ptr TBL_HeroStats_P2_@], ax
    mov ax, 1Bh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_HeroStats_P3_@+2], dx
    mov [word ptr TBL_HeroStats_P3_@], ax
    mov ax, 1Bh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_HeroStats_P4_@+2], dx
    mov [word ptr TBL_HeroStats_P4_@], ax
    mov ax, 1Bh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_HeroStats_P5_@+2], dx
    mov [word ptr TBL_HeroStats_P5_@], ax
    mov ax, 7ECh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Units@+2], dx
    mov [word ptr TBL_Units@], ax
    mov ax, 8
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr CRP_Active_Unit@+2], dx
    mov [word ptr CRP_Active_Unit@], ax
    mov ax, 5Ch
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Nodes@+2], dx
    mov [word ptr TBL_Nodes@], ax
    mov ax, 3
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Fortresses@+2], dx
    mov [word ptr TBL_Fortresses@], ax
    mov ax, 3
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Towers@+2], dx
    mov [word ptr TBL_Towers@], ax
    mov ax, 15Fh
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Encounters@+2], dx
    mov [word ptr TBL_Encounters@], ax
    mov ax, 7
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Events@+2], dx
    mov [word ptr TBL_Events@], ax
    mov ax, 25h
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Hero_Names@+2], dx
    mov [word ptr TBL_Hero_Names@], ax
    mov ax, 1B1h
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Items@+2], dx
    mov [word ptr TBL_Items@], ax
    mov ax, 11h
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Premade_Items@+2], dxa
    mov [word ptr TBL_Premade_Items@], ax
    mov ax, 1E5h
    push ax
    call LBX_Alloc_Space
    pop cx
    push ax
    call MK_FAR
    pop cx
    mov [word ptr TBL_Spell_Data@+2], dx
    mov [word ptr TBL_Spell_Data@], ax
    mov ax, 3Ch
    push ax
    call LBX_Alloc_Space
    pop cx
    mov [CRP_UnitDraw_WorkArea@], ax
    mov ax, 15Eh
    push ax
    call LBX_Alloc_Space
    pop cx
    mov [CRP_SND_Music_Segment], ax
    mov [Wiz5_Spell_00h@], (offset TBL_Wizards.Spells_Known+17E8h)
    mov [Wiz5_Spell_0Ch@], (offset TBL_Wizards.Spells_Known+17F4h)
    mov [Wiz5_Spell_18h@], (offset TBL_Wizards.Spells_Known+1800h)
    mov [Wiz5_Spell_28h@], (offset TBL_Wizards.Spells_Known+1810h)
    mov [Wiz5_Spell_3Ch@], (offset TBL_Wizards.Spells_Known+1824h)
    mov [Wiz5_Spell_50h@], (offset TBL_Wizards.Spells_Known+1838h)
    mov [Wiz5_Spell_64h@], (offset TBL_Wizards.Spells_Known+184Ch)
    mov [Wiz5_Spell_78h@], (offset TBL_Wizards.Spells_Known+1860h)
    mov [Wiz5_Spell_8Ch@], (offset TBL_Wizards.Spells_Known+1874h)
    mov [Wiz5_Spell_A0h@], (offset TBL_Wizards.Spells_Known+1888h)
    mov [Wiz5_Spell_B4h@], (offset TBL_Wizards.Spells_Known+189Ch)
    mov [Wiz5_Spell_C8h@], (offset TBL_Wizards.Spells_Known+18B0h)
    mov [Wiz5_Spell_D4h@], (offset TBL_Wizards.Spells_Known+18BCh)
    mov [Wiz5_Spell_E0h@], (offset TBL_Wizards.Spells_Known+18C8h)
    mov ax, 1
    push ax
    mov ax, offset cnst_EMM_ContH_Name
    push ax
    mov ax, 4
    push ax
    call EMM_CreateHandle
    add sp, 6
    mov [EMM_ContXXX_H], ax
    
    pop si
    pop bp
    retf

endp LBX_Tables_Init

;ends ovr057

end
