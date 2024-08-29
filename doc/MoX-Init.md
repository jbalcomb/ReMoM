


Displaying what has been drawn implicitly depends on the palette
!~"Video", but ... ?
IDK
also, *palette* is part of *font*
so, MUST Load_Font_File()
~== MUST Init_Drivers()

...evenutally, *drawing* also depends on *mouse*, because of the highlight and button up/down state
...also, *cursor* inherently overlaps with *keyboard*, because it provides the UX of indicating where space/enter/escape will trigger


¿ Set_Button_Down_Offsets() ?
How/Why would this ever be different?
Could/Should/Would just be part of the input initialization?

¿ Calculate_Remap_Colors() depends on Load_Palette() ?
¿ Cycle_Palette_Color() depends on Load_Palette() ?

¿ Calculate_Remap_Colors() impacts on Apply_Palette() ?
¿ Cycle_Palette_Color() impacts on Apply_Palette() ?

¿ Are/Can be considered 'Per Screen' - Calculate_Remap_Colors(); Cycle_Palette_Color(); ?





Load_Palette()

Calculate_Remap_Colors()

Set_Button_Down_Offsets()

Cycle_Palette_Color();

Apply_Palette()






Game-Engine Initialization



## Game vs. Tools & Tests

Configuration?



### Video

allocate video back buffers
assign off-page index, for drawing


### Graphics

allocate screen/cache/scratch buffers


### World (Data)

~== ALLOC Allocate_Data_Space()


