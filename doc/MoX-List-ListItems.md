
List
List Items

ArmyList
CityList



Something...list...
    one per city
    one per army
        but, also, units per army

generic list vs. army/city list
list_item_count is the count of items/groups/rows ... army(6) / city(9)
    it's the count for the short-list of what to display

list_first_item
    first what, where?

Get_List_Armies_Index()
    list_first_item = 0
iters over armylist_item_count
    used to index list_armies
        matches unit_idx

    returns list_armies_itr
    list_armies_itr is set to itr, 

armylist_item_scanned_field = Get_List_Armies_Index(armylist_hero_portrait_unit_indices[itr]);

FUU!!!
when you mouse over a hero portrait, it jumps to that unit in the army list
so, scanned_field must match the hero portrait field
then?
    it finds the unit_idx in the army list?
    while tracking which army that would be?
    ...
    sets armylist_item_scanned_field
        which would be in a 6 x 9 array of fields?

finds the unit_idx and returns the list index?


list_armies[] is the list of unit_idx

armylist_list_item_count is the count of units in the stack

a "list item" is a row
subitems?
M*N
army/stack/group and units






"the army list"
"highlighted unit"

"Armies Screen"
ArmyList.C
ArmyList_Set_List_Item_Count()

ArmyList_Screen()
    Build_Army_List();
    list_first_item = 0;
    Build_Army_List_Counts();


Build_Army_List()
    armylist_unit_count = 0;
    armylist_army_count = 0;
    iterate over all units
        if owner is human/current player
            increment armylist unit count  (list item count)
            if not same stack
                add to x,y,p arrays  (indexed by armylist_army_count)
                increment armylist army count  (list group count)
So, ...
    total unit count
    count of groups of units
    wx,wy,wp for each group

..."armylist" as in *List*
..."army" as in *Group*  (*List Group*)

Usages for armylist_unit_count?
    ¿ ~None ?
        only cross-refernce from Build_Army_List()
        set to 0 and incremented in unit loop when owner == player
    ¿ Same for citylist ?
        Nope.
        The 'city list' does not have groups, so it is just the total count and an array of city_idx.


Build_Army_List_Counts()
    ArmyList_Set_List_Item_Count()
iterates over list_item_count

list_armies[]
    6 x 9 array of armies and units




build/steps

count of units for player

arrays of wx,wy,wp

array of unit_idx

array of army unit counts

Build_Army_List_Counts()
    list_armies[unit_count] = itr_units;
    armylist_list_item_count[itr_list_item_count]++;



CityList_Screen()
    CityList_Screen_Load()
        m_city_list_array   = Near_Allocate_First(200);  // 100 city_idx
        list_cities = Near_Allocate_Next(18);    //   9 2-byte ints, for City List of 9
    Build_City_List();
    list_first_item = m_citylist_first_item;
    Update_Cities_List();
    CityList_Set_List_Item_Count();
    citylist_item_scanned_field = 0;
    CityList_Add_List_Fields();

when the displayed city list changes, because you activated the up or down fields
    list_first_item
    Update_Cities_List();
        ...list_cities[itr_list_items] = city_list[(list_first_item + itr_list_items)];
sets list_cities[] from city_list[]
uses list_item_count and list_first_item

list_item_count is set/updated CityList_Set_List_Item_Count()

CityList_Add_List_Fields()
    m_cities_list_field_count = 0;
    CityList_Set_List_Item_Count();
    ...m_cities_list_fields[]
    sets m_cities_list_field_count  (which would equal list_item_count)
        only used to iterate over the fields, in CityList_Screen()

ArmyList_Add_List_Fields()
    m_armies_list_field_count = 0;
    ArmyList_Set_List_Item_Count();
    ...m_armies_list_fields[]
    sets m_armies_list_field_count  (which would equal list_item_count)
        used to iterate over the fields, in ArmyList_Screen()
        also, Get_List_Armies_Index()
            used to index list_armies[], to get unit_idx

Build_City_List()
    m_city_list_array[m_city_list_count] = city_idx




### Build_Army_List()
    calcs m_city_list_count
    populates army_wx,y,p
    ¿ ~== m_city_list_array ?
    ...DNE, because army list is the stacks
    ...array of unit_idx is elsewhere

### m_army_list_count
    ~== m_city_list_count
    count of armies / stacks / groups / list items
    controls up/down buttons


Build_Army_List_Counts()  &&  ArmyList_Screen_Draw()
    ...
    list_armies[list_armies_idx] = itr_units;
    list_armies_idx++;
    &&
```c
    /*
        BEGIN:  Armies, Units
    */
    list_armies_idx = 0;

    for(itr_stacks = 0; itr_stacks < list_item_count; itr_stacks++)
    {
        for(itr_units = 0; itr_units < armylist_list_item_count[itr_stacks]; itr_units++)
        {
            x1 = armylist_start_x + (18 * itr_units);
            y1 = armylist_start_y + (22 * itr_stacks);

            if(list_armies_idx == armylist_item_scanned_field)
            {
                Gradient_Fill((x1 + 1), (y1 + 2), (x1 + 19), (y1 + 16), 15, 1, ST_NULL, ST_NULL, ST_NULL);
            }

            unit_idx = list_armies[list_armies_idx];

            Draw_Unit_StatFig(x1, y1, list_armies[list_armies_idx], 1);

            list_armies_idx++;
        }
    }
    /*
        END:  Armies, Units
    */
```





### Build_City_List()
    populates m_city_list_array[], m_city_list_count

### Update_Cities_List()
    populates list_cities[]
    with list_item_count city_idx's
XREF:
    NX_j_Update_Cities_List()
    CityList_Screen+5F      call    near ptr Update_Cities_List
    CityList_Screen+29D     call    near ptr Update_Cities_List
    CityList_Screen+2DE     call    near ptr Update_Cities_List
    CityList_Screen+47C     call    near ptr Update_Cities_List

At start of CityList_Screen(), ...
    calls Build_City_List()
    sets list_first_item
    calls Update_Cities_List()  ...which calls CityList_Set_List_Item_Count(), to set list_item_count
    So, ...




### m_city_list_array
    populated in Build_City_List()


### m_city_list_count
    count of elements in m_city_list_array[]


### list_first_item
    index into m_city_list_array[]
    used to populate list_cities[]  ...(list_first_item + itr)



### list_item_count
ArmyList_Set_List_Item_Count()
CityList_Set_List_Item_Count()
EoD, sets list_item_count to the count of row for the list screen, full/partial, <= num rows city/army

So, something should iterate over a list
    from list_first_item
    for list_item_count


Update_Cities_List()
    |-> CityList_Set_List_Item_Count()



Update_Col_List_()
    updates _list_col[] from _g_colony_list_ptr, starting from _first, itr < 10
    so, colony_list[] is all the colonies for a given player
    and _list_col[] the short-list of which to display





MoO2  Module: COLXPORT  Build_Global_Colony_List_()
doesn't track the count, assumes _g_colony_list_ptr[] is 250 and checks for ST_UNDEFINED
```c
void Build_Global_Colony_List_(void)
{
    int colony_count;
    int itr;

    colony_count = 0;

    for(itr = 0; itr < _NUM_COLONIES; itr++)
    {
        if((_colony[itr].player_idx == _PLAYER_NUM) && (_colony[itr].is_outpost == ST_FALSE))
        {
            _g_colony_list_ptr[colony_count] = itr;  // ~== colony_idx
            colony_count++;
        }
    }

    for(itr = colony_count; itr < 250; itr++)
    {
        _g_colony_list_ptr[itr] = ST_UNDEFINED;
    }
}
```
```c
Colony_Summary_Screen_()
{
    int16_t l_colony_list_array[250];
    _g_colony_list_ptr = l_colony_list_array;
    Build_Global_Colony_List_();
    Sort_Col_List_();
    Update_Col_List_();  // updates _list_col[]
    if(_list_col[6] == ST_UNDEFINED)
    {
        _first = 0;
        Clear_List_Col_Array_();
        Update_Col_List_();  // updates _list_col[]
        Update_Col();
    }
}
```
```c
Clear_List_Col_Array_()
{
    int itr;
    for(itr = 0; itr < 10; itr++)
    {
        _list_col[itr] = ST_UNDEFINED;
    }
}
```





list_armies[] is Unit Indicies

list_first_item is the index of the first item, in the array of all items, to start from when building/processing the lists

list_item_count is the count of lists ... armies, cities ... always <= 6  ~== MIN(armylist_army_count,6)

armylist_list_item_count is the count of units in each army

ArmyList_Set_List_Item_Count()
    sets list_item_count
Build_Army_List_Counts()
    sets armylist_list_item_count

when you mouse-over a Hero Portrait, it sets the armylist_item_scanned_field
    iterates through the fields, to find the scanned field index
    uses the iterator to index into the array of unit indicies
    passes the unit index to Get_List_Armies_Index()

Get_List_Armies_Index()
    iterates through armylist_item_count
    matches list_armies[itr] to passed in unit index
    returns index of list_armies[]








CityList Count Maximum is 9

list of all cities, owned by player

list for CityList Screen

first list item
index of city, in all cities list, for first city to show on CityList

city list count is minimum of city list count or city count  {1, ..., 9}




if first list item is MIN, no up buttons or hotkey
if last list item is MAX, no down buttons or hotkey
MIN is 0
MAX is first list item + list item max > list count


// ~ MoO2  Update_Col_List_()



list_item_count
list_first_item

ArmyList_Set_List_Item_Count()
armylist_army_count

CityList_Set_List_Item_Count()
citylist_city_count



ArmyList_Set_List_Item_Count()
    expects list_first_item and armylist_army_count to already be set
    sets list_item_count




ArmyList_Screen()       // WZD o66p01
Cities_Screen()         // WZD o65p01
ArmyList_Screen_Draw()  // WZD o66p02
CityList_Screen_Draw()  // WZD o65p04

ArmyList_Set_List_Item_Count()  // WZD o66p09
CityList_Set_List_Item_Count()  // WZD o65p08


IDK_Armies_Screen_s597F6()
    IDK_ArmyList_Unit_Indices[] is all units that match owner, x, y, p
Build_City_List()
    m_city_list_array[] is all cities that match owner
¿ ~== MoO2  Build_Global_Colony_List_() ?
Meh.
    Build_Army_List() only tests owner - seems more equivalent
...both called during initialization of their respective screen functions
¿ Build vs. Sort?

Build_Army_List()
    sets m_army_wx, m_army_wy, m_army_wp
    if NOT same_stack ...
    so, if we haven't seen this x,y,p before, add it
    Umm... so, test against all the units that it has already added?
    so, the m_army_wx,y,p is just the unit indices, in order, of each unique location of a unit?


Build_City_List
    sets citylist_city_count
    sets m_city_list_array[] = itr_cities

so, count of array elements and array elements are city indices
ArmList equivalent?

Build_Army_List()
    armylist_unit_count is count of units, by owner
    equivalent to citylist_city_count in Build_City_List()
    but, ...
        has another loop, with same location check
        and, ...
            m_army_wx,y,p = _UNITS[].wx,y,p
            armylist_army_count++
    so, ...
        translates itr_units to itr_armies?

logic is inherently different
    (because cities can not have same location || because units can have same location )

So, ...
    armylist_unit_count is equivalent by definition to citylist_city_count, but is useless
    armylist_army_count is effectively equivalent to citylist_city_count
    m_army_wx,y,p[] are effectively equivalent to m_city_list_array[]

Where does IDK_ArmyList_Units_6x9 fit in?

    Build_Army_List() is called during the initialization portion of Army List Screen
    also, after right-click Unit/Portrait - Unit Statistics Window (pop-up), to reinitialize

    initialization portion of Army List Screen
        Build_Army_List
        list_first_item = 0
        IDK_Armies_Screen_s597F6()

    right-click Unit
        calls IDK_ArmyList_s59569() right before USW
        calls IDK_Armies_Screen_s597F6() right after Build_Army_List()

    right-click Portrait
        calls sub_59B70() right before USW
        calls IDK_Armies_Screen_s597F6() right after Build_Army_List()


IDK_Armies_Screen_s597F6()
    is the thing like with list_cols and col_list?
    calls ArmyList_Set_List_Item_Count()
    iterates over list_item_count
    sets IDK_ArmyList_Units_6x9[itr] = 0
    itrates over _units
    tests owner, x, y, p against m_army_wx[(list_first_item + itr_list_item_count)]
        sets list_armies[armylist_unit_count] = itr_units
        incs armylist_unit_count
        sets IDK_ArmyList_Units_6x9[itr_list] = IDK_ArmyList_Units_6x9[itr_list] + 1;
            ¿ ~== `IDK_ArmyList_Units_6x9[itr_list]++` ?
    So, ...
        updates list_item_count
        ...
        EoD, list_armies[] is the Unit Indices, in order, of all Units that matched *any* location in armylist_world_?_1000, starting from list_first_item
            all Units that are to be displayed, have fields, match input
        starting from 0, IDK_ArmyList_Units_6x9[] is set to 0
        then, on each matched, gets set to += 1
        inherently, matched can only happen 9 times, because that is the max Units that could possibly have the same *location*
        on the first run through, itr_list is 0 and match is on list_first_item + 0
        gets incremented on every match, up to 9, so values are {0, ..., 9}
        only sets itr_list count of values
        ¿ 54 for bytes as 6x9 is misleading ? just [6] of {0, ..., 9}
        elsewhere, ...
            should retrieve the values from IDK_ArmyList_Units_6x9[itr_list] and get that many Unit Indicies from list_armies[], while it draws, add fields, matches input








// WZD o65p02


// WZD o65p03
void CityList_Screen_Load(void);


// WZD o65p06
// ~ MoO2 Build_Global_Colony_List_()
void IDK_N_Cities(void);

// WZD o65p07
void Update_Cities_List(void);


// WZD o66p03
void ArmyList_Add_Fields(void);

// WZD o66p04
// WZD o66p05
// WZD o66p06

// WZD o66p07
void Build_Army_List(void);


// WZD o66p08


// WZD o66p09



// WZD o66p10
void ArmyList_Screen_Load(void);

// WZD o66p11
// WZD o66p12
