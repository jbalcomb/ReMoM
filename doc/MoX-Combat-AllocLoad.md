



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
