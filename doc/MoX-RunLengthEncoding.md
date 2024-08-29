

RLE
Run-Length Encoding
Run-Length Encoded
Encode
Decode

Draw()
Clipped_Draw()
Draw_Picture()
¿ Clipped_Print() ?



MoO2

Add_Picture_To_Bitmap()

Module: animate
    Draw_Animated_Sprite_On_Page()
    Remap_Draw_Animated_Sprite()
    Remap_Clipped_Draw_Animated_Sprite()

Module: clipped



    function (0 bytes) Remap_Clipped_Draw_Animated_Sprite
    Address: 01:0012BC0B
        Num params: 3
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) x1
            signed integer (2 bytes) y1
            pointer (4 bytes) frame_data
            signed integer (4 bytes) screen_pos
            signed integer (4 bytes) y
            signed integer (4 bytes) screen_start
            signed integer (4 bytes) buffer_pos
            signed integer (4 bytes) buffer_pos_word
            signed integer (4 bytes) packet_end
            signed integer (2 bytes) height
            signed integer (2 bytes) data_count
            signed integer (2 bytes) skip_count
            signed integer (2 bytes) store_type
            signed integer (2 bytes) width
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            signed integer (2 bytes) skip_x
            signed integer (2 bytes) start_x
            signed integer (2 bytes) skip_y
            signed integer (2 bytes) start_y
            signed integer (2 bytes) actual_width
            signed integer (2 bytes) actual_height
            signed integer (2 bytes) skip_add
            signed integer (2 bytes) clipped_count
            pointer (4 bytes) frame_data_word
            signed integer (4 bytes) buffer_add
            signed integer (4 bytes) screen_line_add
            signed integer (4 bytes) screen_line_start
            signed integer (4 bytes) screen_line_end






Where did I put the source code for Animator/FLIC Lib?

C:\STU\develedu\

C:\STU\develedu\FLILIB3\FLISRC3\UNBRUN.ASM
low level routine to uncompress 1st frame of a FLI
unbrun  uncompress byte run
cbuf    pointer to
screen  pointer to
linect  line count
linelp  line loop
ulcloop 
ucopy   
endulcloop  
ulcout      ¿ outro ?
ulcrun

¿ "line count" ~== "height" ?




Clipped_Draw_Frame()

In the Dasm, ...
    VGA - SC_INDEX, SC_MAPMASK
    setup ES:DI and DS:SI
    set VGA write-mask


if skip_x != 0
    skip src bytes


@Clipped_Draw_Frame+74
`push di`
start offset of the scan line
screen_pos


if _SI_ >= 64512
    add 4032 segments to _DS_
    sub 64512 from _SI_
    4032 segments * 16 = 64512 bytes


lodsb                                   ; Frame Byte #1: Op/Count
cmp     al, 0FFh                        ; Op = 255 = Skip Column

@@NextColumn1:                            ; GPBB:  ;retrieve the dest start offset
pop     _DI_screen_pos                    ;
dec     bx                                ; ColumnCount--
                                          ; GPBB:  ;count down scan lines
jz      short @@Done


mov     dh, al                          ; packet_op
lodsb                                   ; Frame Byte #2: packet byte count
mov     dl, al                          ; dx = encoded column data length in bytes (15 bits, the sign bit indicates whether there are repeats)
cmp     dh, 0
jns     short @@Nay_Negative

load another byte - the packet_byte count  ¿ MoO2 "packet_end" ?
test the sign-bit / if the packet_op is negative

if not sign-bit / negative, op-type is 'copy'  ¿ MoO2 "store_type" ?

set tmp_skip_y to CS031_skip_y
if skip_y is 0, reset tmp_skip_y to CS031_height
    ...this would be the same path as in FLIC_Draw_Frame()
    peek Packet Byte #2: skip_count
    subtract skip_count from tmp_skip_y
    add skip count to screen position
    load Packet Byte #1: size_count
    set counter to packet size_count, for loop
    increment pointer to frame data, to skip the a byte that was peek'd above
    packet_byte_count -= sequence_byte_count + 2;
        ...in the Dasm, _DX_ is packet_byte_count, _CX_ is sequence_byte_count
else
    tmp_skip_y = skip_y - skip_count
    how much we are going to skip is reduced by what the frame data already wants us to skip?
        presumably, we'd get to skipping how we would normally skip, but then also skip more if skip_y is greater than skip_count




packet_byte_count -= sequence_byte_count + 2;  // A Packet can have multiples Sequences, so deduct the 2-byte *header* and the size of the Sequence






