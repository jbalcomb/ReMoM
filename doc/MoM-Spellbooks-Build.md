

// WZD o134p03
void Spellbook_Screen(void)
SBK_BuildSpellbook__WIP(slt_Overland, 6);

// WZD o117p04
void SBK_BuildSpellbook__WIP(int16_t spell_list_type, int16_t page_spell_count)

Allocate_First_Block()
Near_Allocate_First()


## Variables

m_spellbook_spell_list

SBK_Research_Incomes

SBK_Group_3_Count
SBK_Group_2_Count
SBK_Group_1_Count
SBK_Group_5_Count
SBK_Group_4_Count
SBK_Group_6_Count

GUI_Multipurpose_Int__m_spell_list_count

m_spellbook_page_count

WIZ_ManaPerTurn

SBK_BookManaLimit



## WIZ_ManaPerTurn
just used for the calculation for SBK_BookManaLimit
XREF:
    Magic_Screen+471                  mov     ax, [WIZ_ManaPerTurn]
    Magic_Screen:loc_60C86            mov     ax, [WIZ_ManaPerTurn]
    Magic_Screen+48F                  mov     ax, [WIZ_ManaPerTurn]
    SBK_BuildSpellbook__WIP+F8        mov     [WIZ_ManaPerTurn], dx
    SBK_BuildSpellbook__WIP+108       mov     ax, [WIZ_ManaPerTurn]
    SBK_BuildSpellbook__WIP:loc_9A840 mov     ax, [WIZ_ManaPerTurn]
    SBK_BuildSpellbook__WIP+12E       mov     ax, [WIZ_ManaPerTurn]

## SBK_BookManaLimit

used in SmlBook_Compose__WIP() to calc turns to cast, which is used to draw the symbols, and nothing else
...same in CmbBook_Compose__WIP()


Combat_Spellbook_Screen() uses it, doesn't calculate it like Spellbook_Screen() does
...because, it's calc'd in Combat_Spellbook_Build__WIP(), which is called from Combat_Cast_Spell__WIP(), right before it calls Combat_Spellbook_Screen()
...xref/usage in Magic_Screen() is meaningless...

XREF:
    Magic_Screen+481                  mov     [SBK_BookManaLimit], ax
    Magic_Screen+496                  mov     [SBK_BookManaLimit], ax
    Magic_Screen:loc_60C9B            mov     [SBK_BookManaLimit], 0 
    Combat_Spellbook_Screen+5A3       cmp     ax, [SBK_BookManaLimit]
    SBK_BuildSpellbook__WIP+120       mov     [SBK_BookManaLimit], ax
    SBK_BuildSpellbook__WIP+135       mov     [SBK_BookManaLimit], ax
    SBK_BuildSpellbook__WIP:loc_9A855 mov     [SBK_BookManaLimit], 0 
    Combat_Spellbook_Build__WIP+78    mov     [SBK_BookManaLimit], ax
    Combat_Spellbook_Build__WIP+99    cmp     ax, [SBK_BookManaLimit]
    Combat_Spellbook_Build__WIP+BD    mov     [SBK_BookManaLimit], ax
    Combat_Spellbook_Build__WIP+D6    mov     [SBK_BookManaLimit], ax
    Combat_Spellbook_Build__WIP+304   cmp     ax, [SBK_BookManaLimit]
    Combat_Spellbook_Build__WIP+407   cmp     ax, [SBK_BookManaLimit]
    SmlBook_Compose__WIP+85           cmp     [SBK_BookManaLimit], 0 
    SmlBook_Compose__WIP+97           idiv    [SBK_BookManaLimit]    
    CmbBook_Compose__WIP+FB           cmp     ax, [SBK_BookManaLimit]
    CmbBook_Compose__WIP:loc_9B64E    mov     ax, [SBK_BookManaLimit]
    BigBook_Compose__WIP+215          cmp     [SBK_BookManaLimit], 0 
    BigBook_Compose__WIP+226          add     ax, [SBK_BookManaLimit]
    BigBook_Compose__WIP+22C          idiv    [SBK_BookManaLimit]    
