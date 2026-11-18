# SimTex Coding Style and Authorship

What the MoO2 debug symbols (`ORION2.EXE`, MS-DOS) let us deduce about how SimTex named things, who wrote what, and how to carry
that onto MoO1 and MoM, which have no symbols.

- **Tool:** [tools/simtex_style/simtex_authorship.py](../../tools/simtex_style/simtex_authorship.py). Run `python3 tools/simtex_style/simtex_authorship.py --validate` from the repo root.
- **Outputs:**
  - [SimTex-Authorship-MoO2-Modules.csv](SimTex-Authorship-MoO2-Modules.csv): one row per MoO2 module, with evidence, posteriors and the final call.
  - [SimTex-Authorship-ReMoM-Files.csv](SimTex-Authorship-ReMoM-Files.csv): one row per ReMoM `.c` file, with matched MoO2 modules, the MoO2 owner posterior and the projected MoM author.
  - [SimTex-Authorship-ReMoM-Functions.csv](SimTex-Authorship-ReMoM-Functions.csv): one row per ReMoM function, with its MoO2 match.
- **Inputs:** [MasterOfOrion2-FunctionIndex.fwv](../@MoO2/MasterOfOrion2-FunctionIndex.fwv), [modules/](../@MoO2/modules/) (`*.fwv`), [ORION2.asm](../@MoO2/ORION2.asm), [MoM/src/CREDITS.c](../../MoM/src/CREDITS.c#L143-L149), and the MoO1 manual's credits page (`/c/STU/devel/ReMoO1/Master_of_Orion_-_Manual_-_PC.pdf`).
- **OG MoM corpus (§5):** [tools/simtex_style/simtex_mom_corpus.py](../../tools/simtex_style/simtex_mom_corpus.py) adds MoM's shipped file names, the LBX name tables and the file-name strings in WIZARDS.EXE / MAGIC.EXE, and tests their case style against MoO2. Its outputs are the `SimTex-Corpus-*` files beside this doc.
- **MoO1 corpus (§5.5):** [tools/simtex_style/simtex_moo1_corpus.py](../../tools/simtex_style/simtex_moo1_corpus.py) runs the same passes on MoO1 v1.30, adding LBX string tables and all EXE strings. It reads the EXEs with [tools/simtex_style/borland_exe.py](../../tools/simtex_style/borland_exe.py). Outputs: [SimTex-Corpus-MoO1-Stats.md](SimTex-Corpus-MoO1-Stats.md) and the `SimTex-Corpus-MoO1-*.fwv` tables.
- **Starlords corpus (§5.6):** [tools/simtex_style/simtex_starlords_corpus.py](../../tools/simtex_style/simtex_starlords_corpus.py) runs the same passes on the May 1993 Starlords preview and compares it with MoO1. It reads the Microsoft C NE executables with [tools/simtex_style/ne_exe.py](../../tools/simtex_style/ne_exe.py). Outputs: [SimTex-Corpus-Starlords-Stats.md](SimTex-Corpus-Starlords-Stats.md) and the `SimTex-Corpus-Starlords-*.fwv` tables.
- **MoO2 game-data corpus (§5.7):** [tools/simtex_style/simtex_moo2_corpus.py](../../tools/simtex_style/simtex_moo2_corpus.py) runs the same passes on the MoO2 v1.31 install, including the per-programmer string files. Outputs: [SimTex-Corpus-MoO2-Stats.md](SimTex-Corpus-MoO2-Stats.md) and the `SimTex-Corpus-MoO2-*.fwv` tables.
- **Name scores (§8.1):** [tools/simtex_style/simtex_name_score.py](../../tools/simtex_style/simtex_name_score.py) scores every ReMoM function name against the MoO2 name shapes and joins in [SimTex-Name-Renames-Proposed.fwv](SimTex-Name-Renames-Proposed.fwv). Its outputs are [SimTex-Name-Scores-ReMoM-Functions.fwv](SimTex-Name-Scores-ReMoM-Functions.fwv) and [SimTex-Name-Scores-Stats.md](SimTex-Name-Scores-Stats.md).

**Authority.** The only SimTex-given names are the MoO2 debug symbols. Every other name in ReMoM came from drake178 or Jim Balcomb.
Names from other projects are not evidence. Every probability below is an estimate from a stated model with stated assumptions;
§6 gives the model's measured accuracy.

---

## 1. The teams (verified sources only)

| Game | Programmers credited | Source |
|---|---|---|
| MoO1 (1993) | Stephen Barcia, Maria Barcia, Ken Burd | MoO1 PC manual, credits page. "Sound Software: John Miles, John Ratcliffe" (the Miles AIL). |
| MoM (1994) | Jim Cowlishaw, Ken Burd, Steve Barcia, Grissel Barcia | [CREDITS.c:146-149](../../MoM/src/CREDITS.c#L146-L149) |
| MoO2 (1996) | first names only, from module file names: **Jim, Ken, Harold, Eric, Russ, Bill, Evan** | `jim.c`, `ken.c`, `harold.c`/`haccess.c`, `eric*.c`/`escroll.c`/`estrings.c`, `russ.c`, `bill*.c`, `evanhelp.c` |

- The MoO2 credits are not plain strings in `ORION2.EXE`, so MoO2 surnames are **unverified**. The only full name in the EXE is the literal `'Evan Axelrad'`, which sits among the modem / network strings.
- `jim.c` = Jim Cowlishaw and `ken.c` = Ken Burd is a working hypothesis: they are the only Jim and Ken in the MoM credits.
- **Ken Burd is the only programmer credited on all three games.**
- Jim Cowlishaw first appears with MoM.
- Harold, Eric, Russ, Bill and Evan appear only in MoO2.
- Whether "Grissel Barcia" (MoM) and "Maria Barcia" (MoO1) are the same person is **unknown**. Don't assume it.

---

## 2. Build-layout evidence

### 2.1 Directory roots

The Watcom debug info records each object's full compile path. Four roots hold the SimTex code:

| Root | Modules | Contents |
|---|---|---|
| `L:\MOX\` | 0–8 | core: `mox.c` (734 globals, 2 functions), `mox2.c` (`main2`, `Screen_Control`), `filedef.c`, `initgame.c`, `loader.c`, `nextturn.c`, `ericnet.c`, `techdata.c` |
| `D:\MOX\` | 9–17, 27, 122–132 | Jim's and Bill's lists, plus `diplomac.c` |
| `D:\MOX\OTHERS\` | 18–121 except 27 | Ken, Harold, Eric and Russ (and Evan's one file) |
| `F:\WLIBS\`, `E:\WLIBS\` | 135+ | the engine library: MoM's "MoX" grown up (`fields`, `fonts`, `animate`/`bitmap`, `video`, `mouse`, `allocate`, `lbx`/`farload`, `sound`, `net`, `modem`, ...) |

`OTHERS` reads as "other people's files" from the point of view of whoever owned `D:\MOX\`. Jim's and Bill's files are the only person-named files outside it.

### 2.2 Third-party code

Exclude these from any SimTex style statistics.

- **Miles AIL:** `R:\NET\LIBS\AIL\...`, `aila.asm`, `ailssa.asm`.
- **RAD Smacker:** `D:\DEVEL\PROJECTS\SMACK\...`, `rfile.asm`, `unsmack.asm`.
- **Greenleaf CommLib 5.2:** `C:\GCL520\...`.
- **Watcom C runtime:** the path-less modules (`cstrt386`, `fpedos4g`, ...).
- **`F:\WLIBS\compress\`** (`arith1e.c`, `bitio.c`, `errhand.c`). The file names and the CamelCase / lowercase names (`OpenInputBitFile`, `InputBits`, `fatal_error`) are those of Mark Nelson's *The Data Compression Book*. SimTex wrapped it in house style (`Compress_Data`, `Uncompress_Data`). This is a filename-and-name match, not a byte comparison.

### 2.3 Link order is each programmer's object list

Module numbers are link order. Between modules 9 and 132 the order breaks into runs that are each **alphabetically sorted** and that each contain exactly one programmer's seed file. The tool prints the runs.

| Modules | Owner | Sorted run(s) |
|---|---|---|
| 9–17 | Jim | `antaroom smack` \| `racecust` \| `council diplodef` \| `dip-scrn events jim npcdiplo` |
| 18–33 | Ken | `cmbtai … combinit diplomac initship ken raceopt racesel shipstak techinit`: one run, 16 files |
| 34–64 | Harold | `aipower` \| `aidesign hotpop` \| `advciv antarans … evanhelp … haccess harold … textbox`: 28-file run |
| 65–89 | Eric | `beams … colxport eric fakebuts … ocpolpup` (21 files) \| `erichelp escroll estrings newpup` (appended) |
| 90–121 | Russ | `aibomb … russ … trade`: one run, 32 files |
| 122–132 | Bill | `bill billhelp fmtpara info list racerprt racescrn science tech turnsum` \| `fieldsav` (appended) |

This structure was found **after** the dialect evidence of §3, and it is independent of it. The two agree on **35 of 37** non-seed modules that have a hard or strong label (§6).

---

## 3. House style: rules shared by everyone

Counts are over the SimTex game code (the `L:\MOX`, `D:\MOX` and `OTHERS` roots): 2,488 code and 1,462 data symbols.

### 3.1 Decoration (Watcom, MoO2 only)

Read the source name back as follows:

| Symbol | Source name |
|---|---|
| `Name_` (code) | `Name` |
| `_name` (data) | `name` |
| `_Squeeze_Print_` | `_Squeeze_Print` (a real leading underscore) |
| `__pp_max_height` | `_pp_max_height` |
| labels from `.asm` modules | written as-is, no decoration |

MoO2 is Watcom C/386 (flat 32-bit, register arguments EAX, EDX, EBX, ECX). MoM is Borland C++ 3.1 (16-bit). So compare code shape **at the source level**, never byte against byte.

### 3.2 Case

- **Functions are `Proper_Snake_Case`: 98.4%** (WLIBS: 95.3%). Every token is capitalised, except small conjunctions written by some people (`_to_`, see §4).
- **Globals are `lower_snake_case`: 97.1%.** The exceptions are deliberate:
  - LBX-file-name variables: `COLONY_LBX`, `PLANETS_LBX` (27).
  - Constant-like globals: `NUM_SHIPS`, `NUM_STARS`, `MAP_MAX_X`.
  - Debug switches: `DEBUG`, `MAPGEN_TRACE`.
- **qsort comparators are lowercase:** `qsort_ship_array_by_size`, `qsort_ship_initiative`, `compare_tech`, `airefit_cmp`.
- **CamelCase means not SimTex**, or WLIBS code adapted from elsewhere (§4, WLIBS).

### 3.3 Function shapes

The mean is 3.6 tokens per name. Names are spelled out.

**Verb-first** is the default. The top first tokens are:

| Token | Count | Token | Count | Token | Count |
|---|---|---|---|---|---|
| `Get` | 226 | `Clear` | 37 | `Update` | 25 |
| `Draw` | 179 | `Init` | 32 | `Check` | 24 |
| `Set` | 120 | `Print` | 31 | `Reload` | 23 |
| `Load` | 88 | `Do` | 26 | `Build`, `Generate`, `Pick`, `Compute`, `Apply` | — |
| `Add` | 56 | | | | |

**Noun-first** is for accessors, predicates and counts:

- Accessors: `Star_Owner`, `Ship_Stack_X`, `Design_Weapon_Count`, `Planet_Type`.
- Predicates: `Player_Has_Stargate`, `Star_Is_Orion`, `Is_Combat_Ship`, `Player_Can_Order_Ship`.
- Counts use an `N_` prefix: `N_Planets_In_System`, `N_Colony_Owners`, `N_Player_Ships` (32 names start with `N`).

**Converters** use `From` / `To`: `Star_Id_From_Colony_Id`, `XY_To_Star_Id`, `Hyper_Field_To_Pseudo_App_Index`.

**Common suffixes:**

| Suffix | Count | Examples |
|---|---|---|
| `_Screen` | 98 | `Planet_Data_Screen`, `Colony_Summary_Screen` |
| `_List` | 73 | |
| `_Popup` | 47 | |
| `_Fields` | 47 | |
| `_Message` | 38 | |
| `_Help` | 35 | |
| `_Box` | 32 | |
| `_Bonus` | 33 | |
| `_Info` | 29 | |
| `_Anims` / `_Anim` | 24 / 16 | |
| `_Id` | 19 | |
| `_Cost` | 19 | |

**Screen lifecycle:** `<X>_Screen` is the screen loop, `Load_<X>_Screen_Pictures` loads its art (`Load_Main_Screen_Pictures`), `Reload_<X>_Screen` redraws it after a popup (`Reload_Newgame_Screen`), and `Set_<X>_Help_List` sets its help.

**Variants:**

- Output-target pairs: `Draw_X` / `Draw_X_To_Bitmap`, `Squeeze_Print_Centered` / `Squeeze_Print_Centered_To_Bitmap`.
- A raw worker with a leading underscore behind a public wrapper: `_Print_Centered` / `Print_Centered`, `_Squeeze_Print` / `Squeeze_Print`.
- A digit for a second version: `main2`, `Font_Colors2`, `String_Builder2`, `Init_Homeworld_Colony2`.
- A C type as suffix: `Get_Weighted_Choice_Long`, `_Char`, `_Short`, `_Int`; `Shuffle_Sint`.

**Abbreviations are few and conventional:**

| Group | Tokens |
|---|---|
| Upper-case | `AI`, `NPC`, `LBX`, `XP`, `HCLS`, `PFMT` |
| Mixed-case | `Id`, `Bldg`, `Anim`, `Pop`, `Pct`, `Msl`, `Spcl`, `Res`, `Hi` / `Lo` |
| File-name-only | `cmbt`, `col`, `flt`, `plnt` |

Most of the file-name-only forms never appear inside function names.

### 3.4 Global-data shapes

**Suffixes:**

| Suffix | Count | Meaning |
|---|---|---|
| `_seg` | 244 | Pointer to a loaded LBX entry or picture. The 16-bit "segment" idiom survived into 32-bit. |
| `_button` | 73 | |
| `_list` | 51 | |
| `_colors` | 44 | |
| `_field` / `_fields` | 39 / 14 | Field / hot-spot indexes |
| `_flag` | 38 | |
| `_buffer` | 25 | |
| `_count` | 23 | |
| `_x` / `_y` | 20 / 21 | |
| `_ptr` | 13 | |
| `_id` | 12 | |

**Prefixes:**

| Prefix | Count | Meaning |
|---|---|---|
| Screen / subsystem name | — | `fleet_` (62), `diplomacy_` (36), `combat_` (33), `officer_` (31), `main_` (29), `fltscrn_` (12) |
| `g_` | 36 | General globals (`g_galaxy_size`) |
| `temp_` | 27 | |
| `n_` | 17 | Counts (`n_links`) |
| `cur_`, `old_`, `last_`, `first_` | — | |

### 3.5 Per-programmer module conventions (team-wide)

- **Init hooks.** A programmer may own `<Name>_Init` and `<Name>_Defaults`, called from `Init_New_Game` and `Set_Defaults` (verified callers: `Harold_*`, `Eric_Init`, `Bill_*`).
- **A personal string file plus an accessor:**

  | Programmer | String file | Accessor |
  |---|---|---|
  | Jim | `jimtext.lbx`, `jimtext2.lbx` | |
  | Ken | `KENTEXT.LBX`, `KENTEXT1.LBX` | `Ken_Get_Text_Message` |
  | Bill | `billtext.lbx`, `billtex2.lbx` | |
  | Eric | `estrings.lbx` plus language variants `estrFREN`, `estrGERM`, `estrITAL`, `estrSPAN`, `estrPOLI` | `E_Strings` |
  | Harold | `H?STRNGS.LBX`, one per language letter | `H_Message` |

- The generic loader `Get_Text_Message(lbx_name, entry, buffer)` sits in `jim.c` and is shared: Bill calls it with `billtext.lbx`.
- **Russ** has no personal LBX. Each of his modules has a `Load_<Module>_Strings` sub-loader, called from `Load_Russ_Strings`.

---

## 4. Per-programmer fingerprints

"Owns" lists come from the final call in [SimTex-Authorship-MoO2-Modules.csv](SimTex-Authorship-MoO2-Modules.csv). Brackets give `P_FINAL`, or the hard-label reason.

### Jim: [jim.c](../@MoO2/modules/0016-jim.c.md), `D:\MOX` list 9–17

- **Text:** `jimtext.lbx` through the generic `Get_Text_Message`. Token substitution through the `Decode_Text_*` family (`Decode_Text_Name_And_Tech`, `Decode_Text_Race_And_Name`, ...).
- **Audio and presentation:** music and streaming (`Play_Background_Music`, `Fade_Down_Background_Music`, `Play_Combat_Music`, `Play_Streaming_Music`), logos and fades (`Draw_Logos`, `Logo_Fade_Out`, `Slow_Fade_In`, `Check_For_Simtex_Logo`).
- **Only programmer who namespaces his globals with his own name:** `jim_current_stream`, `jim_fade_music_flag`, `jim_old_music_id`.
- A personal debug hook, `Jim_Hot_Key`. No `Jim_Init` / `Jim_Defaults`.
- **Owns:** `council` (1.00), `dip-scrn` (1.00; 84 functions, the diplomacy screens), `events` (1.00). By list position: `npcdiplo`, `diplodef`, `racecust`, `antaroom`.

### Ken: [ken.c](../@MoO2/modules/0029-ken.c.md), `OTHERS` list 18–33

- **Text:** `KENTEXT.LBX` / `KENTEXT1.LBX` through `Ken_Get_Text_Message`.
- **Weighted-random family:** `Get_Weighted_Choice`, `_Long`, `Get_Weighted_Best_Choice`, `Get_Weighted_Choice_Best_Long`.
- **`_Seg`-returning picture getters:** `Get_Ship_Picture_Seg`, `Get_Ship_Id_Picture_Seg`. The typo `Get_Ship_Picture_Segk` survives.
- Long conjunctive names: `Init_NPC_Personalities_Objectives_Themes`.
- `Ken_Defaults` and `Ken_Init` are **one empty 1-byte function at one address**, and nothing in the listing calls them.
- **Owns:**
  - The ship-combat cluster: `cmbtdrw1`, `cmbtfir2`, `combat`, `combat1` (each 1.00); `cmbtmis` (0.99), `cmbtmov1` (0.96), `cmbtai` (0.94), `combinit` (0.90), `cmbtfire` (0.73).
  - `raceopt`, `racesel` (1.00).
  - `initship`, `shipstak`, `techinit`.
  - `diplomac` (0.78), by list position only. It is compiled from `D:\MOX\` but sits alphabetically inside Ken's list.

### Harold: [harold.c](../@MoO2/modules/0048-harold.c.md) + [haccess.c](../@MoO2/modules/0047-haccess.c.md), `OTHERS` list 34–64

- **Text:** `H_Message` / `Load_H_Strings` over `HESTRNGS`, `HFSTRNGS`, `HGSTRNGS`, `HISTRNGS`, `HSSTRNGS`.
- **Localization infrastructure:** `Reload_Language_Strings`, `Load_Language_Fonts`, `Write_Language_Setting_File`, `Get_Monetary_Unit_String`.
- **The accessor library** (`haccess` = "Harold access"), the heaviest noun-first and predicate style in the game:
  - `Ship_Stack_X` / `_Y` / `_Star_Id`
  - `Design_Weapon_Firing_Arc`
  - `Player_Has_Jumpgate`
  - `N_Satellites`
  - `Star_XY_To_Ship_Id`
- Map scaling and zoom: `Get_Map_Scale`, `Increment_Zoom_Count_With_Limit`.
- **`Harold_Defaults` is real code (873 bytes).** It and `Harold_Init` are called from `Init_New_Game`. `Harold_Init` is also called from the net-setup screens.
- **Owns:** main screen (`mainscr`, `mainscr2`), fleets (`flt1`, `flt2`, `fleetpop`), ship design (`design`, `desbox`), `officer`, `sys`, `loadsave`, `plntsum`, `ships`, `homegen`, `hotpop`, `movebox`, `mapgen` (0.82), `mainmenu` (0.86).
- His dialect (10 of 10 string calls) is also in `multplay`, which sits in Russ's list. That is one of the two cross-signal conflicts.

### Eric: [eric.c](../@MoO2/modules/0079-eric.c.md), [ericnet.c](../@MoO2/modules/0007-ericnet.c.md), [erichelp.c](../@MoO2/modules/0086-erichelp.c.md), [escroll.c](../@MoO2/modules/0087-escroll.c.md), [estrings.c](../@MoO2/modules/0088-estrings.c.md), `OTHERS` list 65–89

- **Text:** `E_Strings` over `estrings.lbx` and the language variants. Also the `E_` / `_E_` personal prefix (`Add_E_Scroll_Bar_Field`).
- **Colony side:** every `col*` module, `cmbtspec`, `mainpups`, `newgame`, `beams`, `nextturn` (0.97), `score` (0.98, in Harold's list; the other cross-signal conflict).
- **Cinematics and fades:** `Play_*_Flic`, `Fast_Fade_In`, `Set_Fade_Speed_Percent`.
- **Small generic helpers:** `Max`, `Min`, `Sint_Is_In_List`. Note the `Sint` type token.
- **Casual naming:**
  - Lowercase `to` inside Proper_Snake names: `Pulsing_Rect_to_Bitmap`, `Draw_Colony_Bldgs_to_Bitm`.
  - A typo: `Play_Loose_Flic`.
  - Self-describing data: `hacky_need_to_fade_to_colony_palette_flag`.
- `Eric_Defaults` is empty (1 byte). `Eric_Init` is 10 bytes.
- **Help style:** `Set_<Screen>_Help_List`.

### Russ: [russ.c](../@MoO2/modules/0117-russ.c.md), `OTHERS` list 90–121

- **Text:** per-module `Load_<Module>_Strings` (`Load_Colcalc_Strings`, `Load_Netmox_Strings`, ...), registered in `Load_Russ_Strings`. **That registration is the strongest ownership proof in the binary.**
- **Typed-variant utilities**, used by everyone: `Get_Weighted_Choice_Char` / `_Short` / `_Int`, `Shuffle_Char` / `_Sint` / `_Int`, `Shuffle`, `Compare_Ratios`. His share of cross-module utility calls is the largest; §6 explains why that has to be discounted.
- **Infrastructure and test hooks:** `PFMT`, `Assert_Settings`, `Mox_Update`, `Make_All_AI_Game`, `all_ai_game`.
- **Owns:**
  - By aggregator (hard): `colcalc`, `combfind`, `msg`, `netmox`, `netstart`, `netturn`, `report`.
  - By model: the AI modules (`aimove` 1.00, `aibuild` 0.99, `aidata` 0.99, `aidudes` 0.95, and the other `ai*` files by list position), `initgame` (0.84), `techinit`.

### Bill: [bill.c](../@MoO2/modules/0122-bill.c.md) + [billhelp.c](../@MoO2/modules/0123-billhelp.c.md), `D:\MOX` list 122–132

- **Text:** `billtext.lbx` / `billtex2.lbx` through Jim's `Get_Text_Message`.
- **`Bill_Default`, singular.** Everyone else writes `_Defaults`. Both his hooks are real code.
- **The most systematic layering in the codebase.** `_Squeeze_Print` is the worker, `Squeeze_Print*` the public API, and each has a `*_To_Bitmap` twin. `_Print_Paragraph` sits behind `_Print_Paragraph_Bill`.
- **Perspective names:** `Get_My_Spy_Number`, `Get_Their_Spy_Mission`, `Take_My_Agent`, `Add_Their_Spy`.
- **Help style:** `<Topic>_Help` functions over `<topic>_help_list` data (`Tech_Review_Help`, `tribute_cash_help_list`). This differs from Eric's and Evan's `Set_*_Help_List`.
- **Owns:** `racescrn`, `racerprt`, `tech`, `science` (each 1.00), `info` (0.99), `list` (0.98), `turnsum`, `fmtpara`, `fieldsav`.

### Evan: [evanhelp.c](../@MoO2/modules/0042-evanhelp.c.md), inside Harold's list

- Help lists for Harold's screens: `Set_Main_Screen_Help_List`, `Set_Fleet_Screen_Help_List`, ...
- The help-text getters `Get_Help_LBX_Message` and `Get_Application_Description`.
- CamelCase screen abbreviations inside snake case: `Set_PlntSum_Screen_Help_List`, `Set_ColOff_Screen_Help_List`, `Set_ShipOff_...`. These are names of Harold's files and screens.
- Evan was attached to Harold's list. He has no other module.

### Steve Barcia, Grissel / Maria Barcia

There is no MoO2 seed, so there is nothing to attribute directly. They matter for MoO1 and MoM (§7). MoO1's credited programmers are Stephen Barcia, Maria Barcia and Ken Burd. On the MoO1 → MoM → MoO2 lineage, the engine library's oldest layer most plausibly comes from them. That is a hypothesis; nothing in this data tests it.

### WLIBS (the engine library)

No WLIBS module carries any dialect, list or helper signal. All 55 come out as "Other" at the prior, and that is the honest answer. Style clues:

- **`sound.c`** (`F:\WLIBS\sound\`) is the only SimTex-path code with camelCase globals (`soundOn`, `doubleBufferEnabled`, `globalMusicVolume`). It is a separate hand.
- **`sound\lbx.c` and `sound\dllload.c`** use `PREFIX_lowerCamel` names (`LBX_readRecord`, `MEM_alloc_lock`, `VMM_lock_range`, `FILE_read`, `DLL_load`). That is a vendor-SDK style, not SimTex style. The source is unidentified.
- **`fonts.c`, `simmodem.c` and `motrans.c`** were compiled from `E:\WLIBS`; everything else from `F:\WLIBS`. That means a different machine or a different time.

---

## 5. The OG MoM corpus: file names, LBX name tables, file-name literals

MoM has no symbols, but it does ship names SimTex chose: the file names, a name table inside most LBX files, and the file-name strings the
code passes to the LBX loader. This section adds them to the corpus.

- **Tool:** [tools/simtex_style/simtex_mom_corpus.py](../../tools/simtex_style/simtex_mom_corpus.py). Run `python3 tools/simtex_style/simtex_mom_corpus.py` from the repo root (stdlib only; 20,000 permutations, seed 1, about a minute).
- **Inputs:**
  - The OG v1.31 install (`/c/STU/DBWD/MoM131`) and the OG demo (`/c/STU/DBWD/MOMDEM`).
  - The IDA 5.5 exports of WIZARDS.EXE and MAGIC.EXE (`/c/STU/devel/STU-Extras/Piethawn/Piethawn/in/{WIZARDS,MAGIC}.asm`).
  - [ORION2.asm](../@MoO2/ORION2.asm) and the MoO2 module owners from [SimTex-Authorship-MoO2-Modules.csv](SimTex-Authorship-MoO2-Modules.csv).
- **Outputs:**
  - [SimTex-Corpus-MoM-Files.csv](SimTex-Corpus-MoM-Files.csv): every file in both installs, classified.
  - [SimTex-Corpus-MoM-LBX-Entries.csv](SimTex-Corpus-MoM-LBX-Entries.csv): every LBX name-table row.
  - [SimTex-Corpus-MoM-LBX-Literals.csv](SimTex-Corpus-MoM-LBX-Literals.csv): every file-name string in the WIZARDS / MAGIC dseg, with the segments that use it and the ReMoM file each segment maps to.
  - [SimTex-Corpus-MoO2-LBX-Literals.csv](SimTex-Corpus-MoO2-LBX-Literals.csv): the same for ORION2.EXE, with module and owner.
  - [SimTex-Corpus-Stats.txt](SimTex-Corpus-Stats.txt): all numbers quoted below.
- **Overlay = OG source file.** Under Borland's medium model each `.c` file compiles to its own code segment, and VROOMM overlays whole segments. So each `ovrNNN` / `segNNN` is one OG source file, in link order. That is why ReMoM's [Combat.c](../../MoM/src/Combat.c), which holds code from overlays 090, 099, 103, 113, 122, 123 and 124, stands for at least seven OG files. Segments map to ReMoM files through the `// WZD o54p01` header comments.

### 5.1 File names

Files in the v1.31 install, by class:

| Class | Count | Contents |
|---|---|---|
| SimTex-named | 101 LBX + 3 EXE | `WIZARDS`, `MAGIC`, `ITEMMAKE` |
| Named in the code, created at run time | 3, plus the save pattern | `CONFIG.MOM`, `MAGIC.SET`, `COMBAT.TMP`, and the save pattern `SAVEn.GAM` |
| Third party | 8 | Miles `FAT*` timbre banks, `PKUNZJR.COM` |
| Publisher | 5 | `INSTALL.*`, `MPSCOPY.EXE`, `README.TXT` |
| Not shipped | 21 | Saves and other files added later |

- **8.3, upper case, no separators.** No MoM LBX name has `_` or `-`. MoO2 allows `_` (`APP_PICS`).
- **The 8-character limit binds.**
  - 56 of 101 MoM LBX stems use all eight characters. In MoO2 it is 80 of the 142 LBX stems the code names: the same pressure.
  - `FIGURES1`–`FIGURES9` become `FIGURE10`–`FIGURE16` once a second digit is needed: the `S` is dropped to stay at eight.
- **How names are squeezed:**

  | Method | Examples |
  |---|---|
  | Consonant skeleton | `CMBT`, `SNDDRV`, `HLPENTRY`, `SPLMASTR`, `LISTDAT` |
  | Clipped words | `TERRSTAT`, `TERRTYPE`, `BUILDESC` (BUILD + DESC sharing the D), `CMBDESRT` |
  | Run-together compounds | `MAINSCRN`, `CITYSCAP`, `MOODWIZ`, `LILWIZ`, `HALOFAM`, `WALLRISE` |

  The combat terrain files come in pairs whose second member replaces the last letter with `C`: `CMBGRASS` / `CMBGRASC`, `CMBDESRT` / `CMBDESRC`, `CMBMOUNT` / `CMBMOUNC`, `CMBTUNDR` / `CMBTUNDC`.
- **11 stems are shared with MoO2:** `COMBAT`, `DIPLOMSG`, `EVENTMSG`, `EVENTS`, `FONTS`, `HELP`, `HERODATA`, `INTRO`, `MONSTER`, `MUSIC`, `NEWGAME`.

### 5.2 LBX name tables

**Format.** An LBX whose first record starts at or after 0x200 + 32 × count holds a table at 0x200 with one 32-byte row per record:

- a 9-byte entry name (8 characters + NUL);
- a 23-byte description.

**Coverage.** 79 of the 101 v1.31 LBX files have one, giving 5,065 rows: 4,886 named, 891 distinct names, 1,562 with a description. The 22 without a table are the data and sound files (`BUILDDAT`, `SPELLDAT`, `TERRSTAT`, `SOUNDFX`, `HELP`, `MESSAGE`, ...).

**Entry names:**
- **They are DOS file names, and almost certainly the art source files.**
  - All are upper case except two `No File` placeholders.
  - 62% are exactly 8 characters (3,040 of 4,886).
  - 3,709 of 5,065 rows repeat the previous row's name. One name covers a whole sheet or animation; the description tells the frames apart (`BUTTONS2` → "Continue button", "Load button", ...).
  - 82 names appear in more than one LBX (`GRASGRID` 9, `SETTLERS` 9, `TREES` 8).
- **Abbreviations are made per name, not from a table.** The same word is cut differently in different names:

  | Word | Cuts |
  |---|---|
  | halberd | `HALB`, `HLBRD`, `HALBE` |
  | engineer | `ENGNR`, `ENGIN` |
  | shaman | `SHAMN`, `SHAMA` |
  | priest | `PRIEST`, `PRST` |

  Race codes are just as loose: `BRB`, `BST`, `DRK` / `DK`, `DRA`, `NOM` / `NM`, `LIZ`, `ORC`, `TRL`, `HM`, `KL`. The `_` appears in only 30 entries (`LOAD_BUT`, `B_NOTIFY`, `E_ROADS`, ...).

**Descriptions** are informal lower-case notes:
- 1,201 of the 1,562 are all lower case.
- They have their own abbreviations: `backgrnd` (18), `scrn` (9), `pic`, `dn`.
- Only 7 reach the 22-character limit, so descriptions were not squeezed; names were.

**Demo vs v1.31.**
- 42 files have tables in both. Records were inserted and reordered between builds, so 1,115 positions differ.
- Only 9 demo names are absent from v1.31 altogether: `NULL`, `SLR06N`, `SLR06P`, `DEMOSCR`, `FLAME99`, `FLAMEF99`, `DIPLGARG`, `PLATIN2`, `RIVER`. These are placeholders and demo-only art.

**Who named these?** Unknown. The names could be the artists' as easily as the programmers'. They are SimTex naming, but nothing in them identifies a person.

### 5.3 File-name literals in the code

**Counts.**
- WIZARDS.EXE has 220 file-name strings and MAGIC.EXE 35. Every one is referenced from code.
- Most are bare stems: the loader appends `.LBX`.
- Where `.LBX` is written, the stem is upper case 45 times out of 52.

**Case of the stem:**

| | UPPER | lower | Title | Mixed |
|---|---|---|---|---|
| MoM (WIZARDS + MAGIC) | 158 | 66 | 30 | 1 (`SpEcFx`, overlay 135) |
| MoO2 (all 197 LBX literals) | 111 | 80 | 1 | 5 |

The same file is often written several ways:
- In MoM, 22 of the 55 stems that occur at least twice appear in more than one case. `SPECFX` appears in four (`SPECFX`, `Specfx`, `specfx`, `SpEcFx`); `MUSIC`, `WIZLAB` and `SPELLSCR` in three.
- In MoO2, 12 of 30 stems are mixed.

### 5.4 Does the case identify the programmer?

**Method.** Two permutation tests over the literals:

- **Purity:** the share of the majority case within each group. It is compared with two nulls:
  - a global shuffle;
  - a **same-stem** shuffle, which keeps each LBX file's own case habit. Only the same-stem null says anything about *people*.
- **Lowercase-rate chi-square** by owner, against the same-stem null.

**Calibration on MoO2, where owners are known (§6).** The test uses 125 literals: owner P ≥ 0.5, personal text files excluded.

| Test | Observed | Same-stem null | p |
|---|---|---|---|
| Purity by owner | 0.632 | 0.620 | **0.38** |
| Purity by module | 0.774 | 0.752 | 0.088 |
| Lowercase rate by owner (χ²) | 15.2 | 12.3 | 0.096 |
| Ken vs the rest, lowercase | Ken 18 / 25, rest 37 / 100 | | 0.37 |

**In MoO2, case is not a personal signature.** Owners differ only because they load different files, and each file has its habitual case. So this feature does **not** go into the authorship model.

**MoM:**

| Test | Observed | Same-stem null | p |
|---|---|---|---|
| WIZARDS, purity by overlay | 0.781 | 0.728 | **0.0013** |
| MAGIC, purity by overlay | 0.742 | 0.758 | 1.0 |
| WIZARDS overlays in link order: case changes between neighbours (58 overlays) | 18 | 32.4 (random order) | **5e-05** |

In WIZARDS, case clusters by overlay even after the file's own habit is controlled for. Overlays that are neighbours in the link order share it. The sequence splits into three blocks:

| Overlays | Case | ReMoM files |
|---|---|---|
| 050–103 | UPPER (Title only in 094 `Desert`/`Tundra` and 099 `Magic`/`Vortex`) | LOADSAVE, LOADER, CityScr, City_ovr55, ProdScr, MainScr, CityList, ArmyList, UnitStat, MagicScr, WIZVIEW, ItemScrn, AdvsrScr, Outpost, EVENTS, Lair, DIPLOMAC, Combat (090, 099, 103), Surveyor, LVLMAKE |
| **104–140** | **lower and Title dominate** | Help, SCORE, Items, CMBMAGIC, Combat (113, 122–124), ItemMake, Spellbook, NEXTTURN (121, 140), Settings, REPORT, HIRE, Spells128–137, SBookScr, OverSpel, SPLMASTR |
| 143–163 | UPPER | WZD_o143, NewGame / GENDRAW, LoadScr, TEXTBOX / Fields |

Overlays 093 (UPPER: `CONQUEST`, `WIN`, `LOSE`, `MAGIC.EXE`, ...), 154 (`COMBAT.TMP`) and 163 (UPPER: the `CMB*` terrain files and `WALLRISE`) have no ReMoM `// WZD o` comment yet, so they have no file name in the table.

**What the blocks mean:**

- **104–140 is a contiguous run of OG source files with a shared habit.** In MoO2, contiguous link-order runs were per-programmer object lists (§2.3). So this is the first structural hint of how MoM's object lists were split. It shows *grouping*, not *identity*: MoO2 says case is not a personal mark, and the habit could equally be an era or a copy-paste lineage. No author is assigned from it.
- **MoM's combat code divides across the boundary.** Overlays 090, 099 and 103 (UPPER / Title) sit in the first block; 113 and 122–124 (lower) sit in the second. Whatever the reason, the OG combat code was more than one file and probably more than one stretch of work.
- **The cross-tab against the §7.3 projection is consistent but proves nothing.**
  - Files projected to Ken: 12 of 24 literals lower case. Files projected to Jim: 2 of 16. That leans the same way as Ken in MoO2.
  - But the projection is circular (§7.2), and the MoO2 Ken effect does not survive the same-stem null (p = 0.37).

**What changes:** nothing in the §6 posteriors or the §7.3 projection. What's new:

- the corpus itself;
- the overlay = source-file map;
- two link-order boundaries, around 103/104 and 140/143, to test against the authors' answers (§9).

### 5.5 MoO1 v1.30

[tools/simtex_style/simtex_moo1_corpus.py](../../tools/simtex_style/simtex_moo1_corpus.py) runs the same passes on the MoO1 v1.30 install (`/c/STU_DASM/_MoO1v130`). Full numbers: [SimTex-Corpus-MoO1-Stats.md](SimTex-Corpus-MoO1-Stats.md).

**How the EXEs were read.** MoO1 has no IDA export. [tools/simtex_style/borland_exe.py](../../tools/simtex_style/borland_exe.py) reads the Borland executables directly:

- **Data segment:** the copyright string at DS:0004 gives the base, so every string has a DS offset.
- **Overlays:** the `INT 3Fh` stub chain gives each overlay's code location.
- **Resident segments:** the relocation table gives the code segment boundaries.
- **References:** any immediate operand equal to a string's DS offset.
- **Validation:** checked against the MoM IDA exports. On all 52 WIZARDS / MAGIC file-name literals it finds the same strings, the same referencing overlays and the same resident references.

The install's `STARMAP.EXE` was patched (a quiz bypass, 1 byte at 0x71BE6), so the original `STARMAP.EXE.quiz_bak` is read instead.

**What MoO1 adds:**

| | MoO1 | MoM |
|---|---:|---:|
| LBX files | 35 | 101 |
| stems of exactly 8 characters | 8 | 56 |
| files with a 0x200 name table | 26 | 79 |
| table entries / distinct names | 898 / 569 | 5,065 / 891 |
| names of exactly 8 characters | 399 of 893 | 3,040 of 4,886 |
| descriptions / of them all lower case | 136 / 133 | 1,562 / 1,201 |
| `NAME.LBX` literals in code / not upper case | 87 / 2 | 37 / 7 |
| bare-stem LBX literals in code | none confirmed | 197 |

- **The 8.3 squeeze is weaker in MoO1.** Only 8 of 35 file stems use all eight characters. Inside the name tables, 45% of entry names use all eight, against 62% in MoM.
- **Name-table style is the same in both games.** Entry names are upper case (891 of 893). Descriptions are rare and lower case, and typos stay in (`palete`, `gren`, `inssect`, `ameoba`), as §8 rule 6 expects.
- **Shared lineage is visible in the entries.** 11 entry names also occur in MoM's tables. Among them is `FONTS.LBX`'s `No File` / `EMPERATO` pair, so the font file format predates MoM.
- **Five stems occur in all three games:** `EVENTMSG`, `FONTS`, `HELP`, `INTRO` and `MUSIC`.
- **MoO1 loads LBX files by full name.** Its code passes `PLANETS.LBX`. MoM mostly passes the bare stem (`MAPBACK`), which suggests the MoM loader appends `.LBX` itself. MoO1 has two lower-case names, `missile.LBX` and `starview.LBX`, both in STARMAP overlays. With 2 of 107 file-name literals not upper case, the §5.4 case tests have no power in MoO1.
- **The string tables are new data.** `DIPLOMAT`, `EVENTMSG`, `NAMES` and `RESEARCH` are type-5 LBX files and `HELP` is type 3. Together they hold about 1,450 text fields, with the name-insertion control codes kept as `\xNN`. The help pages also cite the 15 art files their screens were made from (`MAINMOVE.FLI`, `NEWDEPLO.FLI`, …). These are SimTex working file names that never shipped.
- **ORION.EXE and STARMAP.EXE share code.** 108 strings occur in both EXEs (the race names, `CONFIG.MOO`, the `You must run ORION.EXE.` guard). That is the same two-EXE layout MoM uses for MAGIC / WIZARDS.

**What changes:** nothing in the authorship numbers. MoO1's case style is almost uniform, so it gives no module or programmer signal. Its value is as a dated baseline: September 1993, with the patch files dated October–December 1993. Habits found in both MoO1 and MoM are older than the MoM team split. That covers upper-case entry names, lower-case descriptions and unnormalized typos.

### 5.6 Starlords (May 1993 preview of MoO1)

[tools/simtex_style/simtex_starlords_corpus.py](../../tools/simtex_style/simtex_starlords_corpus.py) runs the same passes on the Starlords pitch / preview build (`/c/STU_DASM/_SL`). Every game file in it is dated 1993-05-07. Full numbers: [SimTex-Corpus-Starlords-Stats.md](SimTex-Corpus-Starlords-Stats.md).

**Starlords predates both formats MoO1 and MoM use:**

- **EXEs.** They are Microsoft C NE (segmented) executables behind a DOS stub loader. MoO1 and MoM are Borland C++ MZ executables with VROOMM overlays. [tools/simtex_style/ne_exe.py](../../tools/simtex_style/ne_exe.py) reads them through the same interface as the Borland reader.
  - The NE segment table gives exact code-segment boundaries.
  - The name tables are empty, so the EXEs carry no symbols.
  - 211 of 214 file-name literals are referenced from code.
- **LBX files.** There is no `0xFEAD` header and no 0x200 name table. An archive is a table of 8-byte slots (u32 offset, u32 0), and the last offset is the file size.
  - 24 files are archives, 4 are bare 320×200 animations and 4 are raw data.
  - **So the LBX entry names and descriptions (§5.2) were added between May and September 1993.**

**What Starlords adds:**

- **File names.** Even less squeezed than MoO1: 3 of 32 stems use all eight characters (MoO1 8 of 35, MoM 56 of 101).
  - 13 stems carry into MoO1, and 4 into all three games: `FONTS`, `INTRO`, `MUSIC` and `VORTEX`.
  - `DIPLOMAC`, `WIN` and `LOSE` are absent from MoO1 (which has `DIPLOMAT` and `WINLOSE`) but are back in MoM.
- **The `DIPLOMAC` → `DIPLOMAT` rename is proven by content.** 85 diplomacy messages carry over word for word. Other renames rest on the names alone (`NEBULAS` → `NEBULA`, `DESIGNS` → `DESIGN`, `WIN` + `LOSE` → `WINLOSE`) and are unconfirmed.
- **Text was carried over, then edited.** 221 of 449 Starlords text fields reappear in MoO1 unchanged apart from case, spacing and insertion codes. The edits made on the way are visible:
  - Tech names went from all upper case to capitalised words, mechanically: `ECM JAMMER MARK I` became `Ecm Jammer Mark I`. 126 of the 136 carried research fields changed this way.
  - The two leading spaces on 84 of 85 diplomacy messages went away.
  - `emperor` became `Emperor` in 11 messages.
  - The insertion codes moved from printable markers (`^`, `%`, `~`, `@`) to control bytes `\x80`–`\x83`.
- **EXE file-name literals:**
  - The EXE names are lower case (`vortex.exe`, `starlord.exe`, `files.exe`), and `Sound.LBX` is Title case. Everything else is upper case.
  - The program chain is START → `vortex.exe` → `starlord.exe` / `files.exe`.
  - Each use site stores its own copy of a literal (`STARMAP.LBX` ×39 in STARLORD), so this compiler did not pool identical strings.
- **422 EXE strings occur in both Starlords and MoO1.** Most are game data compiled into the program: weapon text, ship names, star names.

**What changes:** nothing in the authorship numbers. What it shows is when house style formed. In May 1993 the LBX format had no names, game text was upper case, and the compiler was Microsoft's. By September 1993 there were per-entry names, mixed-case text and Borland. The naming habits of §5.2 are later than Starlords.

### 5.7 MoO2 v1.31 game data

[tools/simtex_style/simtex_moo2_corpus.py](../../tools/simtex_style/simtex_moo2_corpus.py) runs the same passes on the MoO2 v1.31 install (`/c/STU_DASM/_MoO2v131`). Full numbers: [SimTex-Corpus-MoO2-Stats.md](SimTex-Corpus-MoO2-Stats.md).

**How the EXEs were read.** ORION2.EXE strings come from the IDA listing, which matches this EXE: 298 of 300 sampled strings occur in it byte for byte. Each labelled string carries the MoO2 module that references it and that module's owner (§6). ORION95.EXE, the Windows build, is read as raw bytes.

**The install.** The upper-case file names are the v1.31 patch files (both EXEs and the text LBX files, Dec 1996–Mar 1997). The lower-case names are the base install (1996-12-24). That case split is an artefact of how each set was copied, not a SimTex habit.

- **LBX names change shape.** 360 LBX files; 301 stems use all eight characters and 253 contain `_` (`tanm_001`, `sr_r0_sc`, `bldg0`). No earlier game uses `_` in an LBX name at all. Numbered families: `tanm_###` ×213, `sr_rN_sc` / `_sp` / `_tr` ×10 each.
  - `FONTS` is the only stem in all four games (Starlords, MoO1, MoM, MoO2).
- **The LBX name table is gone.** Every MoO2 archive has a 0x800-byte header and none has a name table (0 of 360). The bytes after the offset table are uninitialised memory. `fonts.lbx` still holds `ain palette` where a description field would be, plausibly residue of an older name table. So the timeline is:
  - May 1993 (Starlords): no names.
  - 1993–1994 (MoO1, MoM): 32-byte names and descriptions.
  - 1996 (MoO2): no names.

  This also answers §9 item 6: MoO2's `_seg` globals cannot have been named from LBX entry names.
- **Per-language slot files are copies.** The four `diplomsX` and four `eventmsX` files are each byte-identical: one English file per language slot.
- **MoO2 text was rewritten, not carried.** Only 12 English fields equal a MoO1 field (8 diplomacy messages, 4 report titles). Compare 221 Starlords → MoO1 (§5.6).

**The per-programmer string files** (§3.5) are the first per-person text in the corpus. English fields only; descriptive, since each file holds the text its owner's code prints:

| Owner | English fields | Mean words | `!` | printf `%s` / `%d` | control-byte codes |
|---|---:|---:|---:|---:|---:|
| Jim (`JIMTEXT`, `JIMTEXT2`) | 169 | 6.5 | 11% | 0% | 34% |
| Ken (`KENTEXT`, `KENTEXT1`) | 59 | 7.3 | 0% | 0% | 0% |
| Bill (`BILLTEXT`, `BILLTEX2`) | 23 | 8.5 | 35% | 0% | 65% |
| Eric (`ESTRINGS`) | 178 | 8.9 | 5% | 43% | 0% |
| Harold (`HESTRNGS`) | 174 | 11.0 | 13% | 57% | 0% |

- **The insertion codes split by person.** Counting all fields in every language, every file uses one style and never the other:

  | Insertion style | Files | Fields using it |
  |---|---|---|
  | Control bytes (`\x80`…), the form MoO1 used | Jim (`JIMTEXT`, `JIMTEXT2`) and Bill (`BILLTEXT`, `BILLTEX2`) | 51 + 170 and 61 + 5 |
  | printf placeholders | Eric (`ESTRINGS`), Harold (`HESTRNGS`) and Ken (`KENTEXT`) | 136, 152 and 27 |

  `KENTEXT1` has neither. That suggests two text paths, a control-code decoder and `sprintf`, chosen per programmer.
- **This is a lead, not evidence yet.** It is not part of the authorship model. §9 item 6 says how to check it at the code level.
- **The English table understates Ken's side.** His English fields are mostly short screen labels (`Pop:`), and his 27 printf fields are not among the ones tagged English.
- **Bill's row is only 23 fields.**

**EXE literals.** ORION2 has 197 `NAME.LBX` literals: 111 upper case and 80 lower case, against 2 lower case in MoO1. The case-by-owner table is in the stats page; §5.4 already showed case is not a personal mark in MoO2. ORION95 shares 336 of ORION2's distinct strings.

**What changes:** nothing in the authorship numbers. Three changes to the record:

- §9 item 6 is answered: there are no LBX names to borrow from.
- The LBX-name timeline is complete.
- The insertion-code split is a new candidate dialect family to test (§9).

---

## 6. The model, and how good it is

Evidence families are computed per MoO2 module. Each family is tempered so that one module counts as at most 12 observations.

| Family | What is counted | Likelihood |
|---|---|---|
| **dialect** | Personal-LBX references, and personal-loader calls not preceded by an LBX reference | The author's own dialect with p = 0.80, otherwise spread evenly |
| **helper** | Calls into another programmer's seed modules, string loaders excluded | Base-rate model: everyone calls everyone's utilities at the global rate, and the author calls their own 4× as often |
| **link block** | §2.3 | Reliability `q` **estimated** from agreement with the reference labels: (35+1)/(37+2) = **0.92** |
| *style* (reported, not used) | 16 binary name-shape features | Dropped. **0/13** in leave-one-out: in these seeds, name shape follows the topic, not the person. |

- **Prior:** the seven named programmers share 0.85 equally; **Other** gets 0.15. Other is the "someone else / can't tell" class. Conflicting evidence lands there, not on a confident wrong name.
- **Hard labels** override the posterior in the `FINAL` column:
  - **Seeds** (13 person-named files).
  - **Aggregator:** modules whose `Load_*_Strings` is called by `Load_Russ_Strings` (7) or by `Load_E_Strings` (1).

**Validation** (`--validate`):

| Check | Result | What it says |
|---|---|---|
| Dialect purity: non-seed modules with ≥ 5 dialect observations whose top dialect gets ≥ 80% | **29 / 30** | One module, one text owner. This does not depend on the seed labels. |
| Link block vs hard and strong-dialect labels | **35 / 37** | Two independent signals agree. The misses are `score.c` (Eric's dialect in Harold's list) and `multplay.c` (Harold's dialect in Russ's list). |
| Held out: Russ's 7 aggregator modules, before the hard override | model **4 / 7**, model + block **5 / 7** | Correct calls had P ≥ 0.78; misses had P ≤ 0.61. The block result is only semi-independent, because these modules are in the set `q` was estimated on. |
| Dialect vs helper top author, both ≥ 5 observations | 14 / 25 | Chance is about 1 / 7. Helper is weak evidence. |
| Leave one seed module out | 2 / 13 | Seed files are mostly utility code with no text, so the strong signals are absent from the files this test can check. Don't read it as accuracy on screen modules. |

**How to read `P_FINAL`:**

- **≥ 0.95 with dialect or a hard label:** treat as established.
- **0.75–0.95:** likely.
- **0.78 with no dialect:** the list position alone, nothing more.
- **< 0.6:** unknown.

---

## 7. ReMoM map

### 7.1 Coverage

[SimTex-Authorship-ReMoM-Functions.csv](SimTex-Authorship-ReMoM-Functions.csv) lists **2,092** function definitions parsed from [MoM/src/](../../MoM/src/) and [MoX/src/](../../MoX/src/). **401** of them match a MoO2 module:

- **314** by exact function name.
- **87** by a `// MoO2 Module: X` comment in the header block.

55 of the 114 files have no match.

### 7.2 Circularity

ReMoM names that equal MoO2 names were mostly given **because of** MoO2. A name match records someone's judgement that it is the same function. It is not independent evidence, and ReMoM's own file names (`AIDUDES.c`, `CMBTAI.c`, ...) are not evidence at all.

### 7.3 Projection onto MoM

MoM's programmers are Jim, Ken, Steve and Grissel. Only Jim and Ken have MoO2 modules.

- If the MoO2 owner is Jim or Ken, the same person wrote the MoM ancestor with τ = **0.6**. This is an assumption; tune `TRANSFER_TAU`.
- Otherwise the MoO2 owner says nothing, and the MoM author falls back to a uniform prior over the four.

| ReMoM file | Matched | Main MoO2 modules | MoO2 owner | MoM author (projected) |
|---|---|---|---|---|
| [Combat.c](../../MoM/src/Combat.c) | 16 / 204 | `combat1`, `combinit`, `combat`, `combfind` | Ken 0.80, Russ 0.14 | **Ken 0.61**, others 0.13 each |
| [COMBINIT.c](../../MoM/src/COMBINIT.c) | 4 / 12 | `combinit` | Ken 0.68 | **Ken 0.55** |
| [EVENTS.c](../../MoM/src/EVENTS.c) | 5 / 7 | `events` | Jim 1.00 | **Jim 0.70** |
| [DIPLOMAC.c](../../MoM/src/DIPLOMAC.c) | 26 / 55 | `diplomac` 14, `dip-scrn` 9, `npcdiplo` 3 | Jim 0.44, Ken 0.42 | Jim 0.39, Ken 0.37 |
| [INITGAME.c](../../MoM/src/INITGAME.c) | 5 / 15 | `diplomac`, `initgame`, `report` | Russ 0.54, Ken 0.33 | Ken 0.40 |
| [NEXTTURN.c](../../MoM/src/NEXTTURN.c) | 11 / 56 | `nextturn`, `colcalc` | Russ 0.46, Eric 0.36 | flat, no information |
| [CITYCALC.c](../../MoM/src/CITYCALC.c), [CityScr.c](../../MoM/src/CityScr.c), [City_ovr55.c](../../MoM/src/City_ovr55.c) | 5–6 each | `colcalc`, `colony`, ... | Eric / Russ | flat, no information |
| [MoX/src/](../../MoX/src/): `Fonts.c`, `Fields.c`, `FLIC_Draw.c`, `Mouse.c`, `Video2.c`, ... | high (34 / 45 in `Fields.c`) | WLIBS | Other | flat, no information. The 0.29 / 0.21 split is the prior, not evidence. |

**What this supports:**

- **Combat → Ken.** Ken is the only programmer on all three games, and the strongest single MoO2 signal (`KENTEXT` in every combat module) points at him.
- **Events and the diplomacy screens → Jim.**
- Everything else in MoM stays open, including the engine library. The MoO2 owners of city and next-turn code (Eric, Russ) were not on the MoM team.

---

## 8. Naming heuristics for unnamed MoM and MoO1 code

1. **Same function in MoO2 → take the MoO2 name verbatim,** decoration stripped (§3.1). Record it as a `// MoO2 Module: <module> <Name>` header line, verified by grep.
2. **A MoO2 sibling family exists → extend the family's pattern** instead of inventing a new one:
   - `Get_Weighted_Choice_*`
   - `Draw_*` / `*_To_Bitmap`
   - `<X>_Screen` / `Load_<X>_Screen_Pictures` / `Reload_<X>_Screen`
   - `Set_<X>_Help_List`
   - `N_<things>`
   - `<Owner>_Has_<Thing>`
   - `<A>_From_<B>`
3. **Use the subsystem owner's vocabulary.** Combat code takes Ken's (`Seg`, `Weighted`, ordinal / type converters such as `Convert_Ordinal_To_Msl_Type`). Diplomacy and event text takes Jim's (`Decode_Text_*`, `Get_Text_Message`). Accessors take Harold's (noun-first, `Is` / `Has` / `N_`).
4. **Globals:** `lower_snake_case`, subsystem prefix first, role suffix last. Use `_seg` for a loaded picture, `_button` / `_field` for a hot spot, `_flag` for a boolean, `n_` or `_count` for a count, `cur_` / `old_` / `last_` for state history. Write upper case only for LBX-name globals and constant-like globals.
5. **Don't use:**
   - CamelCase.
   - Hungarian notation.
   - Library-style prefixes. `CMB_`, `VGA_`, `NEWG_` and similar are drake178's IDA labels, not SimTex.
   - Abbreviations outside the attested set (§3.3).
   - Mode X. MoM is **VGA Mode Y**.
6. **Don't "fix" attested oddities when you borrow a name** (`Segk`, `Loose`, `_to_Bitm`). They show the names are not normalized. Don't invent new oddities either.
7. **Shipped names follow 8.3 rules, code names don't (§5).** LBX file and entry names are upper case and squeezed to eight characters, each cut made individually (`HLBRD`, `HALBE`). Code identifiers are spelled out (§3.3). Don't carry 8.3 cuts into function names. When a variable holds a loaded LBX record, the LBX entry name and description (`MAINSCRN` / `BUTTONS2` "Continue button") are the closest thing to a SimTex label for it. Quote them in the header comment.
8. **Name ReMoM files by OG overlay boundaries where possible (§5).** One overlay is one OG source file. A ReMoM file that spans several overlays ([Combat.c](../../MoM/src/Combat.c): 7) is a merge, and any OG-faithful file split would follow the overlays.
9. **MoO1** has no symbols and only 1oom names. Those are not authority: 1oom is a MoO1 remake, not SimTex and not drake178. Naming MoO1 code means going through MoM / MoO2 equivalents with the same rules. The candidate authors are Stephen Barcia, Maria Barcia and Ken Burd.

### 8.1 Scoring ReMoM's function names against these rules

[tools/simtex_style/simtex_name_score.py](../../tools/simtex_style/simtex_name_score.py) turns §3.3 and the heuristics above into a number for every ReMoM function name.

- **Features:** token count, length, first-token class (verb / noun / `Is` / `N_` / all-caps prefix / lower case), verb in last place, worst token case, unattested abbreviations, digits, underscores. They are shape features only, so MoM's subject words (`Spell`, `Lair`) cost nothing.
- **Learning:** each feature's probability is learned from the 3,345 MoO2 SimTex code names. A name's surprisal is the sum of −log2 P.
- **Score:** the percent of MoO2 names at least as surprising. 50 is a typical SimTex name; 2 means only 2% of real SimTex names are this odd.
- **Not scored:** ReMoM markers (`__WIP`, `__STUB`, `UU_`, …) are stripped first. Miles `AIL_*`, AIL demo code and C-library shims are listed but not scored. A name identical to a MoO2 name is scored but exempt, because it *is* a SimTex name.

| Group | Names | Median score | Share below 5 |
|---|---|---|---|
| MoO2 names themselves (self-check) | 3,345 | 53.4 | 4.6% |
| ReMoM names identical to a MoO2 name | 326 | 61.8 | 2.1% |
| ReMoM reconstructions of OG functions (`// WZD` / `// MGC` header) | 1,513 | 34.2 | 13.6% |
| ReMoM-only code (platform, debug, tests) | 185 | 13.3 | 21.6% |

The MoO2 names were also the training data, so the first two rows are a sanity check, not a held-out test.

**The bottom 20%** is 340 names, all scoring ≤ 7.8. The most common reasons:

| Count | Reason | Examples |
|---|---|---|
| 145 | all-caps label prefix | `AITP_`, `EMM_`, `WIZ_`, `UV_` |
| 117 | CamelCase | `EMM_GetHandle`, `WIZ_MeteorStorm` |
| 79 | over 26 characters | 43 of them over Borland's 31 |
| 51 | lower-case names | `gd_dump_*`, `gfopen` |
| 50 | verb last | `ArmyList_Screen_Draw` |
| 50 | digits inside the name | `oNNNpNN_empty_function` |

**Proposals** are in [SimTex-Name-Renames-Proposed.fwv](SimTex-Name-Renames-Proposed.fwv). The tool joins them into [SimTex-Name-Scores-ReMoM-Functions.fwv](SimTex-Name-Scores-ReMoM-Functions.fwv) and rescores them.

- **283 renames.** The median score moves from 2.2 to 26.0.
- **28 keeps,** where the flagged shape is itself a MoO2 pattern: `LBX_Load`, `Clipped_*`, `AI_Spell_Select` (as `AI_Tech_Select`), and the `moves2` half-move names.
- **29 with no proposal:** address placeholders and empty OG functions, whose only identity is their segment address.
- **47 family rows** outside the bottom group, so a prefix family (`AITP_*`, `*_NewGame`, `ArmyList_*`) is renamed as a whole or not at all.
- **Checks:** every proposal is ≤ 31 characters, no two proposals are the same, and none collides with an identifier already in the code (comments excluded).

**One deliberate low scorer.** The `EMM_` / `EMS_` / `FILE_` proposals keep an upper-case module prefix, so they still score about 2. MoO2's WLIBS uses exactly that shape for engine modules (`LBX_Open_File`, `_FILE_read`), and the scorer can't tell a WLIBS module prefix from a drake178 label.

---

## 9. What else would help

Highest value first.

1. **Ask the authors.** Permission to publish has already been settled with the original authors and copyright owners, so a short question list may be possible:
   - Who wrote MoM's combat, diplomacy, AI, city and engine code?
   - Is Grissel Barcia the same person as Maria Barcia?
   - What does "WLIBS" stand for?
   - Were the makefile object lists per person and alphabetical?
   - What are the surnames of Harold, Eric, Russ, Bill and Evan?

   One answer can confirm or overturn whole blocks of this model.
2. **MoO2 credits.** Read them from the game's credits LBX or the manual to pin surnames, and to confirm Jim = Cowlishaw and Ken = Burd.
3. **MoM link order.** MoM's overlay order is link order, and one overlay is one OG source file (§5).
   - **Done:** the file-name literals show contiguous blocks with boundaries near overlays 103/104 and 140/143 (§5.4).
   - **Still open:** if SimTex's MoM makefile lists were per person and alphabetical (as in MoO2), any candidate set of OG file names must sort in runs within each block. Ask the authors whether 104–140 was one person's list.
4. **Strings in MoM and MoO1.**
   - **Done for MoM file-name strings (§5.3).** Case is not a personal mark in MoO2, so this did not give MoM a dialect family.
   - **Still to do:**
     - Message text in `message.lbx` / `HELP.LBX` / `HLPENTRY.LBX` and the EXEs' own error strings, looked at for personal phrasing.
     - Any per-person text file. MoM has none of the `jimtext` / `KENTEXT` kind among its 101 LBX names. MoO2's per-person files are profiled in §5.7.
5. **MoO1 corpus.**
   - **Done for v1.30 (§5.5):** file names, LBX name tables, LBX string tables and EXE strings.
   - **Done for the Starlords preview (§5.6),** with a text-level comparison against v1.30.
   - **Still open:** `/c/STU_DASM` also holds `MOO1008`, `MOOV12` and `MOOV13`. Diffing their strings and LBX tables against v1.30 would date each change, the way the MoM demo comparison did.
   - **Still open:** the MoO1 string tables (§5.5) are the place to look for personal phrasing, the way item 4 proposes for MoM's `message.lbx` / `HELP.LBX`.
6. **MoO2 LBX tables.**
   - **Answered (§5.7):** MoO2's LBX files have no name tables (0 of 360), so its `_seg` globals cannot have been named from LBX entry names. MoM's entry names (§5.2) stay heuristic name sources only.
   - **New lead:** the insertion-code split in the personal string files, control bytes for Jim / Bill and printf for Eric / Harold / Ken. Check it at the call sites in `ORION2.asm`: who passes strings to `sprintf` and who to the control-code decoder. If it holds at code level, it is a dialect evidence family the model (§6) does not yet use.
7. **ITEMMAKE.EXE.** It ships with MoM and names its own files with odd case (`Itemdata.LBX`, `SPEcial2.LBX`, `special2.LBX`), but there is no IDA export for it. Export it and run the §5.3 pass.
8. **Match by behaviour, not by name.** Compare call graphs, constants and string uses between MoM procs and MoO2 procs (a BinDiff-like method on decompiled pseudo-C). That replaces the circular name matches of §7.2 with independent evidence and raises coverage above 401 / 2,092.
9. **Per-function dialect** inside big mixed modules (`colony.c`, 107 functions; `dip-scrn.c`, 84). The tool scores whole modules today. Proc-level counts would show shared files.
10. **Compiler-independent code-shape stylometry**, trained on the 125 MoO2 game modules that now have a named owner: switch vs. if-chain habits, early returns, constant habits, helper choice (`Random`, weighted choice), frame size. Then apply it to MoM. Only features that survive Watcom → Borland are usable.
11. **More held-out labels to calibrate against.** Use other aggregators like `Load_Russ_Strings`, the callers of each `Set_*_Help_List`, and `MOX.C`'s 734-global order, which may group by programmer.
12. **Drive letters as a weak signal.** `L:` vs `D:` vs `E:` / `F:` say which machine built an object. Pair them with build timestamps if any other MoO2 build (patch, demo, other language) carries debug info. Diffing two symbol sets would also show who changed what late in development.
