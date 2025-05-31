
¿ UNITS are all 18 x 16 ?
¿ FIGURES are all 28 x 30 ?

# Unit Figures



## Overland vs. Combat vs. ¿ ?
(individual) figure pictures are in FIGURE(S).LBX
unit pictures are created by compositing the figure picture
after the base unit picture is created, various effects are applied

Combat_Figure_Load() loads the eight figure lbx entries in to the EMS_PFBA.



## EMM_FIGUREX_Init__HACK()
    USELESS_Combat_Figure_Load_Compose()
    Combat_Figure_Compose_USEFULL()
    j_EMM_FIGUREX_Init__HACK()
        BU_CreateImage__SEGRAX()
        Combat_Figure_Load()
            j_Combat_Figure_Load()
                UU_BU_LoadFigureGFX+2A         call    j_Combat_Figure_Load
                BU_UnitLoadToBattle__SEGRAX+47 call    j_Combat_Figure_Load
                CMB_Units_Init__WIP+C8         call    j_Combat_Figure_Load
                CMB_Units_Init__WIP+27E        call    j_Combat_Figure_Load
                CMB_RaiseDead+39E              call    j_Combat_Figure_Load
                CMB_AnimateDead+362            call    j_Combat_Figure_Load











uses UnitDraw_WorkArea


LOADER.C
Load_Unit_StatFigs()



UnitView.C
Load_Unit_Figure()  AKA Unit_Figure_Load()
Unit_Figure_Draw()
Unit_Figure_Position()



FIGURE LBX
    FIGURES {1,2,3,4,5,6,7,8,9}
    FIGURE  {10,11,12,1,14,15,16}

Unit Type / 15 + 1 is Figure LBX Num
Unit Type % 15 + 2 is Entry Num



### Unit_Figure_Load()


### Unit_Figure_Draw()
    gets passed unit_idx or unit_type, and flag for which
    unit_figure_count = _unit_type_table[unit_type].Figures;
    Unit_Figure_Position(unit_figure_count, itr_figure_count, &fig_y, &fig_x);
    Draw_Picture((x_start + fig_y), (y_start + fig_x), UnitDraw_WorkArea);

    Production_Screen()
    Production_Screen_Draw()
    Production_Screen_Draw_()
    Thing_View_Draw__WIP((popup_base_x + list_field_width), popup_base_y, 0, product_idx, prod_specials_list, UV_specials_count, PS_product_pict_seg);
    Draw_Unit_Figure((UV_x_start_offset + UV_x_start + 11), (UV_y_start - 3), unit_idx, CalledFromFlag);
    

### Unit_Figure_Position()


### UnitDraw_WorkArea



##### Naming Things Is Hard

FIGURES1.LBX
...
FIGURE16.LBX

UNITS1.LBX  "STATFIG1"
UNITS1.LBX  "STATFIG2"

"small figure"
"individual figures"
"individual unit pictures"
"When units are moving over long distances, a “G” appears on their pictures."
"Engineers that are busy constructing roads show an “R” on their pictures."
"If the unit is wielding certain magic weapons, this is represented by a small sword on the unit’s picture."
"You can also view the troops garrisoned in a neutral or enemy city by running the mouse cursor over the small unit pictures above the cityscape window (this identifies the unit types)."
"Information about units can be obtained by right-clicking on their pictures in most of the game screens."
"The number of figures in a unit is shown in the picture in the upper left corner of the unit statistics window."
"Experience levels are indicated on the unit pictures (in the main movement screen, armies screen, city screen, etc.) by colored circles of silver and gold."
"Just as for normal units, right-clicking on the hero pictures in most of the game screens brings up the hero’s unit statistics window."
"The done button ends a unit’s turn. The unit can only be made active during that turn again by clicking on its picture in the combat screen."
"In addition, a damage bar runs along the bottom edge of the active unit’s picture."
"A unit can be activated by right-clicking on the three-dimensional picture in the combat grid."
"To decrease your maintenance costs, you may decide for yourself which creatures to dismiss (right-click on creature pictures to bring up their unit statistics window and then click on the dismiss button)..."
