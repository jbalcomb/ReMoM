

Movement Bar

movement points
movement modes

Unit Window



### ¿ OVL_GetMoveTypes() vs. STK_GetMoveTypes() ?
Draw_Movement_Mode_Icons()
    |->
    ...no unit specified, but have active stack

OVL_GetMoveTypes()
    if all stack units are inactive, use all units
    else use stack units that are active
    then call STK_GetMoveTypes()
    doesn't short-circuit on _unit_stack_count, so assuming its been tested already
    gets tested in Draw_Movement_Mode_Icons(), and can't be 0 in-combat
So, ...
    ~ Active Stack Movement Modes
    ~ Stack Movement Modes


STK_GetMoveTypes()
iterates on count of movement modes
iterates on count of units in stack

Draw_Movement_Mode_Icons()
is looking for a struct of the movement modes 
struct s_Movement_Modes movement_mode_flags;
passed to Stack_Movement_Modes() or passed to Active_Stack_Movement_Modes() then passed to Stack_Movement_Modes()
builds and passes stack array to Stack_Movement_Modes() or 
    Active_Stack_Movement_Modes() builds and passes stack array to Stack_Movement_Modes()

¿ mode priority order ?
planar travel



0 /* M_Sail_Icon */
1 /* M_Swim_Icon */
2 /* M_Mntn_Icon */
3 /* M_Forest_Icon */
4 /* M_Fly_Icon */
5 /* M_Pathf_Icon */
6 /* M_PShift_Icon */
¿ wind ?
8 /* M_Grnd_Icon */
FTLOGAATIH, where is the check for Wind Walking?



sizeof(movement_mode_flags[0]) is 2

Draw_Movement_Mode_Icons()
loops for 9 on movement_mode_flags[]
maybe was an ASM STRUC? so, only shows offsets?



-0016 movement_mode_flags dw 6 dup(?)
-000A Move_PlaneShift dw ?
-0008 Move_WindWalk dw ?
-0006 Move_Ground dw ?



dseg:9978

move_sail_icon
move_swim_icon
move_mt_icon
move_forest_icon
move_fly_icon
move_path_icon
move_astral_icon
move_wind_icon
move_boot_icon
/* ¿ cavalry ? */ dseg:998A 00 00                                           dw 0

move_sail_icon = LBX_Load("MAIN", 18)
move_swim_icon = LBX_Load("MAIN", 19)
move_mt_icon = LBX_Load("MAIN", 20)
move_forest_icon = LBX_Load("MAIN", 21)
move_fly_icon = LBX_Load("MAIN", 22)
move_path_icon = LBX_Load("MAIN", 23)

move_astral_icon = LBX_Load("MAIN", 36)
move_wind_icon = LBX_Load("MAIN", 37)
move_boot_icon = LBX_Load("MAIN", 38)

18    MAINMOVE    sail
19    MAINMOVE    swim
20    MAINMOVE    mt.
21    MAINMOVE    forest
22    MAINMOVE    fly
23    MAINMOVE    path

36    MAINMOVE    astral
37    MAINMOVE    wind
38    MAINMOVE    boot








dseg:33B8 01 00 20 00 40 00 04 00 02 00 08 00             MoveFlag_Array dw M_Cavalry, M_Forester, M_Mntnr, M_Swimming, M_Sailing, M_Flying
dseg:33B8                                                                                         ; DATA XREF: STK_GetMoveTypes+10o
dseg:33B8                                                                                         ; drake178: MoveFlag_Array



STK_GetMoveTypes()
code shows using (F_)SCOPY@ ...far struct copy routine...generate by the compiler...
so, MoveType_Flags and MoveFlag_Array are structures?


int16_t MoveType_Flags[6];  // = { 0x0001, 0x0020, 0x0040, 0x0004, 0x0002, 0x0008 }

struct s_Movement_Modes = 
{
    uint16_t Cavalry = 0x0001;
    uint16_t Forester = 0x0020;
    uint16_t Mountaineer = 0x0040;
    uint16_t Swimming = 0x0004;
    uint16_t Sailing = 0x0002;
    uint16_t Flying 0x0008;
};

/*
(PDF Page 121)
"Basic Movement Types"
"Movement Mode", "Graphic Representation", "Allowed Movement"

"sailing unit (ship)"
*/









"Moves:"
{Walking, Forester, Mountaineer, Flying, Pathfinding, Swimming, Sailing, Wind Walking, Planar Travel}

MainScr.C

Main_Screen_Draw_Do_Draw()

Main_Screen_Draw_Movement_Bar()
OVL_GetStackHMoves()
OVL_DrawMoveTypes()


array of 9 movement modes



Stack_Has_Planar_Travel()
    Active_Stack_On_Astral_Gate()
    Unit_Has_Planar_Travel_Item()
        UNIT_BU_ApplyItems()







Print_Moves_String(x,y,val,flag)



ovr097 seems a bit of a mish-mash

CMB_DrawUnitDisplay()
    calls j_VGA_DrawHalfValue() or Print_Integer_Right()
CMB_DrawAUWStats()
    calls j_VGA_DrawHalfValue() or Print_Integer_Right()



manually draws the *shadow* for "Moves:" and movement points

Main_Screen_Draw_Movement_Bar()
    ..._help_entries[170] = HLP_MOVES  /* 170d  AAh */
    ...if(_unit_stack_count > 0)

    movement_points = OVL_GetStackHMoves();
            
    Set_Font_Style1(1,1,0,0);
    Set_Outline_Color(0);
    Set_Alias_Color(0);
    Print(246, 167, cnst_Moves);
    VGA_DrawHalfValue(275, 167, movement_points, 0);
    Set_Font(1,1,0,0);
    Set_Outline_Color(0);
    Set_Alias_Color(0);
    Print(246, 168, cnst_Moves);
    VGA_DrawHalfValue(275, 168, movement_points, 0);
    Set_Font(1,0,0,0);
    Set_Alias_Color(0);
    Print(246, 167, cnst_Moves);
    VGA_DrawHalfValue(275, 167, movement_points, 0);

    Draw_Movement_Mode_Icons(286, 167, ST_UNDEFINED);


UnitMove.C

/*
    WIZARDS.EXE  ovr071
*/


// WZD o71p01
// drake178: OVL_GetMoveTypes()
void OVL_GetMoveTypes(int16_t * MoveTypes);

// WZD o71p02
// drake178: STK_GetMoveTypes
void STK_GetMoveTypes(int16_t * MoveTypes, int_16_t Stack_Array, int16_t Stack_Size);


ovr063
// WZD o63p12
OVL_GetStackHMoves()





// WZD o71p02
// drake178: STK_GetMoveTypes
void STK_GetMoveTypes(int16_t * MoveTypes, int_16_t Stack_Array, int16_t Stack_Size)
{
    int16_t MoveType_Flags[6];  // = { 0x0001, 0x0020, 0x0040, 0x0004, 0x0002, 0x0008 }
    int16_t Swimming_Units;
    int16_t Flying_Units;
    uint16_t Item_Enchants_LO;
    uint16_t Item_Enchants_HI;
    uint32_t Item_Enchants;
    int16_t Units_With_Same;
    int16_t Unit_Type;
    int16_t Unit_Index;
    int16_t Stack_Index;
    int16_t itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: STK_GetMoveTypes()\n", __FILE__, __LINE__);
#endif

// mov     di, [bp+MoveTypes@]

    // drake178: ; internal compiler routine to copy "struct" values  ; cx = number of bytes to copy  ; source: borlandc\crtl\clib\f_scopy.asm
    // SCOPY@(12, &MoveFlag_Array, &MoveType_Flags);
    // WZD dseg:33B8 01 00 20 00 40 00 04 00 02 00 08 00
    // MoveFlag_Array dw M_Cavalry, M_Forester, M_Mntnr, M_Swimming, M_Sailing, M_Flying

    Flying_Units = ST_FALSE;
    Swimming_Units= ST_FALSE;

    for(itr = 0; itr < 6; itr++)
    {
        *(MoveTypes + itr) = 0;
        Units_With_Same = 0;
        Item_Enchants_HI = 0;
        Item_Enchants_LO = 0;
        Stack_Index = 0;

        if(Stack_Index < Stack_Size)
        {
            Unit_Index = Stack_Array[Stack_Index];
            Unit_Type = _UNIT[Unit_Index].type;

            Item_Enchants_HI = 0x0000;
            Item_Enchants_LO = 0x0000;

            if(_UNIT[Unit_Index].Hero_Slot != ST_UNDEFINED)
            {
                // Active_Unit@  ; 8 LBX_Alloc_Space paragraphs (128 bytes)  ; contains a single battle unit record (110 bytes)
                // ; converts item powers into attack and enchantment flags, applying them to the battle unit along with any stat boosts from items; with the flags being ; zeroed instead if no effects apply or the unit is not a hero  ; returns the item unit enchantment flags
                // Item_Enchants_HI:Item_Enchants_LO = UNIT_BU_ApplyItems(Unit_Index, Active_Unit);
                // Item_Enchants_HI = Active_Unit.Item_UEs_H
                // Item_Enchants_LO = Active_Unit.Item_UEs_L
            }
        }

        if(_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x20)  /* M_Forester */
        {
            *(MoveTypes + itr) = 1;
        }
        if(_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x40)  /* M_Mntnr */
        {
            *(MoveTypes + itr) = 1;
        }
        if(_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x02)  /* M_Sailing */
        {
            *(MoveTypes + itr) = 1;
        }
        if( (_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x08) & (_unit_type_table[Unit_Type].Transport == ST_FALSE) )  /* M_Flying */
        {
            *(MoveTypes + itr) = 1;
        }
        /* M_Forester */  /* UE_Path_Finding */  /* UE_Path_Finding */
        if( (_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x20) & ( ((_UNIT[Unit_Index].Enchants_LO & 0x80) != 0) || ((Item_Enchants_LO & 0x80) != 0) ) )
        {
            *(MoveTypes + itr) = 1;
        }
        /* M_Mntnr */  /* UE_Path_Finding */  /* UE_Path_Finding */
        if( (_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x40) & ( ((_UNIT[Unit_Index].Enchants_LO & 0x80) != 0) || ((Item_Enchants_LO & 0x80) != 0) ) )
        {
            *(MoveTypes + itr) = 1;
        }
        /* M_Flying */  /* UE_Wind_Walking */  /* Ab_WindWalking */
        if( (_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x08) & ( ((_UNIT[Unit_Index].Enchants_HI & 0x01) != 0) || ((_unit_type_table[Unit_Type].Abilities & 0x1000) != 0) ) )
        {
            *(MoveTypes + itr) = 1;
        }
        /* M_Flying */  /* UE_Wind_Walking || UE_Flight */
        if( (_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr) == 0x08) & ( ((_UNIT[Unit_Index].Enchants_HI & 0x03) != 0) & (_unit_type_table[Unit_Type].Transport == ST_FALSE) ) )
        {
            *(MoveTypes + itr) = 1;
        }


        if( ((_unit_type_table[Unit_Type].Move_Flags & *(MoveType_Flags + itr)) & *(MoveType_Flags + itr)) != 0 )
        {
            Units_With_Same++;

            if( *(MoveType_Flags + itr) == 0x04) /* M_Swimming */
            {
                Swimming_Units++;
            }

            // BUG:  double counts units with flying ability
            if( *(MoveType_Flags + itr) == 0x08) /* M_Flying */
            {
                Flying_Units++;
            }

            /* Ab_NonCorporeal */  /* UE_Wraith_Form */  /* UE_Wraith_Form */
            if( ((_unit_type_table[Unit_Type].Abilities & 0x0800) != 0) & ((_UNIT[Unit_Index].Enchants_LO & 0x20) != 0) & ((Item_Enchants_LO & 0x20) != 0) )
            {
                Units_With_Same--;
            }

            // BUG: only checks (M_Flying & Ab_NonCorporeal)  if( (*(MoveType_Flags + itr) == 0x08) & ( ((_unit_type_table[Unit_Type].Abilities & 0x0800) != 0) || ((_UNIT[Unit_Index].Enchants_LO & 0x20) != 0) || ((Item_Enchants_LO & 0x20) != 0) ) )
            /* M_Flying */  /* Ab_NonCorporeal */  /* UE_Wraith_Form */  /* UE_Wraith_Form */
            if( (*(MoveType_Flags + itr) == 0x08) & ((_unit_type_table[Unit_Type].Abilities & 0x0800) != 0) || ((_UNIT[Unit_Index].Enchants_LO & 0x20) != 0) || ((Item_Enchants_LO & 0x20) != 0) )
            {
                Swimming_Units--;
            }

            // BUG:  double counts units with swimming ability
            /* M_Swimming */  /* UE_Water_Walking */  /* UE_Water_Walking */
            if( (*(MoveType_Flags + itr) == 0x04) & || ((_UNIT[Unit_Index].Enchants_LO & 0x0100) != 0) || ((Item_Enchants_LO & 0x0100) != 0) )
            {
                Units_With_Same++;
                Swimming_Units++;
            }

            /* M_Swimming */  /* Ab_NonCorporeal */  /* UE_Wraith_Form */  /* UE_Wraith_Form */
            if( (*(MoveType_Flags + itr) == 0x04) & ( ((_unit_type_table[Unit_Type].Abilities & 0x0800) != 0) || ((_UNIT[Unit_Index].Enchants_LO & 0x20) != 0) || ((Item_Enchants_LO & 0x20) != 0) ) )
            {
                Units_With_Same++;
                Swimming_Units++;
            }

            // S'what?  ...is flying or swimming, but also immaterial?
            /* M_Flying */  /* UE_Wind_Walking || UE_Flight */  /* UE_Wind_Walking || UE_Flight */  /* CC_Flight */  /* UE_Water_Walking */  /* UE_Water_Walking */  /* Ab_NonCorporeal */  /* UE_Wraith_Form */  /* UE_Wraith_Form */
            if( (*(MoveType_Flags + itr) == 0x08) & ( ((_UNIT[Unit_Index].Enchants_HI & 0x03) != 0) || ((Item_Enchants_HI & 0x03) != 0) || ((_UNIT[Unit_Index].Mutations & 0x08) != 0) ) & ( ((_UNIT[Unit_Index].Enchants_LO & 0x0100) != 0) || ((Item_Enchants_LO & 0x0100) != 0) ) & ( ((_unit_type_table[Unit_Type].Abilities & 0x0800) != 0) || ((_UNIT[Unit_Index].Enchants_LO & 0x20) != 0) || ((Item_Enchants_LO & 0x20) != 0) ) )
            {
                Swimming_Units--;
            }
        }

        if(Units_With_Same >= Stack_Size)
        {
            MoveTypes[tri] = 1;
        }

    }

    if(Swimming_Units + Flying_Units >= Stack_Size)
    {
        // MoveTypes@+MoveType_Matrix.Swimming = ST_TRUE
        if(Flying_Units < 1 || Swimming_Units < 1)
        {
            // MoveTypes@+MoveType_Matrix.Swimming = ST_TRUE
            // MoveTypes@+MoveType_Matrix.Flying = ST_TRUE
            if(Flying_Units == 0)
            {
                // MoveTypes@+MoveType_Matrix.Flying = ST_FALSE
            }
            if(Swimming_Units == 0)
            {
                // MoveTypes@+MoveType_Matrix.Swimming = ST_FALSE
            }
        }
    }


    // MoveTypes@+MoveType_Matrix.UU_Cavalry = ST_FALSE

    Units_With_Same = 0;

    for(Stack_Index = 0; Stack_Index < Stack_Size; Stack_Index++)
    {
        Unit_Index = Stack_Array[Stack_Index];
        Unit_Type = _UNIT[Unit_Index].type;
        /* Ab_NonCorporeal */  /* UE_Wraith_Form */
        // ¿ drake178: BUG: ignores the item power ?
        if( ((_unit_type_table[Unit_Type].Abilities & 0x0800) != 0) || ((_UNIT[Unit_Index].Enchants_HI & 0x20) != 0) )
        {
            Units_With_Same++;
        }
    }


    if(Units_With_Same == Stack_Size)
    {
        // MoveTypes@+MoveType_Matrix.Forester = ST_TRUE
        // MoveTypes@+MoveType_Matrix.Mountaineer = ST_TRUE
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: STK_GetMoveTypes()\n", __FILE__, __LINE__);
#endif
}

