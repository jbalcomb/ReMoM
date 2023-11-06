

RLE
Run-Length Encoding
Run-Length Encoded
Encode
Decode

Draw()
Clipped_Draw()
Draw_Picture()


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






