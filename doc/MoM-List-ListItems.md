
List
List Items

ArmyList
CityList





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
    city_list[] is all cities that match owner
¿ ~== MoO2  Build_Global_Colony_List_() ?
Meh.
    Build_Army_List() only tests owner - seems more equivalent
...both called during initialization of their respective screen functions
¿ Build vs. Sort?

Build_Army_List()
    sets armylist_world_x_1000, armylist_world_y_1000, armylist_world_p_1000
    if NOT same_stack ...
    so, if we haven't seen this x,y,p before, add it
    Umm... so, test against all the units that it has already added?
    so, the armylist_world_?_1000 is just the unit indices, in order, of each unique location of a unit?


Build_City_List
    sets citylist_city_count
    sets city_list[] = itr_cities

so, count of array elements and array elements are city indices
ArmList equivalent?

Build_Army_List()
    armylist_unit_count is count of units, by owner
    equivalent to citylist_city_count in Build_City_List()
    but, ...
        has another loop, with same location check
        and, ...
            armylist_world_?_1000 = _UNITS[].world_?
            armylist_army_count++
    so, ...
        translates itr_units to itr_armies?

logic is inherently different
    (because cities can not have same location || because units can have same location )

So, ...
    armylist_unit_count is equivalent by definition to citylist_city_count, but is useless
    armylist_army_count is effectively equivalent to citylist_city_count
    armylist_world_?_1000[] are effectively equivalent to city_list[]

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
    tests owner, x, y, p against armylist_world_x_1000[(list_first_item + itr_list_item_count)]
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
