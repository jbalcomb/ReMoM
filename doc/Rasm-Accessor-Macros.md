


¿ Macros Per Structure ?
s_WIZARD        _players[]
s_UNIT          _UNITS[]
s_BATTLE_UNIT   battle_units[]
s_OWNED_HERO
s_ITEM          _ITEMS[]



## _UNITS[]

$ grep "Status == us_Wait" *
MainScr.C:            if(_UNITS[itr_units].Status == us_Wait)
MainScr.C:                        if(_UNITS[itr_wait_units].Status == us_Wait)
MainScr.C:                if(_UNITS[unit_idx].Status == us_Wait)
NEXTTURN.C:        if((_UNITS[itr_units].Status == us_Wait) || (_UNITS[itr_units].Status == us_ReachedDest))

UNIT_MarkRemoved()
.owner_idx = ST_UNDEFINED

    _UNITS[unit_idx].owner_idx = ST_UNDEFINED;

    _UNITS[unit_idx].wp = ST_UNDEFINED;  // ¿ here because `wp = 9` is used for dead combat summon units ?




Build_Unit_Stack()
    if((_UNITS[unit_idx].owner_idx == player_idx) && 
        (_UNITS[unit_idx].wx == wx) &&
        (_UNITS[unit_idx].wy == wy) &&
        (_UNITS[unit_idx].owner_idx != ST_UNDEFINED) && 
        ((_UNITS[unit_idx].wp == wp) || (_UNITS[unit_idx].in_tower == ST_TRUE))
    )

owner/player
wx,wy,wp
wx && wy
wp || in_tower

¿ Not Dead ? (_UNITS[unit_idx].owner_idx != ST_UNDEFINED)

    bus_Gone        = 6   / * ¿ ? ¿ ~== _UNITS[].wp == 9 ? * /

¿ activateable ?
if((_UNITS[unit_idx].moves2 > 0) && (_UNITS[unit_idx].Finished == ST_FALSE))


WIZ_NextUnit()
        if(
            (_UNITS[itr_units].owner_idx == player_idx) &&
            ( (_UNITS[itr_units].wp == current_world_plane) || (_UNITS[itr_units].in_tower == ST_TRUE) ) &&
            (_UNITS[itr_units].owner_idx != ST_UNDEFINED) &&  /* ¿ not dead / disbanded / banished / unsummoned ? */
            (_UNITS[itr_units].Finished == ST_FALSE)
        )


RdBd_UNIT_SelectStack_STUB()
                    if(
                        (Unit_Has_AirTravel_Item(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_WaterTravel_Item(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_Swimming(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_WindWalking(entire_stack[itr]) == ST_TRUE)
                        ||
                        (Unit_Has_AirTravel(entire_stack[itr]) == ST_TRUE)
                    )
legal travel methods?
usage/reason/outcome?
e.g.,  does not require a boat
per terrain type/group?
water vs. ocean or river?


is a boat
(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)
is not a boat
(_unit_type_table[_UNITS[entire_stack[itr]].type].Transport < 1)








### HELP

// _help_entries[(11 + (display_index + itr))]
// &_help_entries.help_11 + ((display_index + itr) * sizeof(struct s_HELP_FIELD)))
*((int16_t *)&_help_entries.help_11 + ((display_index + itr) * sizeof(struct s_HELP_FIELD))) = ST_UNDEFINED;

mov     [bx-4418h], ax
mov     _help_entries.entry_idx+6Eh)+bx], ax
dseg:BB7A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_help_entries s_HELP_ENTRY 32h dup(<0>)
(_help_entries[0x6E] + ( (uv_specials_list_index + itr) * (sizeof(struct s_HELP_FIELD)) )) = ...
*((int16_t *)&_help_entries.help_11 + ((display_index + itr) * sizeof(struct s_HELP_FIELD))) = ...





# HOWTO::Macros

w/w/o parenthesis?  
w/w/o braces?  
w/w/o do{}while()?  
w/w/o required parenthesis?  
w/w/o required semi-colon?  

ALWAYS enclose macro variables in parenthesis  

"macro body" ... "statement"  



### w/w/o do{}while()
    a single statement or a statement block
¿ definitely only needed for multiple statement macro ?
¿ IIF multiple statement macro ?


### w/w/o required parenthesis


### w/w/o required semi-colon


placing a semicolon after the macro leads to a compile error.
Semicolons are not valid after arbitrary blocks, but it is acceptable to place them after do-while loops.
It's much nicer to be able to call "MYMACRO(1, 2, 3);" than it is to have to remember to leave the semi-colon off.


If you forget to use curly braces,
  the compiler will only expand the macro to the first statement in the macro body.  

...execute a sequence of multiple statements as a group...  

Inline functions are, in general, more suitable for this task  
  (see PRE00-C. Prefer inline or static functions to function-like macros).  
Occasionally, however, they are not feasible  
  (when macros are expected to operate on variables of different types, for example).  

When multiple statements are used in a macro,  
  they should be bound together in a do-while loop syntactically,  
  so the macro can appear safely inside if clauses or other places that expect a single statement or a statement block.  

Macro replacement lists should be parenthesized
  to protect any lower-precedence operators from the surrounding expression.  


Macro definitions, regardless of whether they expand to a single or multiple statements, should not conclude with a semicolon.  
(See PRE10-C. Wrap multistatement macros in a do-while loop.)  
If required, the semicolon should be included following the macro expansion.  
Inadvertently inserting a semicolon at the end of the macro definition can unexpectedly change the control flow of the program.  



"PRE" ~== "Preprocessor"  

Exceptions
PRE02-C-EX1:  
  A macro that expands to a single identifier or function call is not affected by the precedence of any operators in the surrounding expression, so its replacement list need not be parenthesized.  
```#define MY_PID getpid()```
PRE02-C-EX2:  
  A macro that expands to an array reference using the array-subscript operator [], or an expression designating a member of a structure or union object using either the member-access . or -> operators is not affected by the precedence of any operators in the surrounding expression, so its replacement list need not be parenthesized.  
```#define NEXT_FREE block->next_free```
```#define CID customer_record.account.cid```
```#define TOOFAR array[MAX_ARRAY_SIZE]```


PRE11-C. Do not conclude macro definitions with a semicolon

Noncompliant Code Example
In this noncompliant code example, the programmer defines a macro that increments the value of the first argument, x, by 1 and modulates it with the value of the second argument, max:

#define INCREMOD(x, max) ((x) = ((x) + 1) % (max));
 
int index = 0;
int value;
value = INCREMOD(index, 10) + 2;
/* ... */
In this case, the programmer intends to increment index and then use that as a value by adding 2 to it. Unfortunately, the value is equal to the incremented value of index because of the semicolon present at the end of the macro. The + 2; is treated as a separate statement by the compiler. The user will not get any compilation errors. If the user has not enabled warnings while compiling, the effect of the semicolon in the macro cannot be detected at an early stage.

Compliant Solution
The compliant solution is to write the macro definition without the semicolon at the end, leaving the decision whether or not to have a semicolon up to the person who is using the macro:

#define INCREMOD(x, max) ((x) = ((x) + 1) % (max))
Compliant Solution
This compliant solution uses an inline function as recommended by PRE00-C. Prefer inline or static functions to function-like macros.

inline int incremod(int *x, int max) {*x = (*x + 1) % max;}



https://wiki.sei.cmu.edu/confluence/display/c/PRE00-C.+Prefer+inline+or+static+functions+to+function-like+macros
https://wiki.sei.cmu.edu/confluence/display/c/PRE02-C.+Macro+replacement+lists+should+be+parenthesized
https://wiki.sei.cmu.edu/confluence/display/c/PRE10-C.+Wrap+multistatement+macros+in+a+do-while+loop
https://wiki.sei.cmu.edu/confluence/display/c/PRE11-C.+Do+not+conclude+macro+definitions+with+a+semicolon
https://wiki.sei.cmu.edu/confluence/display/c/PRE31-C.+Avoid+side+effects+in+arguments+to+unsafe+macros





The Dasm shows various approaches to getting words or bytes from the near and far pointers.

LEA and then word/byte ptr [ES:BX]

wp * WORLD_SIZE
wp * WORLD_SIZE + wy * WORLD_WIDTH
wp * WORLD_SIZE + wy * WORLD_WIDTH + wx


wp * WORLD_SIZE_DB
wp * WORLD_SIZE_DB + wy * WORLD_WIDTH_DB
wp * WORLD_SIZE_DB + wy * WORLD_WIDTH_DB + wx * 2


...like I did with CITYX(), CITY(), CITYP()
```c
#define CITYX() ( _CITIES[city_idx].wx )
#define CITYY() ( _CITIES[city_idx].wy )
#define CITYP() ( _CITIES[city_idx].wp )
```
requires having/naming `city_idx`
assumes `_CITIES` and `s_CITY`
Then, ...
    same for city_wx, city_wy, city_wp?
    option for passing some/all parameters?

Kinda wanna just use wx,wy,wp...
But, ...
    some functions have
        x,y,p for units and cities
        iters for them
        intermediate/current values




So, ...
    ~ 'Get Word Of World Map' (wx, wy, wp)
    ~ 'Get Byte Of World Map' (wx, wy, wp)
    ¿ the DB or DW access seen in the Dasm is due to the width of the data-type of the pointer ?
        (uint8_t  *)(_world_maps + ( (wp * 4800) + (wy * 120) + (wx * 2) ) );
        (uint16_t *)(_world_maps + ( (wp * 2400) + (wy *  60) + (wx    ) ) );


### _world_maps

if the value matches what is in the Terrain Type enum, then the value is a Terrain Type
¿ that just happens to also be the index into TERRAIN.LBX,0 and TERRAIN.LBX,1 ?

    uint8_t * world_map_ptr;
    int16_t terrain_type_idx;
    int16_t terrain_type;
    world_map_ptr = (_world_maps + (wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2));
    terrain_type_idx = GET_2B_OFS(world_map_ptr, 0);
    terrain_type = terrain_type_idx % TERRAIN_COUNT;

Draw_Map_Unexplored_Area()
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));
    terrain_001_0 = GET_1B_OFS(terrain_lbx_001, (terrain_001_index * 2) + 0);

Redraw_Map_Unexplored_Area()
    uint16_t terrain_001_index;
    terrain_001_index = GET_2B_OFS(_world_maps, ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2)));

Draw_Map_Terrain()
    int16_t world_maps_offset;
    uint16_t terrain_001_index;
    uint8_t terrain_001_0;
    uint8_t terrain_001_1;
    world_maps_offset = ((world_plane * WORLD_SIZE * 2) + (itr_world_y * WORLD_WIDTH * 2) + (curr_world_x * 2));
    terrain_001_index = GET_2B_OFS(_world_maps,world_maps_offset);
    terrain_001_index += terrain_tile_base;
    terrain_001_index *= 2;  // because, sizeof(int16_t)
    terrain_001_0 = 0;
    terrain_001_0 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 0);
    terrain_001_1 = GET_1B_OFS(terrain_lbx_001, terrain_001_index + 1);

Draw_Reduced_Map()
    int16_t terrain_type_idx_base;
    int16_t terrain_type_idx;
    uint8_t * explore_data_ptr;
    uint8_t * world_data_ptr;
    uint8_t * minimap_pict_data_ptr;
    terrain_type_idx_base = world_plane * TERRAIN_COUNT;
    explore_data_ptr = (TBL_Scouting + (world_plane * WORLD_SIZE));
    world_data_ptr = (_world_maps + (world_plane * 4800));
    minimap_pict_data_ptr = minimap_pict_seg + 16;  // +1 segment (paragraph), for the animation/picture/bitmap header
    terrain_type_idx = GET_2B_OFS(world_data_ptr, ((minimap_square_y * 120) + (minimap_square_x * 2)));
    terrain_type_idx += terrain_type_idx_base;
    minimap_terrain_color = terrain_lbx_002[terrain_type_idx];

Terrain_Is_Sailable()
    uint8_t * src_sgmt;
    uint16_t src_ofst;
    uint16_t world_map_value;
    uint16_t terrain_type;
    // les  bx, [_world_maps]
    // ~== ES = (&_world_maps[0] / 16); BX = (&_world_maps[0] % 16);
    src_sgmt = _world_maps;
    src_ofst = 0;
    src_ofst = (map_plane * 4800) + (world_y * 120) + (world_x * 2);
    world_map_value = GET_2B_OFS(src_sgmt, src_ofst);
    terrain_type = world_map_value % TERRAIN_TYPE_COUNT;

Terrain_Is_River()
    terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % TERRAIN_COUNT);


Map_Square_Production_Bonus()
    ¿ no word ptr or byte ptr ?
    ¿ all 16-bit registers ?

```x86asm
    mov     ax, [bp+wp]
    mov     dx, e_WORLD_SIZE_DB
    imul    dx
    les     bx, [_world_maps]               ; 25Ah paragraphs in World_Data@
    add     bx, ax
    mov     ax, [bp+wy]
    mov     dx, e_WORLD_WIDTH_DB
    imul    dx
    add     bx, ax
    mov     ax, [bp+wx]
    shl     ax, 1
    add     bx, ax
    mov     ax, [es:bx]

    mov     bx, TerType_Count
    xor     dx, dx
    div     bx
    mov     _SI_terrain_type, dx

    cmp     _SI_terrain_type, TT_Desert_end
```
