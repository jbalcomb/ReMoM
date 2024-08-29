



lotsa biz with checking if the mouse cursor is available for clicking
    ... 




## Fields
Alchemy
Ok
Anvil
Hero Portrait
Hero Item Slots
Vault Item Slots
Gold
Mana
ESCAPE
¿ "..." ? left-click outside window

Left-Click
Right-Click
    'Unit Statistics Window'
    'Item View' or 'Help - Item Slot Type'
    ALCHEMY
    OK
    ANVIL
    TOTAL GOLD
    TOTAL MANA




### ITEM_HeroLabels

WZD dseg:C274
int16_t * ITEM_HeroLabels

Item_Screen_Load()
    ITEM_HeroLabels = Near_Allocate_Next(12)

Item_Screen_Add_Fields()
    ITEM_HeroLabels = -1000  INVALID_FIELD
    ITEM_HeroLabels[itr1] = Add_Hidden_Field()

Item_Screen()



### ITEM_ClickLabels

WZD dseg:C276
int16_t * ITEM_ClickLabels[6]

Item_Screen_Load()
    for itr 0 to 5
    ITEM_ClickLabels[itr1] = Near_Allocate_Next(6)

Item_Screen_Add_Fields()
    ITEM_ClickLabels[itr1][0] = -1000  INVALID_FIELD
    ITEM_ClickLabels[itr1][1] = -1000  INVALID_FIELD
    ITEM_ClickLabels[itr1][2] = -1000  INVALID_FIELD
    ITEM_ClickLabels[itr1][itr2] = Add_Hidden_Field()







