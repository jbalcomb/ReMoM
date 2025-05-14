



https://learn.microsoft.com/en-us/windows/win32/api/mmreg/ns-mmreg-waveformat
WAVEFORMAT structure (mmreg.h)
The WAVEFORMAT structure describes the format of waveform-audio data. Only format information common to all waveform-audio data formats is included in this structure. This structure has been superseded by the WAVEFORMATEX structure.
typedef struct waveformat_tag {
  WORD  wFormatTag;
  WORD  nChannels;
  DWORD nSamplesPerSec;
  DWORD nAvgBytesPerSec;
  WORD  nBlockAlign;
} WAVEFORMAT;
Members
wFormatTag
Format type. The following type is defined:
Name	Description
WAVE_FORMAT_PCM
Waveform-audio data is PCM.
nChannels
Number of channels in the waveform-audio data. Mono data uses one channel and stereo data uses two channels.
nSamplesPerSec
Sample rate, in samples per second.
nAvgBytesPerSec
Required average data transfer rate, in bytes per second. For example, 16-bit stereo at 44.1 kHz has an average data rate of 176,400 bytes per second (2 channels — 2 bytes per sample per channel — 44,100 samples per second).
nBlockAlign
Block alignment, in bytes. The block alignment is the minimum atomic unit of data. For PCM data, the block alignment is the number of bytes used by a single sample, including data for both channels if the data is stereo. For example, the block alignment for 16-bit stereo PCM is 4 bytes (2 channels — 2 bytes per sample).







https://wavefilegem.com/how_wave_files_work.html

Wave Files are RIFF Files
Back in the late 80s Electronic Arts came up with a general container file format that could be used to store different types of data – audio, graphics, etc. It was called IFF, for Interchange File Format. Microsoft then took this format, switched the byte order from big-endian to little-endian to better suit Intel processors, and dubbed it RIFF (Resource Interchange File Format). The RIFF format was then used for the *.wav file format.

RIFF Files Contain “Chunks”
Like an IFF file, a RIFF file is broken up into “chunks” of data. Each chunk starts with an 8-byte header containing a 4-byte identifier code, and a 4-byte size field. This is followed by the chunk body.

The identifier code, called a FourCC, is a sequence of 4 bytes. When each byte is interpreted as an 8-bit ASCII character, the sequence typically forms a human readable string. For example, 0x52 0x49 0x46 0x46 (i.e. "RIFF"), or 0x64 0x61 0x74 0x61 (i.e. "data"). Since this is a raw sequence of bytes, the characters are case-sensitive.

The size field indicates the size of the chunk’s body in bytes, as a 32-bit unsigned integer. The size should not include the 8-byte header. I.e., if a chunk consists of the 8-byte header followed by 1,000 bytes of data, the size field should indicate 1000, not 1008.

Important! If a chunk body has an odd number of bytes, it must be followed by a padding byte with value 0. In other words, a chunk must always occupy an even number of bytes in the file. The padding byte should not be counted in the chunk header’s size field. For example, if a chunk body is 17 bytes in size, the header’s size field should be set to 17, even though the chunk body occupies 18 bytes (17 bytes of data followed by the padding byte).




At minimum, the child chunks must include a format chunk ("fmt ") and a data chunk ("data"), and the format chunk must come before the data chunk. If the format tag in the format chunk is not 1 (see below), then there must also be a fact chunk ("fact").

RIFF Chunk
Format Chunk
Data Chunk

### The RIFF Chunk
    Like all chunks, the RIFF chunk starts with an FourCC ID code. In this case, it is "RIFF".
    Next is the size field, which is the size of the entire Wave file except for the 8-byte RIFF chunk header.
    The first 4 bytes following the header will identify the type of RIFF chunk.
    In the case of a Wave file, it will be "WAVE".
    Immediately following that will be the child chunks.
Field               Bytes   Description
Chunk ID            4       0x52 0x49 0x46 0x46 (i.e. "RIFF")
Chunk Body Size     4       32-bit unsigned integer
RIFF Form Type      4       0x57 0x41 0x56 0x45 (i.e. "WAVE")
Child Chunks    Variable    Variable
### The Format Chunk
    The format chunk describes the format that the samples in the data chunk are encoded in.
    The exact structure of the format chunk depends on the value of the format tag field.
    If the format tag is 1 (integer PCM), then the format chunk will only contain the fields above the dashed line in the diagram below.
    If it’s not 1, the chunk will also contain the fields after the dashed line.
Field                       Bytes   Description
Chunk ID                    4       0x66 0x6d 0x74 0x20 (i.e. "fmt ")
Chunk Body Size             4       32-bit unsigned integer
Format Tag                  2       16-bit unsigned integer
Number of Channels          2       16-bit unsigned integer
Samples per second          4       32-bit unsigned integer
Average Bytes per Second    4       32-bit unsigned integer
Block align                 2       16-bit unsigned integer
Bits per sample             2       16-bit unsigned integer
These fields are only present if format tag is not 1:
Extension Size              2           16-bit unsigned integer
Extra fields                Variable    It depends on the format tag

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








WAV file header
This is an example of a WAV file header (44 bytes). Data is stored in little-endian byte order.

[Master RIFF chunk]
   FileTypeBlocID  (4 bytes) : Identifier « RIFF »  (0x52, 0x49, 0x46, 0x46)
   FileSize        (4 bytes) : Overall file size minus 8 bytes
   FileFormatID    (4 bytes) : Format = « WAVE »  (0x57, 0x41, 0x56, 0x45)

[Chunk describing the data format]
   FormatBlocID    (4 bytes) : Identifier « fmt␣ »  (0x66, 0x6D, 0x74, 0x20)
   BlocSize        (4 bytes) : Chunk size minus 8 bytes, which is 16 bytes here  (0x10)
   AudioFormat     (2 bytes) : Audio format (1: PCM integer, 3: IEEE 754 float)
   NbrChannels     (2 bytes) : Number of channels
   Frequency       (4 bytes) : Sample rate (in hertz)
   BytePerSec      (4 bytes) : Number of bytes to read per second (Frequency * BytePerBloc).
   BytePerBloc     (2 bytes) : Number of bytes per block (NbrChannels * BitsPerSample / 8).
   BitsPerSample   (2 bytes) : Number of bits per sample

[Chunk containing the sampled data]
   DataBlocID      (4 bytes) : Identifier « data »  (0x64, 0x61, 0x74, 0x61)
   DataSize        (4 bytes) : SampledData size
   SampledData
