
#include "MoX.H"

#include "PNG_Draw.H"
#include "PNG_Load.H"

#include "..\3PL\lodepng\lodepng.h"

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"



struct s_PNG_PICT* PNG_Load(char* png_file_name)
{
    unsigned error;
    struct s_PNG_PICT * png_pict;
    uint8_t * png_image;
    uint32_t png_width;
    uint32_t png_height;
    uint16_t num_blocks;
    uint32_t entry_length;

    png_pict = ST_NULL;
    png_image = ST_NULL;
    png_width = ST_NULL;
    png_height = ST_NULL;

    error = lodepng_decode32_file(&png_image, &png_width, &png_height, png_file_name);

    if (png_image != ST_NULL && &png_width != ST_NULL && png_height != ST_NULL)
    {
        // if (error) printf("decoder error %u: %s\n", error, lodepng_error_text(error));

        entry_length = png_width * png_height;
        num_blocks = 1 + (entry_length / SZ_PARAGRAPH_B);
        png_pict = (struct s_PNG_PICT*)Allocate_Space_No_Header(num_blocks);

        if (png_pict != ST_NULL)
        {
            png_pict->png_width = png_width;
            png_pict->png_height = png_height;
            png_pict->png_pixels = png_image;
        }
        else
        {
            // The System is Down
            // It's all gone wrong, probably because *you* have made a *huge mistake*
        }
    }

    return png_pict;
}
