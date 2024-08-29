
Armies
Armylist Screen
ARMYLIST.LBX



the army list



ArmyList_UnitFigure_Coordinates()
calculates the coordinates from the unit figure icon to the *grow-out* for the UV/USW




Main_Screen()
armies button
hotkey 'A'

ArmyList_Screen()
ArmyList_Screen_Draw()

ArmyList_Screen()
    ArmyList_Screen_Load();
    Build_Army_List();
    list_first_item = 0;
    Build_Army_List_Counts();
        ArmyList_Add_List_Fields();

        button_armylist_items = Add_Button_Field(273, 163, "", armylist_items_button_seg, cnst_HOTKEY_I_2, ST_UNDEFINED);

        button_armylist_ok = Add_Button_Field(273, 182, "", armylist_ok_button_seg, cnst_HOTKEY_O_4, ST_UNDEFINED);
        
        hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc5);

Up/Down Buttons
hotkey 'U', 'D'
button up right, up left, down right, down left
if(list_first_item != 0)
    ...do up fields
if((((list_first_item + 6) - armylist_army_count) != 0) && ((list_item_count - armylist_army_count) != 0))
    ...do down fields





##### Naming Things Is Hard

armies screen  
upkeep window  
highlighted unit  
activates  
main movement screen  
reduced map window  
unit enchantment window  
the army list  
blinking white dot  
enchantment icon  
group of stacked units  
map square  
detailed description of the unit  


ARMYLIST.LBX

000  ARMYLIST   armylist backgrnd
001  ARMYLBUT   armylist up but
002  ARMYLBUT   armylist down but
003  ARMLBUT2   armylist items but
004  ARMLBUT2   armylist ok but
005  HEROBACK   hero background
006  HEROBAC2   hero outline
007  HEROBUT4   hero alchemy butt
008  HEROBUT4   hero ok butt


Page 26  (PDF Page 31)  

Armies  
Clicking on the armies button takes you to the armies screen.  
This screen provides a complete list of all your armies and heroes and gives you access to the items screen.  
You can move through the army list quickly by using the arrow buttons that are located on either side of the list.  
Units that are stacked in the same map square are grouped together on the armies screen.

Hero portrait
Hero units
Army units
Upkeep summary for all units
Reduced map window
Name of highlighted unit
Unit enchantment window

Clicking on such a group of stacked units takes you directly to the map square which the group is occupying.  
Placing your mouse cursor over a unit on the list highlights that unit and causes the following:  
· All current enchantments on the highlighted unit are listed in the unit enchantment window underneath the army list.  
· The location of the highlighted unit is shown as a blinking white dot in the reduced map window underneath the army list.  
  Enemy troops and cities are shown in this reduced map window as dots in the color of the controlling wizard.  
· Right-clicking on the highlighted unit presents a detailed description of the unit (see Basic Units Information).  
  You can dismiss the unit from your army by clicking on the dismiss button.  
  You can also remove any enchantments that were cast by your wizard from the unit by clicking on the enchantment icon.  
· Clicking on the highlighted unit activates it and finds the unit for you by taking you to the map square it is occupying in the main movement screen.  

In the bottom-left corner of the armies screen is the upkeep window.  
This displays how much total gold, mana and food is required each turn to maintain your armies. Most armies and enchantments require some sort of maintenance cost.  
The upkeep window is a way for your wizard to conveniently examine and readjust the numbers of armies (by dismissal) and enchantments (by removal) so that any upkeep costs can be adjusted to reasonable values.  
Clicking on the OK button returns you to the main movement screen at the same position you were when you entered the armies screen.  
