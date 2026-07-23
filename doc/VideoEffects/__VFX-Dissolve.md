
VFX
Graphics
...angle, shear
...FLIC_Draw

SEEALSO: C:\STU\devel\ReMoM\doc\#CodeReview\Video-PageFlip_Dissolve.md



PageFlip_FX()

PageFlip_Dissolve()

Draw_Expanding_Bitmap()

Dissolve_Main_Screen()



void PageFlip_FX(void)
{

    switch(_page_flip_effect)
    {
//         case pfe_None:
//         {
//             Apply_Palette();
//             Toggle_Pages();  // |-> Page_Flip()
//         } break;
        case pfe_VGA_CutRight:
        {
            // TODO  RP_VGA_CutRight();
        } break;
        case pfe_TogglePagesFadeIn:
        {
            Toggle_Pages();  // |-> Page_Flip()
            Fade_In();
        } break;
        case pfe_Dissolve:
        {
            Apply_Palette();
            PageFlip_Dissolve();  // s026p01;  |-> Toggle_Pages_No_Draw_Fields() |-> Page_Flip()
            Toggle_Pages();  // |-> Page_Flip()
        } break;
        case pfe_GrowOut:
        {
            Draw_Expanding_Bitmap(GrowOutLeft, GrowOutTop, GrowOutFrames, (_screen_seg + (20 * SCREEN_WIDTH)));
        } break;
        case pfe_Palette:
        {
            Apply_Palette();
        } break;
        default:
        {
            Apply_Palette();
            Toggle_Pages();  // |-> Page_Flip()
        } break;
    }

    _page_flip_effect = pfe_None;

}
