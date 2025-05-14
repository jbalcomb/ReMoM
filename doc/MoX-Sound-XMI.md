
FAT.AD
FAT.CAT
FAT.MT
FAT.OPL
FAT-GEN.CAT
FAT-OPL3.BNK
FATV10.BNK



# TBD, someday, maybe...
http://www.palserv.com/XMidi/
https://libtimidity.sourceforge.net/
https://github.com/sezero/libtimidity
https://stackoverflow.com/questions/19613244/xmidi-file-format
https://github.com/Mindwerks/wildmidi/blob/master/docs/formats/XMIFileFormat-AIL.txt
https://ml.csound.1bpm.net/mailinglist.csound/thread/60154
https://github.com/sezero/libtimidity/tree/master/src
...
https://github.com/jimdose/Apogee_Sound_System
https://github.com/volkertb/JUDAS
https://www.vogons.org/viewtopic.php?t=91224
https://discourse.libsdl.org/t/playing-midi-music-through-sdl-mixer/10659
https://www.google.com/search?q=SDL2+play+midi+music&rlz=1C1GCEU_enUS1161US1161&oq=SDL2+play+midi+music&gs_lcrp=EgZjaHJvbWUyBwgAEEUYoAEyBwgBECEYoAHSAQg2MDg0ajFqN6gCCLACAfEFFzJzOrUzry0&sourceid=chrome&ie=UTF-8
http://cd.textfiles.com/vgaspectrum/sound/
http://cd.textfiles.com/20mnn/SOUND/
https://www.vgmpf.com/Wiki/index.php?title=AdLib_Visual_Composer_(DOS)
http://fileformats.archiveteam.org/wiki/AdLib_Visual_Composer_/_Roland_Synthesizer_song
https://www.google.com/search?q=AdLib+Visual+Composer&rlz=1C1GCEU_enUS1161US1161&oq=AdLib+Visual+Composer&gs_lcrp=EgZjaHJvbWUyBggAEEUYQDINCAEQABiGAxiABBiKBTINCAIQABiGAxiABBiKBTIHCAMQABjvBTIHCAQQABjvBTIKCAUQABiABBiiBNIBBzkxNmoxajeoAgiwAgHxBXFQnXf-698f&sourceid=chrome&ie=UTF-8
https://www.vogons.org/viewtopic.php?t=48513&start=20
https://www.google.com/search?q=Adlib+Insmaker+Bankfile&rlz=1C1GCEU_enUS1161US1161&oq=Adlib+Insmaker+Bankfile&gs_lcrp=EgZjaHJvbWUyBggAEEUYQDIHCAEQIRigATIHCAIQIRigATIHCAMQIRigAdIBBzQ4NmoxajeoAgiwAgHxBWp6UVbf_6gv&sourceid=chrome&ie=UTF-8
https://moddingwiki.shikadi.net/wiki/AdLib_Timbre_Bank_Format
https://moddingwiki.shikadi.net/wiki/AdLib_MIDI_Format
...
https://www.google.com/search?q=winamp+source+code&rlz=1C1GCEU_enUS1161US1161&oq=winamp+source+code&gs_lcrp=EgZjaHJvbWUyBwgAEEUYgAQyBwgBEAAYgAQyCAgCEAAYFhgeMggIAxAAGBYYHjIICAQQABgWGB4yCAgFEAAYFhgeMggIBhAAGBYYHjIICAcQABgWGB4yCAgIEAAYFhgeMggICRAAGBYYHtIBCDMwMTFqMWo3qAIIsAIB8QVLsFTjAm-9hg&sourceid=chrome&ie=UTF-8
https://github.com/alexfreud/winamp
https://github.com/alexfreud/winamp/tree/community/Src
https://github.com/alexfreud/winamp/tree/community/Src/Plugins/Output
https://github.com/alexfreud/winamp/tree/community/Src/Plugins
https://github.com/alexfreud/winamp/tree/community/Src/Plugins/Input
https://github.com/alexfreud/winamp/tree/community/Src/Plugins/Input/in_midi
https://github.com/alexfreud/winamp/blob/community/Src/Plugins/Input/in_midi/xmi.cpp
...
https://www.freepascal-meets-sdl.net/chapter-9-music-and-sound/
https://www.freepascal-meets-sdl.net/wp-content/uploads/SDL_Layers.png
...
MIDIPLEX
Meh. It's in Pascal... ...maybe, ask Implode about it
C:\STU\developp\MIDIPLEX\Main.pas











C:\STU\devel\AIL214\A214_D2\DOC\ASCIIDOC\XMIDI.TXT
The term 'Extended MIDI,' or 'XMIDI,' refers to the set of features added to the base Musical
Instrument Digital Interface (MIDI) data specification by the drivers and utility programs
included with the Audio Interface Library's Version 2.00 release.  The XMIDI standard brings
many powerful features to the Audio Interface Library which would otherwise be absent in a
system conforming to the original MIDI standard.

XMIDI provides a number of additional MIDI-style Control Change messages which greatly
extend the range of composition and arrangement options available to the musician and
application programmer.  Below is a list of Extended MIDI Control Change messages available
in the current Audio Interface Library release, together with explanations of their uses in
XMIDI sequences.

旼컴컴컴컴컴컴컴쩡컴컴컴컴컴컴컴컴컴컴컴컴컴컴?
?Controller    ?Description                 ?
팠袴袴袴袴袴袴袴妄袴袴袴袴袴袴袴袴袴袴袴袴袴袴?
?110 (6eh)     ?Channel Lock                ?
?111 (6fh)     ?Channel Lock Protect        ?
?112 (70h)     ?Voice Protect               ?
?113 (71h)     ?Timbre Protect              ?
?114 (72h)     ?Patch Bank Select           ?
?115 (73h)     ?Indirect Controller Prefix  ?
?116 (74h)     ?For Loop Controller         ?
?117 (75h)     ?Next/Break Loop Controller  ?
?118 (76h)     ?Clear Beat/Bar Count        ?
?119 (77h)     ?Callback Trigger            ?
?120 (78h)     ?Sequence Branch Index       ?
읕컴컴컴컴컴컴컴좔컴컴컴컴컴컴컴컴컴컴컴컴컴컴?












LBX sound entries have an additional 16-byte header
DEAF and 0001 or 0002



       C: HSEQUENCE AIL_register_sequence(HDRIVER driver, void far
            *FORM_XMID, unsigned sequence_num, void far
            *state_table, void far *controller_table)







Technical
The XMI format is an IFF variant used to contain MIDI events. The top-level chunk is always FORM. This is followed by an XDIRINFO chunk containing the amount of subsongs contained, written as a 16-bit little-endian integer. After that comes a CAT  chunk which contains the collection of subsongs as a series of XMIDFORM, XMIDEVNT, XMIDTIMB, and EVNT chunks. All XMI files end with hex values "FF 2F" followed by one or two nulls.

XMI files play MIDI at a fixed clock rate of 120 Hz. To play them correctly, the Tempo and PPQN division values used for playback should therefore also yield a constant 120 Hz, for example a tempo of 500 000 microseconds and a PPQN of 60, or a tempo of 1 000 000 microseconds and a PPQN of 120. Tempo events contained in the EVNT chunks should be ignored; they are vestigial remnants of the original MIDI file converted into the XMI format and do not correspond to the values used by the converted event stream.

All XMI files use IFF tree structure, and the identifiers are XDIR and XMID.

IFF Tree Structure
File Root
│
├─ FORM:XDIR     - IFF header
│  └─ INFO       - Info chunk
│
└─ CAT :XMID     - IFF subheader (catalog)
   ├─ FORM:XMID  - Subsong #1
   │  ├─ TIMB    - Timbre chunk (optional)
   │  └─ EVNT    - MIDI events chunk
   ├─ FORM:XMID  - Subsong #2
   │  └─ ...
   └─ ...




https://moddingwiki.shikadi.net/wiki/XMI_Format

File format
The file format is made up of two standard Interchange File Format (IFF) files one after the other. As there is more than one root-level chunk in the same file, technically the file does not meet the IFF standard which only allows one root-level chunk per file. The chunks are arranged as follows:

FORM (type XDIR)
INFO
Immediately following this is another IFF file, which contains a FORM subchunk for each sequence (song) stored in the XMI file:

CAT (type XMID)
FORM (type XMID) - first song
TIMB
EVNT
FORM (type XMID) - second song (optional)
TIMB
EVNT
etc. for as many songs as needed
It is unknown why the files are arranged as multiple separate IFF files concatenated, rather than one IFF with the same chunk arrangement, but perhaps it is due to a misunderstanding on the part of the format designers of the way the IFF format works.

IFF root chunk 1 (FORM:XDIR)
The FORM chunk is of type XDIR. It contains one subchunk of type INFO. The contents of the INFO chunk are as follows:

Data type	Name	Description
INT16LE	seqCount	Number of sequences (songs) in the file
IFF root chunk 2 (CAT :XMID)
This chunk is a simple list, with one FORM:XMID subchunk for each song in the file, which contains all the information necessary to play that single song. Each FORM subchunk is further broken up into the following chunks.

TIMB
The TIMB chunk stores details about the MIDI patches used in the song. It is used so that normal MIDI patch-change events can set instrument banks at the same time. It has the following structure:

Data type	Name	Description
UINT16LE	count	Entry count of the following array
Then it is followed by structure, repeated count times:

Data type	Name	Description
UINT8	patch	MIDI patch for this instrument number
UINT8	bank	MIDI bank for this instrument number
Each entry in the array is repeated once for each instrument in the current track.

The official XMI creation utility (midiform) uses MIDI controller AIL_TIMB_BNK (114) to set the bank on a given channel, and a standard MIDI program change event to store the patch. These two values (bank and patch) are used to populate an entry in the TIMB chunk on the next note-on event on the channel.



## MIDIPLEX
function TMainForm.DetectXMI(var F: TMemoryStream): Boolean;
var
  Nodes: IFFNodes;
begin
  IFFGetNodes('IFF', F.Memory, 0, F.Size, Nodes);
  Result := (IFFSearchNode(Nodes, 'CAT ', 'XMID') > 0)
         or (IFFSearchNode(Nodes, 'FORM', 'XMID') > 0);
end;


Name: AnsiString; Sub: AnsiString
if IFFSearchNode(Nodes, 'CAT ', 'XMID')
// Standard multi-song XMIDI



C:\STU\devel\AIL214\A214_D3\XMIDI.ASM

C:\STU\devel\AIL214\A214_D3\XPLAY.C





## Audio_Init()

...

AIL_state_table_size()








컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴퀼
C: unsigned AIL_state_table_size(HDRIVER driver)
Purpose:
    Returns the amount of memory in bytes required to store the state table for each XMIDI sequence registered with a given driver.
Remarks:
    One of the features provided by the AIL's Extended MIDI drivers is the ability to stop, start, and maintain the status of up to eight independent, concurrent XMIDI sequences.
    To keep the drivers' internal memory requirements to a minimum, the data pointers, time counters, and controller value registers associated with each XMIDI sequence are preserved in an external area of memory called the "state table."
    Each sequence's state table will occupy approximately 500-700 bytes; this function returns the precise amount of memory required.

The application should allocate memory for a sufficient number
of state tables to maintain as many XMIDI sequences as the
application expects to register concurrently.  Since many
applications will never need to register more than one or two
concurrent XMIDI sequences, substantial memory savings may be
realized by allowing the application to dynamically allocate
memory for its sequences' state tables.

After an XMIDI sequence's handle is released with AIL_release_sequence_handle(), its state table is no longer needed by the driver.
(Refer to the AIL_release_sequence_handle() description for an important exception.)
The state table's memory may be returned to the application's heap, or re-used when registering another sequence.
However, the required state table size may vary
between different XMIDI driver types, so a state table whose
size was determined with a call to one driver should not be
arbitrarily reassigned to a different driver.  Under no
circumstances should the sequence state table be released or
altered before its handle is released.  

Example: XPLAY.C, XP16.C, MIX16.C, WINXMIDI.C
컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴퀼
