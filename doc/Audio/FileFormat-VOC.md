
.VOC
Creative Labs
"Creative Voice File"



As used by Audio Interface Library (AIL), ...
AIL214 does not check for block types 3, 5, 8, or 9

As seen in MoM, ...
    codec ids
        0x00  8 bits unsigned PCM




FFmpeg
sox
Exult
1oom
chocolate-doom






Main header
bytes 0-18   Identifier string containing: Creative Voice File
byte  19     0x1A (EOF). This is belived to be used to abort printing of file
bytes 20-21  Total size of this main header (usually 0x001A)
bytes 22-23  Version number, calculated as (major<<8)|minor
               major is usually 0x01
               minor is usually 0x0A or 0x14
bytes 24-25  Validity check. This must be equal to ~version + 0x1234
Data blocks
All the different data blocks begin with a common header:

byte  0      block type
bytes 1-3    block size (NOT including this common header)
The data following this common block header depends on the block type.

Block type 0x00: Terminator
Block type 0x01: Sound data
Block type 0x02: Sound data continuation
Block type 0x03: Silence
Block type 0x04: Marker
Block type 0x05: Text
Block type 0x06: Repeat start
Block type 0x07: Repeat end
Block type 0x08: Extra info
Block type 0x09: Sound data (New format)

Supported codec ids
  0x00  8 bits unsigned PCM
  0x01  4 bits to 8 bits Creative ADPCM
  0x02  3 bits to 8 bits Creative ADPCM (AKA 2.6 bits)
  0x03  2 bits to 8 bits Creative ADPCM
  0x04  16 bits signed PCM
  0x06  alaw
  0x07  ulaw
0x0200  4 bits to 16 bits Creative ADPCM (only valid in block type 0x09)



## Block type 0x00: Terminator
This is a special block type as it's common header don't contain any size field. It indicate the end of the file. It is not mandatory (you can reach EOF without encountering this block type).

## Block type 0x01: Sound data
byte  0      frequency divisor
byte  1      codec id
bytes 2..n   the audio data
The sample rate is defined as
    1000000/(256 - frequency_divisor)

## Block type 0x02: Sound data continuation
bytes 2..n    the audio data
This block uses the same codec parameters as the previous "Sound data" block.

## Block type 0x03: Silence
bytes 0-1    length of the silence - 1 (unit is sample)
byte  2      frequency divisor
The sample rate is defined as

1000000/(256 - frequency_divisor)
Block type 0x04: Marker
bytes 0-1    the mark value
This can be used by the software to synchronize the sound with an animation.

## Block type 0x05: Text
bytes 0..n   zero terminated string

## Block type 0x06: Repeat start
bytes 0-1    repeat count - 1
The sound data following this block and up to the next Repeat end block is repeated count+1 times. When count == 0xFFFF this means endless repetitions.

## Block type 0x07: Repeat end
Empty block which terminate a repeat section.

## Block type 0x08: Extra info
bytes 0-1    frequency divisor
byte  2      codec id
byte  3      channels number - 1
The sample rate is defined as

256000000/(nb_channels * (65536 - frequency_divisor))
This block must be followed by a "Sound data" block, and it supercedes its codec parameters.

## Block type 0x09: Sound data (New format)
This block type is probably only valid in version 1.20 (0x0114) or greater files.

bytes 0-3    sample rate
byte  4      bits per sample
byte  5      channels number
bytes 6-7    codec_id
bytes 8-11   reserved
bytes 12..n  the audio data







C:\STU\devel\AIL214\A214_D2\DOC\ASCIIDOC\NOTES.TXT

* For a given sampling frequency in hertz rate, the sampling_rate value
  for use with a sound_buff structure may be determined by 
          
            sampling_rate = 256 - (1000000 / rate)

  During stereo playback, the frequency rate must be doubled to
  compensate for the interleaving of left/right sample information.

;f in Hz. = 1E6 / (256 - SB_Rate)
call ul_divide C,4240h,0fh,ax,0
0xF4240  1000000d
