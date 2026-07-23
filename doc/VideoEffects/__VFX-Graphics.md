
¿ All Graphics / VFX ?



all/part of ...
...seg016
...seg033

DEDU  Why is Gradient_Fill() in seg014?


Elsewhere, ...
    Vanish_Bitmap()
        |-> Vanish_Bitmap_Dither_Seed
            _CS_random_seed


XREF's of ...

...called by Gradient_Fill()

...calls UU_VGA_CreateColorWave()

...uses UU_Wave_Colors[]

...uses UU_RNG_7bit_Lookup[]

...uses VGA_LeftBits[]

...uses VGA_RightBits[]



CS_width
XREF:
    Mask_Bitmap_Pixels+1D
    Mask_Bitmap_Pixels+3E
    Mask_Bitmap_Pixels_Inverse+1D
    Mask_Bitmap_Pixels_Inverse+3E
    Reduce_Bitmap+58
    Reduce_Bitmap+9B
    Enlarge_Bitmap+72
    Enlarge_Bitmap+B9
    Vanish_Bitmap+23
    Vanish_Bitmap+63



Gradient_Fill()
    |-> UU_VGA_Columns()
        |-> UU_VGA_CreateColorWave()
        UU_Wave_Colors[]
        UU_RNG_7bit_Lookup[]


Gradient_Fill()
    |-> UU_VGA_DiagColumns()
        |-> UU_VGA_CreateColorWave()
        UU_Wave_Colors[]
        UU_RNG_7bit_Lookup[]

Gradient_Fill()
    |-> UU_VGA_ScrambleRect()
        |-> UU_RNG_7bit_Lookup()
        VGA_LeftBits[]
        VGA_RightBits[]
        UU_RNG_7bit_Lookup[]
