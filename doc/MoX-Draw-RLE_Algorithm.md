


CPO:    Copy Packet Operation
DPO:    Decode Packet Operation
CP:     CoPy
DC:     DeCode

packet_byte_count
data_count

skip_x
skip_y

¿ `skip_count`  `data_count` ?



Eh?


column-wise

¿ C code vs. Assebly ?
~ impossible to match the code logic/structure in C-style code
maybe just use Assembly, probably just use C goto


elsewhere, "... one packet per scanline ..."




¿ transparency ?
¿ remap colors ?
¿ clipped ?



Packet Operation Types
0xFF    skip
0x80    decode
0x00    copy



Is the code the same for Draw() as for Clipped_Draw() when skip_y == 0?



packet_byte_count
data_count  ~== data_count
delta_byte_count     ~== skip_count
Usage?




looks like "data_count" and "skip_count" are regularly used in MoO2 for the the two words of the sequence header
also, "skip_x" and "skip_y"
odd mix of x1,y1 vs. x_start,y_start
...frame,buffer,screen...
line_add
line_skip
buffer_add
screen_add
skip_add
clipped_count
actual_width, actual_height
screen_line_add
screen_line_start
screen_line_end
screen_start, screen_pos
unsigned integer (4 bytes) data



### Clipped_Draw()

skip_x
¿ one "packet" per column ?
x axis and column-wise, so just skip packets (being careful about 'skip' packet ops)

skip_y
`copy`
if skip_y != 0
loop through sequences until skip_y is consumed, either by sequence skip_count or sequence byte_count

looks like four paths
but one is just an alternate path to the end of the packet, because skip_y was large enough to cause the whole packet to be skippped


Meh. Haven't been able to figure out how to make it work with proper C-style code - loops and whatnot.
So, Go-To's ...
Per Packet Operation - Copy or Decode


In the disassembly, ...

@@Nay_Negative
@@copy_packet
@@cpo  (as in, "Copy Packet Operation")
@@copy


`@@copy_sequence_pre1`  or  `@@copy_sequence_pre2`  or  `copy_sequence_pre3`
...
@@copy_loop_height
...
@@copy_sequence_start

The *normal* path ...
    @@copy_consume_skip_y
        @@copy_sequence_pre1
            @@copy_sequence_start
            @@copy_sequence_start_loop
                @@copy_loop_height

First alternate path ...
    @@copy_consume_skip_y
        @@NayNegative_YaySkipY
            @@copy_sequence_pre2
            @@copy_sequence_start_loop
                @@copy_loop_height

Second alternate path ...
    @@copy_consume_skip_y
        @@NayNegative_YaySkipY
            loops over sequences 
            ¿ handle partial sequence skip_count ?


... skip_y vs. sequence skip_count ...
    loops back to if(line_skip_count != 0)



            signed integer (4 bytes) packet_end
            signed integer (2 bytes) data_count
            signed integer (2 bytes) skip_count
            signed integer (2 bytes) store_type

BX is CS031_height  (passed in as actual_height, probably "cheight", (y2 - start_y + 1) || (screen_window_y2 - start_y + 1))

if height is 0
    no check, no change
then, height - (tmp)skip_count
then, (height - 1) and if (height < 0) ... @@copy_sequence_start
so, does nothing, but still loops through width
¿ assert tmp_skip_count < 200 ?


¿ Order of Operation ?

Loop Width
Loop Packet
Loop Height
Loop Sequence

Copy - Packet Operation
if skip_y == 0
    then normal  ¿ while(packet_byte_count) { ... packet_byte_count -= (2 + data_count); }
        get line height
        get sequence skip count

else if skip_count > skip_y
    them almost normal
        get line height

height -= skip_count || -= (skip_y - skip_count)

```
skip_y == 0  >|   |<  (skip_y - skip_count) < 0
              |_ _|
                |
```

core copy / loop lines  (FliLib: "linelp")

```c
copy_loop_height:
do {
    if (--ylen >= 0) {
        *q = *data++;
        q += pitch;
    } else {
        data += len_run;
        break;
    }
} while (--len_run);
```

copy_loop_height:
    tmp_height--;
    if(tmp_height < 0) { goto copy_end_loop_height; }
ULC_Copy_Loop:
    /* ~ while(count--) {*dst = *src++; dst += SCREEN_WIDTH; } */
    *bbuff = *frame_data++;
    bbuff += SCREEN_WIDTH;
    data_count--;
    if (data_count != 0) { goto ULC_Copy_Loop; }

```c
copy_loop_height:
for(;;)
{
    tmp_height--;
    if(tmp_height < 0) { frame_data += data_count; break; }
    *bbuff = *frame_data++;
    bbuff += SCREEN_WIDTH;
    data_count--;
    if(data_count < 0) { break; }
}
```


***consume skip_y***
if(skip_y == 0) { ... } else
skip_count = *(frame_data + 1)
line_skip_count -= skip_count
if(line_skip_count > 0)
data_count = *frame_data++
tmp_skip_count -= data_count;
if(tmp_skip_count < 0) { ... } else


if skip_y != 0
    loop over sequences until delta or size consumes skip_y
        skip_y -= delta
        skip_y -= size
...
...
...










            line_skip_count = CS031_skip_y;

            if(line_skip_count == 0)
            {
                /* ¿ same as Draw() ? */
                do {
                    tmp_height = CS031_height;
                    skip_count = *(frame_data + 1);
                    // same from here
                    tmp_height -= skip_count;
                    bbuff += (skip_count * SCREEN_WIDTH);
                    data_count = *frame_data++;
                    frame_data++;
                    packet_byte_count -= data_count + 2;
                    while ((tmp_height > 0) && (data_count > 0))
                    {
                        tmp_height--;
                        *bbuff = *frame_data++;
                        bbuff += SCREEN_WIDTH;
                        data_count--;
                    }
                    frame_data += data_count;
                } while (packet_byte_count >= 1);
            }
            else
            {
                skip_count = *(frame_data + 1);

                line_skip_count -= skip_count;

                if (line_skip_count <= 0)
                {
                    tmp_skip_count = -(line_skip_count);
                    tmp_height = CS031_height;
                    // same from here
                    tmp_height -= skip_count;
                    bbuff += (skip_count * SCREEN_WIDTH);
                    data_count = *frame_data++;
                    frame_data++;
                    packet_byte_count -= data_count + 2;
                    while ((tmp_height > 0) && (data_count > 0))
                    {
                        tmp_height--;
                        *bbuff = *frame_data++;
                        bbuff += SCREEN_WIDTH;
                        data_count--;
                    }
                    frame_data += data_count;


                }
                else
                {
                }




                tmp_height -= tmp_skip_count;  // actual skip_count or residual skip_y

                    bbuff += (tmp_skip_count * SCREEN_WIDTH);  // ; add skip count to screen position

                    data_count = *frame_data++;  // size_count - Count of Bytes to Copy

                    frame_data++;  // ; skip the byte that was peek'd above

                    packet_byte_count -= data_count + 2;  // A Packet can have multiples Sequences, so deduct the 2-byte *header* and the size of the Sequence

                    while ((tmp_height > 0) && (data_count > 0))
                    {
                        tmp_height--;
                        *bbuff = *frame_data++;
                        bbuff += SCREEN_WIDTH;  // wrap around to the next pixel down
                        data_count--;
                    }
                    frame_data += data_count;

                } while (packet_byte_count >= 1);



#### Magic Screen - Staves

Magic_Screen_Load_Pictures()
    mana_staff_empty_seg = LBX_Reload_Next("MAGIC", 7, _screen_seg);

Magic_Screen_Draw()
    Set_Window(0, 79, SCREEN_XMAX, (152 - mana_stave_pct_pos));
    Clipped_Draw(29, 83, mana_staff_empty_seg);

MAGIC.LBX, 007  STAVES     mana staff empty

0000ECF1
0000EE3E

0C 00   12  width
46 00   70  height
00 00       current_frame
01 00       frame_count
00 00       loop_frame
00          emm_handle_number
00          emm_logical_page_number
00 00       emm_logical_page_offset
00 00       palette_header_offset

00 00 

1A 00 00 00     frame start
4D 01 00 00     frame end

01              full store flag

FF 
00 06 04 05 B8 B5 B5 B8 
00 0A 08 03 B8 B4 B2 B2 B2 B3 B5 B8 
00 14 09 02 B8 B4 B0 B8 B6 B6 B8 B4 B5 01 02 B8 01 01 B8 01 01 B8 
80 35 33 01 B8 B5 B0 B8 B3 E1 A2 B3 B8 E1 B5 B8 B6 B8 B6 B8 B6 C4 B6 E4 C4 B6 C4 11 C4 11 C4 B6 E4 C4 B6 E4 C4 B6 E4 C4 B6 E4 C4 B6 E4 C4 B6 C4 11 C4 11 C4 B6 B8 B6 
80 35 33 01 B5 B2 B8 B3 E1 A2 B2 E1 B3 B8 B3 B6 B5 B6 B5 B6 B5 19 B5 E4 19 B5 19 C4 19 C4 19 B5 E4 19 B5 E4 19 B5 E4 19 B5 E4 19 B5 E4 19 B5 19 C4 19 C4 19 B5 B6 B5 
80 35 33 01 B5 B3 B8 B3 A2 B1 B3 A0 B3 B8 B2 B5 B3 B5 B3 B5 B3 1C B0 E4 1C B0 1C 19 1C 19 1C B0 E4 1C B0 E4 1C B0 E4 1C B0 E4 1C B0 E4 1C B0 1C 19 1C 19 1C B0 B5 B0 
80 35 33 01 B8 B4 B5 B8 B3 A2 B2 B3 B8 B4 B2 B6 B0 B6 B0 B6 B0 19 B5 E4 19 B5 19 C4 19 C4 19 B5 E4 19 B5 E4 19 B5 E4 19 B5 E4 19 B5 E4 19 B5 19 C4 19 C4 19 B5 B6 B5 
00 14 09 02 B8 B5 B5 B8 B6 B6 B8 B4 B0 01 02 B6 01 01 B6 01 01 B6 
00 0A 08 03 B8 B5 B4 B5 B5 B2 B0 B8 
00 06 04 05 B8 B3 B2 B8 
FF

Clipped_Draw_Frame(start_x, start_y, actual_width, actual_height, skip_x, skip_y, frame_data);
start_x         29
start_y         83
actual_width    12
actual_height   70
skip_x           0
skip_y           0

x1              29
y1              83
width           12
height          70
skip_x           0
skip_y           0

Packet Operation - Skip
packet_op          255

Packet Operation - Copy
packet_op            0
packet_byte_count    6
line_skip_count      0
line_count          70
line_skip_count      5
line_count          65
data_count           4
(packet_byte_count -= data_count + 2) == 0
(line_count--) == 64
(data_count--) == 3
(line_count--) == 63
(data_count--) == 2
(line_count--) == 62
(data_count--) == 1
(line_count--) == 61
(data_count--) == 0
