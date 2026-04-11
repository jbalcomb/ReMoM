
fluidsynth
fluidsynth: Ignoring unrecognized meta event type 0x20

https://github.com/1oom-fork/1oom/issues/155
opened on Aug 7, 2025 · edited by 1oom-fork
Vanilla beta RC3 has become quite stable.
UI:
I don't have enough knowledge and motivation to check the XMID converter code
--- fluidsynth: Ignoring unrecognized meta event type 0x21 (Darlok audience)

https://groups.google.com/g/comp.music.midi/c/_MIjgi-8xQQ
Can anyone identify Standard Midi File meta-events 0x21 and 0x20 for me?
I have come across these two unrecognised events in many MIDI files
pulled from the net, but nowhere can I find an explanation of what they
are supposed to be! 'mftext' lists the events as follows:
Time=0 Meta event, unrecognized, type=0x21 leng=1
Time=0 Meta event, unrecognized, type=0x20 leng=1
The actual byte stream looks like this:
For 0x21: 00 FF 21 01 00
For 0x20: 00 FF 20 01 00
These meta-events seem to occur - if they are found in the file at all -
at the start of every track except for the first (timing) track. 0x21
seems much more common than 0x20.
Anybody got any clues?
(Yes, I am writing Yet Another Midifile Player.. :-)
Guenter Nagler
unread,
Apr 24, 1997, 3:00:00 AM
to Laurence Tyler
Laurence Tyler wrote:
>
> Can anyone identify Standard Midi File meta-events 0x21 and 0x20 for me?
ff 21 01 vv => prefix port
used to address more sound devices (e.g. soundcards) playing
one song together. Also used to increase number of channels
(each device has its own 16 channels).
Its a logical number (0, 1,...,not the interrupt number)
All commands of this track are sent to device vv (default=00)
ff 20 01 vv => prefix channel (vv=00-0f)
used to assign channel numbers (1-16) to meta events and sysex events
(events that have no channel number in standard midi protocol).
Usually if a sysex depends on a channel then the channel is encoded
anywhere in the channel data. So I think that this event is only
for editing purpose, that when splitting a track by channel the
sysex and meta events are also moved to right part.
see also midi2txt, midi2hex
http://hgiicm.tu-graz.ac.at/Cpub .... midi utilities for PC/UNIX
