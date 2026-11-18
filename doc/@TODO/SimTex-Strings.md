

Q: Could SimTex have used manifest constants for its in-code strings, rather than string literals?
A: 



I'll check what ReMoM does with strings now and what the OG data segment shows, then look at the period sources.

I'm checking whether the string literals in the OG data segment are shared tails of longer strings, which would mean BCC 3.1 emitted literals rather than named arrays.

Confirmed that BCC 3.1 with -d pools a module's literals at the end of its data and shares tails,
 so the original game passed literals at the call sites rather than named arrays.

Next I'm checking cross-module consistency and what the MoO2 material shows.

SimTex already stores its long text (help, diplomacy, spells, buildings, events, messages and item names) in data files. I'm checking next whether the strings repeated across several places are consistent enough to justify a shared header.



I have to take back what I said last time: SimTex didn't use a strings header. MoM's short strings were typed by hand at each call site, so Newgame_Screen_0_Draw's option names were most likely typed into the function too.

What MoM's compiled strings show about SimTex's style
They wrote literals in the calls, not named string arrays. I tested this with BCC 3.1:

It puts all of a file's string literals in one block after its other data, in the order the code first uses them.
With -d it lets a short literal reuse the end of a longer one.
NEWGAME and the settings screen both have that layout, with shared endings. " picks" is the end of "You have already made all your picks", and "" is the closing zero of "WIZARDS.EXE". A named char x[] = "..." gets its own storage and never shares. So ReMoM's char cnst_Wiz_Select[] = "Select Wizard"; at NewGame.c:1003 is not the original form; Print_Centered(242, 1, "Select Wizard") is.

They typed them by hand. A shared #define would give identical bytes everywhere, and these aren't:

One LBX file name is spelled SPECFX, Specfx and specfx inside a single WIZARDS overlay, and SpEcFx in another.
hlpentry and hlpentry.lbx both appear.
NEWGAME has two local tables of realm names, in two different functions, in two different orders. Those are the same kind of table as the option names.
Long text went in data files: HELP.LBX (1.7 MB), DIPLOMSG, DESC, BUILDESC, EVENTMSG, MESSAGE and ITEMDATA.

By MoO2 they had changed approach. SimTex's own MoO2 function names include:

Set_Mox_Language and Load_Mox_Global_Strings in MOX2.C
Load_E_Strings in ESTRINGS.C
Load_Russ_Strings in RUSS.C
That means strings loaded when the game runs, with a language setting. The files named after people (ERIC.C, RUSS.C) hint that the strings were split by programmer. I couldn't see what those functions load, because there's no MoO2 game data on disk.

Side result: 
Devel-Borland-Codegen-Test.md says -d can't be checked from the asm. 
It can: without -d, BCC 3.1 never shares endings (I compiled it both ways), and the OG does share them.

How other projects did it
id Software used #define headers, chosen when compiling.
Wolfenstein 3D's FOREIGN.H has entries like STR_NG "New Game" and STR_SD "Sound".
Doom's d_englsh.h has entries like GOTARMOR and PD_BLUEK. Its dstrings.h includes d_french.h or d_englsh.h depending on #ifdef FRENCH, so each language needed its own EXE.
Wolf3D's two-letter names are exactly the "chase down the name" problem you want to avoid.
SimTex's MoO2 loaded string tables at run time, as above.
What I'd do in ReMoM
Put the literal in the call, as SimTex did, and skip the header:

That's the form the compiled bytes show. The cnst_ arrays are a reconstruction artifact, so converting them is a REBUG, like the colour arrays.
There's no name to look up: you type the text you see on screen or in the data segment.
A #define header only helps translation, and only with one build per language. If ReMoM ever wants translations, MoO2's run-time tables are the model. Going from literals to that later is mechanical.
Your other questions:

How to start: NewGame.c, which has 49 cnst_ strings and is the file we've been working in. Convert, build and run DeMoM, then do the rest with the tool. There are 729 in all; UnitView.c alone has 200.
One header for the project? No header. If translation comes later, use one run-time table, not #defines.
A tool? Yes, a python3 tool in tools/ that:
takes each string's exact bytes from the IDA listings;
replaces every use of a cnst_ name with the literal, and comments out the old definition with a REBUG note;
writes control bytes as octal ("\033", not "\x1B", which also swallows any hex digits after it);
has a check mode that flags any literal that doesn't match the original bytes. Typos are the one real weakness of literals, and this catches them.
Easy for the team: with literals there's nothing to look up. What's left is when rebuilding from asm: a function's asm shows offset cnst_Pick_Error_22, not the text. If Piethawn wrote the text as a comment on those lines (; " picks in "), people could type the literal straight from the asm file.
I checked the conversion is safe: nothing in ReMoM writes into these strings or takes their sizeof.

The compiler test is C:/Users/jbalcomb/AppData/Local/Temp/claude/c00932cd21261fca/b8775e42-472b-43e5-a9eb-08f89368ed55/scratchpad/bcc9/LIT.C.
