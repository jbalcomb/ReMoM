

Disjunct
Disjunct True
Spell Bind


Spell_Target_Global_Enchantment_Screen_Load()
Spell_Target_Global_Enchantment_Screen_Draw()
Spell_Target_Global_Enchantment_Screen()
Spell_Target_Global_Enchantment_Bind__WIP()
Spell_Target_Global_Enchantment_Disjunct__WIP()



What's the formula for this 'y'?
    FLIC_Draw((x_start + 73), (y_start + 16 + ((_osc_scanned_field - 1) * 13) + (((_osc_scanned_field - 1) / 3) * 7)), spellscr_oversbut_seg);

...'ystart + 16' is moving from the top of the popup window to the top of the spell list boxes area
...the '- 1' is because field indices start at 1
...the '* 13' is for the height of the spell name boxes

same y1 for the spell name fields?
    y1 = (16 + ((y_start + ((itr % 3) * 13)) + (46 + (itr / 3))));


if(((_osc_scanned_field - 1) % 3) >= ovl_ench_list_cnt[(1 + ((_osc_scanned_field - 1) / 3))])
...
ovl_ench_list_cnt[(1 + ((_osc_scanned_field - 1) / 3))]
...
'((_osc_scanned_field - 1) / 3)'
gets you player_idx from the field_num





Cast_Spell_Overland__WIP()
    // OVERLAND:  Disjunction, Disjunction True
    case scc_Disjunctions:  // 20
        if(player_idx == HUMAN_PLAYER_IDX)
            IDK_Spell_DisjunctOrBind(spell_idx, player_idx);



for(itr2 = 1; itr2 < _num_players; itr2++)
    for(itr1 = 0; itr1 < 3; itr1++)
        spell_idx = ovl_ench_list_ptr[(itr2 - 1)][(ovl_ench_list_fst[itr2] + itr1)]
        ...
        spell_data_table[ovl_ench_list_ptr[(itr2 - 1)][(ovl_ench_list_fst[itr2] + itr1)]].name




sets ovl_ench_list_spells[144] = -1
uses Build_Overland_Enchantment_List()
ovl_ench_cnt
    count of global enchantments
ovl_ench_list_spells[]
    array of spell_idx
ovl_ench_list_players[]
    array of player_idx

Cast_Spell_Disjuncts_Load__WIP()
builds an array of global enchantment count per player, using ovl_ench_list_players[]
builds an array of the first index per player
builds an array of the first list index per player

IDK_list_first[]
    the list index of the first spell to draw/match, per player

IDK_list_count_ptr[] is a pointer to the array element in ovl_ench_list_spells[] 
e.g.,
                     ~== ovl_ench_list_spells[0]
ovl_ench_list_ptr[0] ~== ovl_ench_list_spells[1]
ovl_ench_list_ptr[1] ~== ovl_ench_list_spells[3]
ovl_ench_list_ptr[2] ~== ovl_ench_list_spells[10]
ovl_ench_list_ptr[3] ~== ovl_ench_list_spells[11]

...don't know why they hard-coded it for players 2, 3, 4, 5
...as coded, it can't get there, but those can/will be invalid

So, ...
    all the spell_idx's are in ovl_ench_list_spells[], in order by player
    ovl_ench_list_ptr[] provides a pointer to the subset
    IDK_list_counts[] provides the count for the subset

ovl_ench_list_ptr
ovl_ench_list_cnt
ovl_ench_list_fst

plyr_ench_list_ptr
plyr_ench_list_cnt
plyr_ench_list_fst

...first + iter...







## Popup Window

4 gems
3 spells per gem
up/dn arrows per gem



x_start
y_start
...relative to the popup window...

top arrow
    if(IDK_list_first[itr] > 0)
    ...current index is greater than 0, so we can move up one
bot arrow
    if((IDK_list_first[itr] + 3) < IDK_list_count[itr])
    ...means the the current + 3 is less than the (max/total) count, so we can move down one
positions
    starting screen x,y for the popup window
    x_start, y_start
    screen x,y offset relative to the popup window
        same x 61
        top y 19
        bot y 43

                top_arrow_fields[itr] = Add_Button_Field(
                    (x_start + 61),
                    (y_start + 19 + (itr * 46)),
                bot_arrow_fields[itr] = Add_Button_Field(
                    ( x_start + 61),
                    (y_start + 43 + (itr * 46)),
    y += (itr * 46)
        means each block of spells per wizard is 46 pixels high
-2 two for the lines in between
each spell line is 14 pixels





(num_player % 3) = {0,1,2,3}

+

(num_player / 3) = offset



1st gem at 23,50?
24,234
24,418
25,602


fields:
    gems
    arrows
    list items
4 * 3 = 12
 0, 1, 2,
 3, 4, 5,
 6, 7, 8,
 9,10,11

mod to get list
div to get offset

top_arrow_fields[4]
bot_arrow_fields[4]
indexed by iter (_num_players - 1)

top arrow:
    move list up one
    must have list
    must have current list item?
    must check if there are more than 3 or 3 more than the current?

array of counts
array of currents




## spell_data_table[].name

Build_Overland_Enchantment_List() sets spell_idx's into ovl_ench_list_spells[]
ovl_ench_cnt is the count
ovl_ench_list_players[] is the player_idx per spell in the list

...their first plus their current plus iter {0,1,2}
IDK_list_count_end[(itr2 -1)] + IDK_list_first[((itr2 - 1) + itr1)]

ovl_ench_list_spells[(IDK_list_count_end[(itr2 -1)] + IDK_list_first[((itr2 - 1) + itr1)])]


spell_idx = ...

mov     bx, _DI_itr2
shl     bx, 1
mov     bx, [(IDK_list_count+0Ah)+bx]   IDK_list_count_end[(itr2 -1)]

mov     si, _DI_itr2
shl     si, 1
mov     ax, [bp+itr1]
add     ax, [_xtra_mana_pos+si]         IDK_list_first[((itr2 - 1) + itr1)]
shl     ax, 1

add     bx, ax
mov     ax, [bx]


