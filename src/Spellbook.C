/*
    WIZARDS.EXE
        ovr117
*/

#include "MoM.H"
#include "Spellbook.H"

#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:9448                                                 ¿ BEGIN: BEGIN: Spellbook (!Screen) - Uninitialized Data ?

// WZD dseg:9448 00 00                                           SBK_Candidate_Page dw 0                 ; DATA XREF: G_WLD_StaticAssetRfrsh+67w ...

// WZD dseg:944A
int16_t SBK_SomePageSaveVar;

// WZD dseg:944C 00 00                                           CMB_SpellBookPage dw 0                  ; DATA XREF: G_WLD_StaticAssetRfrsh+61w ...
// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0

// WZD dseg:9452
SAMB_ptr _spellbook_small_text;

// WZD dseg:9454
// _spellbook_small_symbols SPBK_ICONS <0>
SAMB_ptr _spellbook_small_symbols[5];

// WZD dseg:9460 00 00                                           IMG_SPL_BigBook@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           IMG_SPL_BigRDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           IMG_SPL_BigLDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...

// WZD dseg:9468
SAMB_ptr _spellbook_small_right_corner_seg;

// WZD dseg:946A
SAMB_ptr _spellbook_small_left_corner_seg;

// WZD dseg:946C 00 00                                           word_3FF0C dw 0                         ; DATA XREF: sub_BECD9+23w ...

// WZD dseg:946E
SAMB_ptr _spellbook_small_seg;



// WZD dseg:C8FA                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?

// WZD dseg:C8FA 00 00                                           SBK_BookPages@ dw 0                     ; DATA XREF: CMB_ShowSpellbook+23Fr ...
// WZD dseg:C8FA                                                                                         ; LBX_NearAlloc_Next pointer
// WZD dseg:C8FC 00 00                                           SBK_NewSpellAnim_Stg dw 0               ; DATA XREF: CMB_SetTargetCursor+24Dr ...
// WZD dseg:C8FC                                                                                         ; (0-7), also combat casting cursor anim stage (0-4)

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
SAMB_ptr IMG_SBK_BookRealms[6];

// WZD dseg:C8FE                                                                                         ; decoded image headers appended to LBX_Sandbox_Segment
// WZD dseg:C90A 00 00                                           SBK_BookManaLimit dw 0                  ; DATA XREF: Magic_Screen+481w ...
// WZD dseg:C90A                                                                                         ; the predicted amount of mana that the wizard will
// WZD dseg:C90A                                                                                         ; be able to channel into a spell each turn?
// WZD dseg:C90A                                                                                         ; (lower of mana+manaperturn and skill+heroskill)
// WZD dseg:C90A                                                                                         ; in combat, the highest castable cost
// WZD dseg:C90C 00 00                                           SBK_Research_Incomes dw 0               ; DATA XREF: SBK_BuildSpellbook+32w ...
// WZD dseg:C90C                                                                                         ; NearAlloc_First!
// WZD dseg:C90E 00 00                                           WIZ_ManaPerTurn dw 0                    ; DATA XREF: Magic_Screen+471r ...
// WZD dseg:C90E                                                                                         ; used by spellbook functions
// WZD dseg:C910 00 00 00 00                                     SBK_SpellList@ dd 0                     ; DATA XREF: SBK_CountSpellGroups+13r ...
// WZD dseg:C910                                                                                         ; 0Dh LBX_Alloc_Space paragraphs (first or next)
// WZD dseg:C910                                                                                         ; NOT BIG ENOUGH TO HOLD MORE THAN 104 SPELLS!

// WZD dseg:C914
int16_t SBK_BookPageCount;

// WZD dseg:C916 00 00                                           SBK_Group_6_Count dw 0                  ; DATA XREF: SBK_CountSpellGroups:loc_9A68Fw ...
// WZD dseg:C916                                                                                         ; Combat Spells or Sorcery
// WZD dseg:C918 00 00                                           SBK_Group_5_Count dw 0                  ; DATA XREF: SBK_CountSpellGroups:loc_9A683w ...
// WZD dseg:C918                                                                                         ; Enchantments or Life
// WZD dseg:C91A 00 00                                           SBK_Group_4_Count dw 0                  ; DATA XREF: SBK_CountSpellGroups:loc_9A689w ...
// WZD dseg:C91A                                                                                         ; Unit Spells or Nature
// WZD dseg:C91C 00 00                                           SBK_Group_3_Count dw 0                  ; DATA XREF: SBK_CountSpellGroups:loc_9A67Dw ...
// WZD dseg:C91C                                                                                         ; City Spells or Death
// WZD dseg:C91E 00 00                                           SBK_Group_2_Count dw 0                  ; DATA XREF: SBK_CountSpellGroups:loc_9A677w ...
// WZD dseg:C91E                                                                                         ; Special Spells or Chaos
// WZD dseg:C920 00 00                                           SBK_Group_1_Count dw 0                  ; DATA XREF: SBK_CountSpellGroups:loc_9A671w ...
// WZD dseg:C920                                                                                         ; Summoning Spells or Arcane

// WZD dseg:C922 00 00                                           
int16_t SBK_Dogears;
// WZD dseg:C922                                                                                         ; apprentice: 0 - choose research; 2 - apprentice book
// WZD dseg:C922                                                                                         ; new spell anim: 0 - spell on left; 1 - spell on right

// WZD dseg:C924
int16_t SBK_OpenPage;
// drake178: ; set to the index + 1 (into the page) of the spell
// drake178: ; being cast while setting up the spellbook display
// drake178: ; in the apprentice, it is then reset to the page count
// drake178: ; without the research candidates

// WZD dseg:C926 00 00                                           GUI_Multipurpose_Int dw 0               ; DATA XREF: sub_95B62+6w ...
// WZD dseg:C926                                                                                         ; Spell List Count, New Spell Index, ...
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+SBK_Descriptions@ dd 0                                    ; 0
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: GAME_LearnSpellAnim+31Ew ...
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+dd 0                                    ; 1 ; 9 or 12 LBX_Alloc_Next paragraphs in the
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 dd 0                                    ; 2 ; LBX_Sandbox_Segment (after a flush)
// WZD dseg:C928                                                 dd 0                                    ; 3
// WZD dseg:C928                                                 dd 0                                    ; 4
// WZD dseg:C928                                                 dd 0                                    ; 5
// WZD dseg:C928                                                 dd 0                                    ; 6
// WZD dseg:C928                                                 dd 0                                    ; 7
// WZD dseg:C928                                                 dd 0                                    ; 8
// WZD dseg:C928                                                 dd 0                                    ; 9
// WZD dseg:C928                                                 dd 0                                    ; 10
// WZD dseg:C928                                                 dd 0                                    ; 11
// WZD dseg:C928                                                 dd 0                                    ; 12
// WZD dseg:C928                                                 dd 0                                    ; 13
// WZD dseg:C928                                                 dd 0                                    ; 14
// WZD dseg:C928                                                 dd 0                                    ; 15
// WZD dseg:C968 00 00                                           IMG_GAME_Lab_Wizard@ dw 0               ; DATA XREF: GAME_NewSpellAnim+1Cr ...
// WZD dseg:C96A 00 00                                           IMG_GAME_LabFmiliar@ dw 0               ; DATA XREF: GAME_NewSpellAnim+3Ar ...
// WZD dseg:C96C 00 00                                           IMG_GAME_MagicColmn@ dw 0               ; DATA XREF: GAME_NewSpellAnim+4Er ...
// WZD dseg:C96E 00 00                                           IMG_GAME_Lab_Podium@ dw 0               ; DATA XREF: GAME_NewSpellAnim+8r ...
// WZD dseg:C970 00                                              db    0
// WZD dseg:C971 00                                              db    0



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
// WZD o117p02
// WZD o117p03


// WZD o117p04
// drake178: SBK_BuildSpellbook()
void SBK_BuildSpellbook(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: SBK_BuildSpellbook()\n", __FILE__, __LINE__);
#endif





#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: SBK_BuildSpellbook()\n", __FILE__, __LINE__);
#endif
}


// WZD o117p05
// WZD o117p06
// WZD o117p07
// WZD o117p08

// WZD o117p09
// drake178: OVL_DrawSpellbook()
void OVL_DrawSpellbook(int16_t x, int16_t y)
{
    SAMB_ptr Work_IMG;
    uint8_t colors[6];

    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_DrawSpellbook()\n", __FILE__, __LINE__);
#endif

    Mark_Block(_screen_seg);
    Work_IMG = Allocate_Next_Block(_screen_seg, 1050);

    for(itr = 0; itr < 6; itr++)
    {
        IMG_SBK_BookRealms[itr] = Allocate_Next_Block(_screen_seg, 4);
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], IMG_SBK_BookRealms[itr]);
    }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: OVL_DrawSpellbook()\n", __FILE__, __LINE__);
#endif
}

// WZD o117p10
// WZD o117p11
// WZD o117p12
// WZD o117p13
