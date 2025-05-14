
LBX entries have an additional 16-byte header
DEAF
01 or 02



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

