__TODO_Spellbooks.md


'Main Screen'
    'Spells Button'                                                 Line 1244
            'Small Spellbook'

'Main Screen'
    'Info Button'
        'Apprentice Screen'
            'Big Spellbook'

'Combat Screen'
    'Spell Button'
        'Combat Spellbook'



'Main Screen'
    'Spells Button'                                                 Line 1244
        current_screen = scr_Spellbook_Screen;
            'Small Spellbook'
            Spellbook_Screen()



## VFX - Spellbook Page-Turn
    // BOOK.LBX  "PTURNSML"





[ ] WIZARDS.EXE  ovr117
[ ] Build_Spell_List()
[x] Combat_Spellbook_Add_Spells_From_Magic_Realm()
[ ] Spellbook_Group_Counts()
[ ] Build_Spellbook()
[ ] UU_IDK_turn_to_cast__STUB()
[x] Combat_Spellbook_Build__WIP()
[x] SmlBook_Compose()
[x] CmbBook_Compose__WIP()
[ ] SmlBook_Draw()
[x] CmbBook_Draw__WIP()
[x] SmlBook_PageTurn()
[ ] Spellbook_Add_Page()
[ ] Spellbook_Add_Group_Pages()
[ ] WIZARDS.EXE  ovr118
[ ] BigBook_PageTurn()
[ ] BigBook_Draw()
[ ] Learn_Spell_Animation_Draw()
[ ] Fill_Black_Draw()
[ ] Learn_Spell_Animation_Load()
[ ] Learn_Spell_Animation()
[ ] SBK_DrawSpellDecode__STUB()
[ ] SBK_LoadSpellDescs__WIP()
[ ] Apprentice_Screen__WIP()
[ ] BigBook_Compose__WIP()
[ ] Spell_Research_Select()
[ ] Check_Research_Spell_Is_Known()
[ ] WIZARDS.EXE  ovr134
[ ] Spell_Screen_Load()
[ ] Spellbook_Screen_Draw()
[ ] Spellbook_Screen()
[ ] Cast_Spell_Overland_Do()
[ ] sub_B9837()
[ ] Full_Draw_Main_Screen()
[ ] Dissolve_Main_Screen()
[ ] Fizzle_Notification()
[ ] Cast_Spell_City_Enchantment_Animation_Load_Music()
[ ] Spell_Animation_Load_Graphics()
[ ] Spell_Animation_Load_Sound_Effect__WIP()
[ ] Spell_Animation_Screen__WIP()



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
void Build_Spell_List(int16_t type, int16_t spell_list[]);

// WZD o117p02
void Combat_Spellbook_Add_Spells_From_Magic_Realm(int16_t * spell_list, int16_t magic_realm);

// WZD o117p03
void Spellbook_Group_Counts(void);

// WZD o117p04
void Build_Spellbook(int16_t spell_list_type, int16_t page_spell_count);

// WZD o117p05
int16_t UU_IDK_turn_to_cast__STUB(int16_t player_idx);

// WZD o117p06
int16_t Combat_Spellbook_Build__WIP(int16_t caster_idx);

// WZD o117p07
void SmlBook_Compose__WIP(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap);

// WZD o117p08
void CmbBook_Compose__WIP(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap, int16_t caster_idx);

// WZD o117p09
void SmlBook_Draw(int16_t x, int16_t y);

// WZD o117p10
void CmbBook_Draw__WIP(int16_t x, int16_t y, int16_t caster_idx);

// WZD o117p11
void SmlBook_PageTurn__WIP(int turn_type, int combat_flag, int player_idx);

// WZD o117p12
void Spellbook_Add_Page(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size);

// WZD o117p13
void Spellbook_Add_Group_Pages(int16_t Page_Size);

/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
void BigBook_PageTurn(short int direction);

// WZD o118p02
void BigBook_Draw(void);

// WZD o118p03
void Learn_Spell_Animation_Draw(void);

// WZD o118p04
void Fill_Black_Draw(void);

// WZD o118p05
void Learn_Spell_Animation_Load(void);

// WZD o118p06
void Learn_Spell_Animation(int16_t spell_idx, int16_t research_flag);

// WZD o118p07
void SBK_DrawSpellDecode__STUB(void);

// WZD o118p08
void SBK_LoadSpellDescs__WIP(int16_t page);

// WZD o118p09
void Apprentice_Screen__WIP(void);

// WZD o118p10
void BigBook_Compose__WIP(int16_t page, SAMB_ptr pict_seg, int16_t flag);

// WZD o118p11
void Spell_Research_Select(void);

// WZD o118p12
void Check_Research_Spell_Is_Known(int16_t fade);


/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
// PRIVATE  void Spell_Screen_Load(void);

// WZD o134p02
void Spellbook_Screen_Draw(void);

// WZD o134p03
void Spellbook_Screen(void);

// WZD o134p04
int16_t Cast_Spell_Overland_Do(int16_t player_idx, int16_t spell_idx, int16_t spellbook_page_spell_index);

// WZD o134p05
// sub_B9837()

// WZD o134p06
void Full_Draw_Main_Screen(void);

// WZD o134p07
void Dissolve_Main_Screen(void);

// WZD o134p08
void Fizzle_Notification(int16_t player_idx, int16_t counter_player_idx, int16_t spell_idx, char * counter_spell_name);

// WZD o134p09
// sub_B9D14()
void Cast_Spell_City_Enchantment_Animation_Load_Music(int16_t spell_idx, int16_t good_bad);

// WZD o134p10
int16_t Spell_Animation_Load_Graphics(int16_t spell_idx);

// WZD o134p11
void Spell_Animation_Load_Sound_Effect__WIP(int16_t spell_idx);

// WZD o134p12
void Spell_Animation_Screen__WIP(int16_t wx, int16_t wy, int16_t wp);
