
¿ ?
What it is?

not sure what all this is or what you might call it
but, there's a whole bunch of it blocked up in same location
~ UV/USW
~ Unit Data Tables
¿ MoO2 ?
¿ 1oom ?
    C:\STU\developp\1oom\src\game\game_shiptech.c
        struct shiptech_weap_s tbl_shiptech_weap[WEAPON_NUM]
            &game_str_tbl_st_weap[0], &game_str_tbl_st_weapx[0],
    C:\STU\developp\1oom\src\game\game_str.c
        const char *game_str_tbl_st_weap[WEAPON_NUM - 1] = { "NUCLEAR BOMB", ... };
        const char *game_str_tbl_st_weapx[WEAPON_NUM - 1] = { "GROUND ATTACKS ONLY", ... };


e.g.,
Get_Unit_Enchantment_Names()
itr 32 
itr 3





Unit Enchantment
Unit Mutation
Unit Enchantments
Unit Mutations
UnitEnchantment
UnitMutation
UnitEnchantments
UnitMutations



dseg:3A50                                                 ¿ BEGIN:  ovr078  UV / USW ?
dseg:3A50 01 00 00 00 2B 00 03 02 02 00 00 00 2B 00 03 02+UV_Hero_Abilities USW_HeroAbl <1, 2Bh, HLP_HSA_LEADERSHIP>





### _unit_enchantment_data
XREF:
    Get_Unit_Enchantment_Names+37 and     ax, [word ptr _unit_enchantment_data.flag+bx];
    Get_Unit_Enchantment_Names+3B and     dx, [word ptr (_unit_enchantment_data.flag+2)+bx];
    Get_Unit_Enchantment_Names+4C push    [word ptr _unit_enchantment_data.name+bx]; src                               
    USW_Build_Effect_List+1AB8    and     ax, [word ptr _unit_enchantment_data.flag+bx];
    USW_Build_Effect_List+1ABC    and     dx, [word ptr (_unit_enchantment_data.flag+2)+bx];
    USW_Build_Effect_List+1AD1    push    [word ptr _unit_enchantment_data.name+bx]; src                               
    USW_Build_Effect_List+1AF3    mov     ax, [word ptr (_unit_enchantment_data.flag+2)+bx];
    USW_Build_Effect_List+1AF7    mov     dx, [word ptr _unit_enchantment_data.flag+bx];
    USW_Build_Effect_List+1B18    mov     al, [_unit_enchantment_data.icon_idx+bx];     
    USW_Build_Effect_List+1B3D    mov     ax, [_unit_enchantment_data.help_idx+bx];     
    UV_Remove_Unit_Enchantment+1D mov     ax, [word ptr (_unit_enchantment_data.flag+2)+bx];
    UV_Remove_Unit_Enchantment+21 mov     dx, [word ptr _unit_enchantment_data.flag+bx];
    UV_Remove_Unit_Enchantment+5D push    [word ptr _unit_enchantment_data.name+bx]; src                               

### _unit_mutation_data
XREF:
    Get_Unit_Enchantment_Names+8D test    [_unit_mutation_data.flag+bx], dl             
    Get_Unit_Enchantment_Names+9C push    [word ptr _unit_mutation_data.name+bx]; src   
    USW_Build_Effect_List+1819    mov     dx, (offset _unit_mutation_data.name+0Ch); WTF
    USW_Build_Effect_List+1923    test    [_unit_mutation_data.flag+bx], dl             
    USW_Build_Effect_List+1933    push    [word ptr _unit_mutation_data.name+bx]; src   
    USW_Build_Effect_List+196C    mov     al, [_unit_mutation_data.icon_idx+bx]         
    USW_Build_Effect_List+1991    mov     ax, [_unit_mutation_data.help_idx+bx]         






struc s_UNIT_ENCHANTMENT ; (sizeof=0xA)
00000000 name dw ?                               ; offset (00036AA0)
00000002 flag dd ?
00000006 icon_idx db ?
00000007 align2_07h db ?
00000008 help_idx dw ?                           ; enum HELP_TXT
0000000A ends s_UNIT_ENCHANTMENT

struc s_UNIT_MUTATION ; (sizeof=0x6)
00000000 name dw ?                               ; offset (00036AA0)
00000002 flag db ?                               ; enum enum_UNIT_MUTATION  // enum MUT_FLAGS
00000003 icon_idx db ?
00000004 help_idx dw ?                           ; enum HELP_TXT
00000006 ends s_UNIT_MUTATION


WZD dseg:3C9E
_unit_enchantment_data s_UNIT_ENCHANTMENT

WZD dseg:3C68
_unit_mutation_data s_UNIT_MUTATION

UnitView.C
struct s_UNIT_ENCHANTMENT _unit_enchantment_data[32] =

struct s_UNIT_MUTATION USW_Chao_unit_mutation_datasChannels[] =



// WZD dseg:42A7
char cnst_ChaosChannel[] = "Chaos Channel";

char * unit_mutation_names[] =
{
    cnst_ChaosChannel
};

// WZD dseg:3C68
// { name, flag, icon_idx, help_idx }
struct s_UNIT_MUTATION _unit_mutation_data[3] =
{
    { &unit_mutation_names[0],  0x04,  SPECIAL_ICON_WARPED_DEFENSE,  HLP_WARPED_DEFENSE },
    { &unit_mutation_names[0],  0x08,  SPECIAL_ICON_WARPED_FLIGHT,   HLP_WARPED_FLIGHT  },
    { &unit_mutation_names[0],  0x10,  SPECIAL_ICON_WARPED_BREATH,   HLP_WARPED_BREATH  }
};
