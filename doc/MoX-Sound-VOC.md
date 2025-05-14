


convert unsigned, 8-bit, 1-channel, integer PCM to signed, 16-bit, 2-channel, integer PCM

SDL2
Audio format flags
#define AUDIO_U8        0x0008  /**< Unsigned 8-bit samples */
#define AUDIO_S16LSB    0x8010  /**< Signed 16-bit samples */
...assumed to Integer



VOC offers
    sample rate
    ...1000000 / (256 - sample rate)
WAV requires
    sample rate
    sample frame size = (bits per sample * channel count)
    bits per sample
    channel count



preordaned
MoM's VOCs are 8-bit, unsigned, mono (1 channel), PCM  (~== Wave: Integer PCM at 8 bits per sample (format tag 1))
therefore
1 byte per sample
1 byte per sample frame  ((8 / 8) * 1) = 1


RIFF Chunk
Format Chunk
Data Chunk

RIFF Chunk  (12 bytes)
    Chunk ID            4       0x52 0x49 0x46 0x46 (i.e. "RIFF")
    Chunk Body Size     4       32-bit unsigned integer  (the size of the entire Wave file except for the 8-byte RIFF chunk header)
    RIFF Form Type      4       0x57 0x41 0x56 0x45 (i.e. "WAVE")
Format Chunk  (24 bytes)
    Chunk ID                    4       0x66 0x6d 0x74 0x20 (i.e. "fmt ")
    Chunk Body Size             4       32-bit unsigned integer
    Format Tag                  2       16-bit unsigned integer
    Number of Channels          2       16-bit unsigned integer     (1: mono, 2: stereo, ..., 6: 5.1 surround sound)
    Samples per second          4       32-bit unsigned integer     (sample rate – number of sample frames that occur each second)
    Average Bytes per Second    4       32-bit unsigned integer     (number of bytes required for one second of audio data - sample rate * sample frame size)
    Block align                 2       16-bit unsigned integer     (sample frame size - size in bytes of a unit of sample data) (e.g., 16-bit, 2-channel (16÷8)×2=4)
    Bits per sample             2       16-bit unsigned integer     (the size in bits of a single sample (not a sample frame, but a single sample))

Data Chunk
The Data Chunk
Field               Bytes   Description
Chunk ID            4       0x64 0x61 0x74 0x61 (i.e. "data")
Chunk Body Size     4       32-bit unsigned integer
Sample Data         Various It depends on the format tag
The layout for the data chunk is simpler than the format chunk: the normal 8-byte chunk header, followed by nothing but raw sample data.
The sample data can be stored in a number of formats, which will be indicated by the format tag field in the format chunk.
The following sections describe several formats that sample data in the data chunk can be stored as.
Integer PCM Data Chunk
Format tag: 1

This is the most common format, and consists of raw PCM samples as integers. The bits per sample field will indicate the range of the samples:

Bits per sample  Min Value       Mid Value  Max Value
8                0               128        255
16               -32,768         0          32,767
24               -8,388,608      0          8,388,607
32               -2,147,483,648  0          2,147,483,647
Samples in a multi-channel PCM wave file are interleaved.
That is, in a stereo file, 
  one sample for the left channel will be followed by one sample for the right channel,
  followed by another sample for the left channel, then right channel, and so forth.
One set of interleaved samples is called a sample frame (also called a block). A sample frame will contain one sample for each channel.
In a monophonic file, a sample frame will consist of 1 sample.
In a stereo file, a sample frame has 2 samples (one for the left channel, one for the right channel).
In a 5-channel file, a sample frame has 5 samples.
The block align field in the format chunk gives the size in bytes of each sample frame.
This can be useful when seeking to a particular sample frame in the file.
For example, for a 2 channel file with 16-bit PCM samples, the sample data would look like this:
+===================================+ +===================================+
|           Sample Frame 1          | |           Sample Frame 2          |
| +==============+ +==============+ | | +==============+ +==============+ |
| | Left Channel | | Left Channel | | | | Left Channel | | Left Channel | |
| |   LSB MSB    | |   LSB MSB    | | | |   LSB MSB    | |   LSB MSB    | |
| +==============+ +==============+ | | +==============+ +==============+ |
+===================================+ +===================================+
LSB means "least significant byte", and MSB means "most significant byte."


https://www.joelstrait.com/digital_audio_primer/
Frequency is expressed in hertz, or cycles per second.
Analog Signals vs. Digital Signals
So our humble sine wave is wafting through the air, and we want our computer to capture it and store it for later. To do this, we need to convert the sound wave from an analog form to a digital form. This process is called sampling, and is necessary because computers can only store data digitally.
An analog signal is continuous. Notice how the sine waves in the previous section are smooth. There are no gaps anywhere – if you were to zoom in, and keep zooming in into infinity, the line would always be smooth. If you have two points in an analog signal, there are an infinite number of points between them.
In contrast, a digital signal consists of a series of instantaneous “snapshots” of the amplitude of the signal over time. Each snapshot is called a sample.
A Sample of Sampling
“Sampling a signal” means to record instantaneous amplitudes (i.e. y-values) at a regular time interval, and put them in a list.
Because the same amount of time is elapsed between each sample, the samples are all the same distance apart on the x-axis. (The x-axis denotes some arbitrary amount of time).
Sample Rate
An important property of digital audio is the sample rate. This represents how many times a signal is sampled per second, and is expressed in Hertz. A common sample rate is 44,100Hz, which means a sample is taken every 1/44,100th of a second. Or put differently, it means that 44,100 samples are used to create 1 second of sound.
Bits Per Sample / Sample Depth
While the sample rate determines how accurately we can capture frequencies with digital audio, the sample depth determines how accurately we can capture amplitudes. 
The process of converting a sampled analog amplitude to one of the possible integer values is called quantization. The number of bits used to encode each sample is called the bits per sample.








Options?
convert VOC to WAV
decode VOC as expected by SDL Mixer
FFmpeg
sox



load LBX entry
convert LBX entry VOC data to WAV data
load WAV data in SDL Mixer
play WAV data in SDL Mixer



hw_audio_sfx_init() |-> hw_audio_sfx_init_do() |-> hw_audio_sfx_init_alloc(); fmt_sfx_convert(); sfxtbl[].chunk = Mix_LoadWAV_RW(SDL_RWFromMem())

hw_audio_sfx_init()
    hw_audio_sfx_init_do()
        hw_audio_sfx_init_alloc()
        fmt_sfx_convert(data_in, len_in, &data, &len, NULL, audio_rate, true)
        sfxtbl[sfx_index].chunk = Mix_LoadWAV_RW(SDL_RWFromMem(data, len), 0);
hw_audio_sfx_play(sfxi)
    |-> Mix_PlayChannel(0, sfxtbl[sfx_index].chunk, 0);

e.g., ...
INTRO.C
Draw_Logos()
    ...
    digi_sound_buffer = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroT1, _screen_seg);
    sound_buffer_size = lbxload_entry_length;
    ...
    if((itr_scenes == 4) && (itr_frames == 0))
        if(magic_set.sound_effects == ST_TRUE)
            // TODO  Play_Sound__WIP(digi_sound_buffer);
            sdl2_Play_Sound(digi_sound_buffer, sound_buffer_size);





## ¿ work backwards from the call(s) to Mix_PlayChannel() ?

Mix_PlayChannel()
    Play an audio chunk on a specific channel.

ui_sound_play_sfx(sfx_index)
    hw_audio_sfx_play(sfx_index)
        Mix_PlayChannel(0, sfxtbl[sfx_index].chunk, 0);

uidefs.h
#define NUM_SOUNDS  0x29
#define NUM_MUSICS  0x28
uiintro.c
#define SFX_ID_1    (NUM_SOUNDS + 0)
#define SFX_ID_3    (NUM_SOUNDS + 1)
#define SFX_ID_5    (NUM_SOUNDS + 2)
ui_play_intro()
    hw_audio_sfx_batch_start(SFX_ID_5)
    intro_sfx5 = lbxfile_item_get_with_len(LBXFILE_INTROSND, 5, &len)
    hw_audio_sfx_init(SFX_ID_5, intro_sfx5, len)
    intro_sfx3 = lbxfile_item_get_with_len(LBXFILE_INTROSND, 3, &len)
    hw_audio_sfx_init(SFX_ID_3, intro_sfx3, len)
    intro_sfx1 = lbxfile_item_get_with_len(LBXFILE_INTROSND, 1, &len)
    hw_audio_sfx_init(SFX_ID_1, intro_sfx1, len)
    hw_audio_sfx_batch_end()
    ...
    ...
    ...
    hw_audio_sfx_play(SFX_ID_5)

¿ hw_audio_sfx_batch_start() and hw_audio_sfx_batch_end() only do anything if using threads ?
but, ...
    hw_audio_sfx_batch_start() calls hw_audio_sfx_init_alloc()
    hw_audio_sfx_init_alloc() reallocates sfxtbl[]
also, ...
    hw_audio_sfx_init_alloc() is called by hw_audio_sfx_init_do()
    hw_audio_sfx_init_do() is called by hw_audio_sfx_init()

hw_audio_sfx_init(SFX_ID_5, intro_sfx5, len)
    hw_audio_sfx_init_do()
        hw_audio_sfx_init_alloc()

## sfxtbl[]
¿ gets used reguardless of threaded ?

int hw_audio_sfx_init(int sfx_index, const uint8_t *data_in, uint32_t len_in)
    if(hw_audio_sfx_init_do(sfx_index, data_in, len_in))

static int hw_audio_sfx_init_do(int sfx_index, const uint8_t *data_in, uint32_t len_in)


sfxtbl[sfx_index].chunk = Mix_LoadWAV_RW(SDL_RWFromMem(data, len), 0);






## Convert_VOC_To_WAV

...fmt_sfx_convert()...fmt_sfx_detect()...fmt_sfx_convert_voc()...

extract the code that builds what gets passed to MIX_PlayChannel()
remove fmt_sfx_detect()
merge fmt_sfx_convert_voc() into fmt_sfx_convert()




## fmt_sfx_convert()
bool fmt_sfx_convert(const uint8_t *data_in, uint32_t len_in, uint8_t **data_out_ptr, uint32_t *len_out_ptr, sfx_type_t *type_out, int audiorate, bool add_wav_header)
    type = fmt_sfx_detect(data_in, len_in)
    switch (type)
    case SFX_TYPE_VOC:
    conv_res = fmt_sfx_convert_voc(data_in, len_in);

What is `type_out`?  ...LBXVOC, VOC, or WAV, where "LBXVOC" is just a VOC with Simtex's 16-byte LBX entry header
When would you not want `add_wav_header`?
    only ever not true in hwalleg_audio.c
Why two sets of data_in, len_in, data_out_ptr, len_out_ptr?
    VOC in, WAV out

hw_audio_sfx_init(int sfx_index, const uint8_t *data_in, uint32_t len_in)
    if (hw_audio_sfx_init_do(sfx_index, data_in, len_in)) {

sfxtbl[sfx_index].chunk is the sound buffer
hw_audio_sfx_init_alloc() reallocs and resets all of sfxtbl
hw_audio_sfx_init() |-> hw_audio_sfx_init_do() |-> hw_audio_sfx_init_alloc(); fmt_sfx_convert(); sfxtbl[].chunk = Mix_LoadWAV_RW(SDL_RWFromMem())
hw_audio_sfx_init() is called on every SFX/VOC LBX entry

ui_data.sfx[i] = lbxfile_item_get_with_len(LBXFILE_SOUNDFX, i, &len);
hw_audio_sfx_init(i, ui_data.sfx[i], len);
So, ...
data_in and len_in come from the special LBX Load function
Then, ...
    sets the data pointer passed the LBX VOC header and subtracts the LBX VOC header size from length
    memcmp()'s the VOC file header  "Creative Voice File..."
    sets the data pointer passed the VOC file header and subtracts the VOC file header size from length
    sets a rover pointer for the data, but only use of data pointer after that is for debugging purposes
    loops over length, switches on "block type"


Exult only checks for VOC block types 0, 1, 2, and 3
    COUT("Terminator");
    COUT("Sound data");
    COUT("Sound continues");
    COUT("Silence");




AIL214 only processes block types 0, 1, 2, 4, 6, and 7
NOTE: no 3 or 5
C:\STU\devel\AIL214\A214_D3\AIL.ASM

Procedures/Function?
xfer_chunk
block_type
next_block
process_block
CB_play_VOC
DP_play_VOC_file

next_block
    sets `block_ptr` to start of next block, using a roving pointer to get the current blocks `blk len`
block_type
    gets a 1-byte, unsigned at block_ptr

block_ptr       dd ?
    DP_play_VOC_file PROC H,File:FAR PTR,Block
                    LOCAL block_file:DWORD
                lds si,block_file
                mov ax,[si+14h]         ;get offset of data block
                ADD_PTR ax,0,ds,si
                mov WORD PTR block_ptr,si
                mov WORD PTR block_ptr+2,ds
                ...
                                        ;return w/block_ptr -> 1st file block



;****************************************************************************
process_block   PROC                    ;Process current block in voice data
                USES ds,si,di           ;(May be called from IRQ handler)

__do_block:     call block_type
                cmp ax,0                ;terminator?
                je __terminate
                cmp ax,1                ;new voice block?
                je __new_voice
                cmp ax,2                ;continued voice block?
                je __cont_voice
                cmp ax,4                ;marker (end of data?)
                je __terminate
                cmp ax,6                ;beginning of repeat loop?
                je __rept_loop
                cmp ax,7                ;end of repeat loop?
                je __end_loop
                jmp __skip_block        ;else unrecognized block type, skip it






## fmt_sfx_detect()
checks the first four bytes
#define HDRID_LBXVOC    0xafde0200/*0xdeaf 2*/
#define HDRID_VOC       0x43726561/*Crea*/
#define HDRID_WAV       0x52494646/*RIFF*/


## fmt_sfx_convert_voc()



¿ audiorate ?
¿ libsamplerate ?

fmt_sfx_resample_simple()
fmt_sfx_resample_libsamplerate()


fmt_sfx.c
fmt_sfx_convert()
    type = fmt_sfx_detect()
    switch(type)
        fmt_sfx_convert_voc()

fmt_mus.c



uisound.c
ui_sound_play_sfx()
    |-> hw_audio_sfx_play()
ui_sound_stop_sfx()
    |-> hw_audio_sfx_stop()
ui_sound_play_music()
    |-> hw_audio_music_stop()
    |-> hw_audio_music_play()
ui_sound_stop_music()
    |-> hw_audio_music_stop()

hw_audio_sfx_play()
hw_audio_music_play()
~== Play_Sound()






static int hw_audio_sfx_init_do(int sfx_index, const uint8_t *data_in, uint32_t len_in)
    uint8_t *data = NULL;
    uint32_t len = 0;
    hw_audio_sfx_init_alloc(sfx_index);
    if(sfxtbl[sfx_index].chunk)
        hw_audio_sfx_release(sfx_index);
    if(fmt_sfx_convert(data_in, len_in, &data, &len, NULL, audio_rate, true))
        sfxtbl[sfx_index].chunk = Mix_LoadWAV_RW(SDL_RWFromMem(data, len), 0);
        lib_free(data);
...hw_audio_sfx_init_alloc() reallocs and resets all of sfxtbl...



            ui_data.sfx[i] = lbxfile_item_get_with_len(LBXFILE_SOUNDFX, i, &len);
            hw_audio_sfx_init(i, ui_data.sfx[i], len);
ui_data.sfx[i] = lbxfile_item_get_with_len(LBXFILE_SOUNDFX, i, &len);
...
digi_sound_buffer = LBX_Reload_Next(soundfx_lbx_file__MGC_ovr058, SFX_IntroT1, _screen_seg);
sound_buffer_size = lbxload_entry_length;
...


ui_sound_play_sfx_24()
    ui_sound_play_sfx(0x24)
ui_sound_play_sfx_06()
    ui_sound_play_sfx(0x06)

ui_sound_play_sfx(sfxi)
    hw_audio_sfx_play(sfxi)
        Mix_PlayChannel(0, sfxtbl[sfx_index].chunk, 0);



orion.exe
init_vortex_etc()
    lbxsound_24_seg = lbx_load_item_opt000(aSoundfx_lbx, 36)
    lbxsound_06_seg = lbx_load_item_opt000(aSoundfx_lbx, 6)
sound_play_24()
    Play_Sound__WIP(lbxsound_24_seg)
sound_play_06()
    Play_Sound__WIP(lbxsound_06_seg)

## sound_play_06()
XREF
    Negame_Screen+1E6
    ui_load_game+1A0
    ui_load_game+1E6
    j_sound_play_06
        newgame_racebanner

...something like the sounds for OK and Cancel
...not quite Left and Right Click
...why in with New/Load Game code?






¿ converts VOC to WAV ?

static int hw_audio_sfx_init_do(int sfx_index, const uint8_t *data_in, uint32_t len_in)
    uint8_t *data = NULL;
    uint32_t len = 0;
    hw_audio_sfx_init_alloc(sfx_index);
    if(fmt_sfx_convert(data_in, len_in, &data, &len, NULL, audio_rate, true))
...

int hw_audio_sfx_init(int sfx_index, const uint8_t *data_in, uint32_t len_in)
    if (!sfx_process.started) {
        if (hw_audio_sfx_init_do(sfx_index, data_in, len_in)) {


ui_classic.c
ui_late_init()
    res = hw_audio_sfx_batch_start(NUM_SOUNDS);
    for(int ti = 0; ti < TBLLEN(sounds); ++ti)
        ui_data.sfx[i] = lbxfile_item_get_with_len(LBXFILE_SOUNDFX, i, &len);
        hw_audio_sfx_init(i, ui_data.sfx[i], len);
    res = hw_audio_sfx_batch_end();
...


## hw_audio_sfx_batch_start() && hw_audio_sfx_batch_end()

¿ only do anything if using threads ?


ui_data.sfx[] is an array of pointers to the loaded LBX entries?
hw_audio_sfx_init(i, ui_data.sfx[i], len);
int hw_audio_sfx_init(int sfx_index, const uint8_t *data_in, uint32_t len_in)
    hw_audio_sfx_init_do(sfx_index, data_in, len_in)
        sfx_process.tbl[sfx_process.num].sfxi = sfx_index;
        sfx_process.tbl[sfx_process.num].data = data_in;
        sfx_process.tbl[sfx_process.num].len = len_in;
#else /* !USE_SFX_INIT_THREAD */
    hw_audio_sfx_init_do(sfx_index, data_in, len_in)


int hw_audio_sfx_batch_end(void)
    sfx_process_thread = HWSDLX_CreateThread(hw_audio_sfx_init_thread);
#define HWSDLX_CreateThread(_func_) SDL_CreateThread(_func_, "SFX init", 0)



struct ui_data_s {
    uint32_t seed;
    uint8_t *sfx[NUM_SOUNDS];
    uint8_t *mus;
    int music_i;



