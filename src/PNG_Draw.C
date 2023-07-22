
#include "MoX_TYPE.H"
#include "PNG_Draw.H"
#include "Video.H"

void PNG_Draw(int x_start, int y_start, struct s_PNG_PICT* png_pict)
{
    uint32_t w;
    uint32_t h;
    unsigned int x, y;
    // IDGI  int jump = 1;
    int screenw, screenh, pitch;
    // uint32_t * sdl_pixels;
    uint32_t * bbuff_pos;

    w = png_pict->png_width;
    h = png_pict->png_height;

    // /* avoid too large window size by downscaling large image */
    // IDGI  if (w / 1024 >= jump) jump = w / 1024 + 1;
    // IDGI  if (h / 1024 >= jump) jump = h / 1024 + 1;

    // IDGI  screenw = w / jump;
    // IDGI  screenh = h / jump;
    screenw = w;
    screenh = h;
    pitch = screenw * sizeof(uint32_t);

    // sdl_pixels = (uint32_t *)malloc(screenw * screenh * sizeof(uint32_t));
    // if (!sdl_pixels) {
    //     printf("Failed to allocate pixels\n");
    //     return 0;
    // }
    bbuff_pos = (uint32_t * )((video_page_buffer_XBGR[1 - draw_page_num]) + ((y_start * screen_pixel_width) + x_start));

//get DIB bits
//for each pixel
//r = r * alpha / 255
//g = g * alpha / 255
//b = b * alpha / 255
//set DIB bits

    /* plot the pixels of the PNG file */
    // IDGI  for (y = 0; y + jump - 1 < h; y += jump)
    for (y = 0; y - 1 < h; y ++)
    {
        // IDGI  for (x = 0; x + jump - 1 < w; x += jump)
        for (x = 0; x - 1 < w; x++)
        {
            /* get RGBA components */
            uint32_t r = png_pict->png_pixels[4 * y * w + 4 * x + 0]; /* red */
            uint32_t g = png_pict->png_pixels[4 * y * w + 4 * x + 1]; /* green */
            uint32_t b = png_pict->png_pixels[4 * y * w + 4 * x + 2]; /* blue */
            uint32_t a = png_pict->png_pixels[4 * y * w + 4 * x + 3]; /* alpha */

            /* make translucency visible by placing checkerboard pattern behind image */
            int checkerColor = 191 + 64 * (((x / 16) % 2) == ((y / 16) % 2));
            r = (a * r + (255 - a) * checkerColor) / 255;
            g = (a * g + (255 - a) * checkerColor) / 255;
            b = (a * b + (255 - a) * checkerColor) / 255;

            /* give the color value to the pixel of the screenbuffer */
            // sdl_pixels[(y * screenw + x) / jump] = 65536 * r + 256 * g + b;
            // IDGI  bbuff_pos[(y * screenw + x) / jump] = 65536 * r + 256 * g + b;
            bbuff_pos[(y * screenw + x)] = 65536 * r + 256 * g + b;
        }
    }

}
