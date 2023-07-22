
# Proof of Concept (PoC)

1. 320x200 VGA
2. 320x200 VGA |-> 640x400 XBGR
3. 640x400 XBGR



! convert 320x200 VGA to 640x400 XBGR
convert 320x200 VGA to 320x200 XBGR
convert 320x200 XBGR to 640x400 XBGR



## 1. 320x200 VGA
native VBB and native drawing functions
Update_Window_Display() converts `320x200 VGA` to `320x200 XBGR` MS-DIB
StretchDIBits() stretches to `640x400`
Req'd: native VBB, width, height; MS-DIB VBB


## 2. 320x200 VGA |-> 640x400 XBGR
native VBB and native drawing functions
Update_Window_Display() converts `320x200 VGA` to `640x400 XBGR` MS-DIB
StretchDIBits() does not stretch


## 3. 640x400 XBGR
new drawing functions
//1oom: lbxgfx_draw_pixels_fmt0_scale()
¿ this would have to include fixing up all of the allocation - 4 * bytes ?
No? that's part of the next stage? where all the images are pre-upscaled/native x2 XBGR/RGBA?
