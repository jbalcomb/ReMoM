



image resize
upsize, downsize
upscale, downscale
shrink, grow
zoom, zoom in, zoom out

sampling / resampling
nearest neighbor

interpolated, non-interpolated

scale vs. ratio
¿ independent axis ?
pitch
pass src w,h and dst w,h, then calculate scale




https://homepages.inf.ed.ac.uk/rbf/HIPR2/affine.htm

the operation of the affine transformation by applying a series of special-case transformations
e.g., pure scaling

 https://homepages.inf.ed.ac.uk/rbf/HIPR2/scale.htm

Image zooming is achieved by pixel replication or by interpolation.

Scaling is a special case of affine transformation.

An image (or regions of an image) can be zoomed either through pixel replication or interpolation. Figure 2 shows how pixel replication simply replaces each original image pixel by a group of pixels with the same value (where the group size is determined by the scaling factor). Alternatively, interpolation of the values of neighboring pixels in the original image can be performed in order to replace each pixel with an expanded group of pixels. Most implementations offer the option of increasing the actual dimensions of the original image, or retaining them and simply zooming a portion of the image within the old image boundaries.

https://homepages.inf.ed.ac.uk/rbf/HIPR2/figs/scalzoom.gif

Figure 2 Methods of zooming. a) Replication of a single pixel value. b) Interpolation.

References
E. Davies Machine Vision: Theory, Algorithms and Practicalities, Academic Press, 1990, pp 465 - 469, 470.
R. Gonzalez and P. Wintz Digital Image Processing, 2nd edition, Addison-Wesley Publishing Company, 1987, pp 22 - 26.
A. Jain Fundamentals of Digital Image Processing, Prentice-Hall, 1986, Chap. 4.
A. Marion An Introduction to Image Processing, Chapman and Hall, 1991, Chap. 5.
D. Vernon Machine Vision, Prentice-Hall, 1991
Local Information
Specific information about this operator may be found here.
More general advice about the local HIPR installation is available in the Local Information introductory section.
©2003 R. Fisher, S. Perkins, A. Walker and E. Wolfart.

https://homepages.inf.ed.ac.uk/rbf/HIPR2/hipr_top.htm





https://www.programcreek.com/cpp/?CodeExample=resize+image

Example 5
Source File: ImageResize.cpp    From BlueshiftEngine with Apache License 2.0	6 votes	vote downvote up
void ResizeImageNearest(const T *src, int srcWidth, int srcHeight, T *dst, int dstWidth, int dstHeight, int numComponents) {
    int srcPitch = srcWidth * numComponents;

    float ratioX = (float)srcWidth / dstWidth;
    float ratioY = (float)srcHeight / dstHeight;

    for (int y = 0; y < dstHeight; y++) {
        int iY = y * ratioY;

        int offsetY = iY * srcPitch;

        for (int x = 0; x < dstWidth; x++) {
            int iX = x * ratioX;

            int offsetX = iX * numComponents;
        
            const T *srcPtrY = &src[offsetY];

            for (int i = 0; i < numComponents; i++) {
                *dst++ = srcPtrY[offsetX + i];
            }
        }
    }
} 

|-> https://www.programcreek.com/cpp/?code=PolygonTek%2FBlueshiftEngine%2FBlueshiftEngine-master%2FSource%2FRuntime%2FPrivate%2FImage%2FImageResize.cpp
|-> |-> https://github.com/PolygonTek/BlueshiftEngine/blob/master/Source/Runtime/Private/Image/ImageResize.cpp





https://stackoverflow.com/questions/41861274/scaling-up-an-image-using-nearest-neighbor

Notes (makeBigger related - designed to replace the content of the image given as argument):  
* Construct a temporary image that will be the enlarged one  
* Only traverse the temporary image once (populate its pixels as we allocate them); to maintain scaling to the original image and make sure that the appropriate pixel is "copied" into the new one, simply divide the indexes by the scaling factor: tmp->pixels[i][j] = img->pixels[i / scale][j / scale]  
* Deallocate the original image content: since each pixel row is malloced, it should also be freed (free(img->pixels); alone will yield memory leaks)  
* Store the temporary image content (into the original one) and then deallocate it  
```c
void makeBigger(Image *img, int scale)
{
    uint32_t i = 0, j = 0;
    Image *tmp = (Image*)malloc(sizeof(Image));
    tmp->height = img->height * scale;
    tmp->width = img->width * scale;

    tmp->pixels = (Pixel**)malloc(sizeof(Pixel*) * tmp->height);

    for (i = 0; i < tmp->height; i++)
    {
        tmp->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * tmp->width);
        for (j = 0; j < tmp->width; j++)
        {
            tmp->pixels[i][j] = img->pixels[i / scale][j / scale];
        }
    }

    for (i = 0; i < img->height; i++)
        free(img->pixels[i]);
    free(img->pixels);

    img->width = tmp->width;
    img->height = tmp->height;
    img->pixels = tmp->pixels;
    free(tmp);
}
```





https://stackoverflow.com/questions/28566290/optimize-a-nearest-neighbor-resizing-algorithm-for-speed

```c
void resizeNearestNeighbor(const uint8_t* input, uint8_t* output, int sourceWidth, int sourceHeight, int targetWidth, int targetHeight)
{
    const int x_ratio = (int)((sourceWidth << 16) / targetWidth);
    const int y_ratio = (int)((sourceHeight << 16) / targetHeight) ;
    const int colors = 4;

    for (int y = 0; y < targetHeight; y++)
    {
        int y2_xsource = ((y * y_ratio) >> 16) * sourceWidth;
        int i_xdest = y * targetWidth;

        for (int x = 0; x < targetWidth; x++)
        {
            int x2 = ((x * x_ratio) >> 16) ;
            int y2_x2_colors = (y2_xsource + x2) * colors;
            int i_x_colors = (i_xdest + x) * colors;

            output[i_x_colors]     = input[y2_x2_colors];
            output[i_x_colors + 1] = input[y2_x2_colors + 1];
            output[i_x_colors + 2] = input[y2_x2_colors + 2];
            output[i_x_colors + 3] = input[y2_x2_colors + 3];
        }
    }
}
```
