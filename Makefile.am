AUTOMAKE_OPTIONS = foreign subdir-objects

bin_PROGRAMS = ReMoMber

# =========================================================================
#  Compiler flags
# =========================================================================

AM_CFLAGS = -Wall -Wno-array-bounds -Wno-maybe-uninitialized -Wno-missing-braces -Wno-sequence-point -Wno-unknown-pragmas -Wno-unused-function -Wno-unused-label  -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-variable

# =========================================================================
#  Convenience libraries (noinst = not installed, just linked into the exe)
# =========================================================================

noinst_LIBRARIES = \
    libstu.a \
    libplatform.a \
    libmox.a \
    libmom.a

# -------------------------------------------------------------------------
#  STU
# -------------------------------------------------------------------------

libstu_a_SOURCES = \
    STU/src/STU_DBG.c \
    STU/src/STU_DRAW.c \
    STU/src/STU_EMS.c \
    STU/src/STU_FMT.c \
    STU/src/STU_INT.c \
    STU/src/STU_JSON.c \
    STU/src/STU_MSG.c \
    STU/src/STU_PRN.c \
    STU/src/STU_SND.c \
    STU/src/STU_TST.c \
    STU/src/STU_UTIL.c \
    STU/src/STU_VLD.c \
    STU/src/STU_WAV.c \
    STU/src/STU_WRLD.c \
    ext/stu_compat.c

libstu_a_CFLAGS = $(AM_CFLAGS) \
    -I$(srcdir)/STU/src

# -------------------------------------------------------------------------
#  Platform
# -------------------------------------------------------------------------

if USE_SDL3
PLATFORM_CORE_SOURCES = \
    platform/sdl3/sdl3_EMM.c \
    platform/sdl3/sdl3_Init.c \
    platform/sdl3/sdl3_Input.c \
    platform/sdl3/sdl3_KD.c \
    platform/sdl3/sdl3_MD.c \
    platform/sdl3/sdl3_PFL.c \
    platform/sdl3/sdl3_SDLK.c \
    platform/sdl3/sdl3_State.c \
    platform/sdl3/sdl3_Timer.c \
    platform/sdl3/sdl3_Video.c \
    platform/replay/Replay.c
PLATFORM_BACKEND_DIR = platform/sdl3
else
PLATFORM_CORE_SOURCES = \
    platform/sdl2/sdl2_EMM.c \
    platform/sdl2/sdl2_Init.c \
    platform/sdl2/sdl2_Input.c \
    platform/sdl2/sdl2_KD.c \
    platform/sdl2/sdl2_MD.c \
    platform/sdl2/sdl2_PFL.c \
    platform/sdl2/sdl2_SDLK.c \
    platform/sdl2/sdl2_State.c \
    platform/sdl2/sdl2_Timer.c \
    platform/sdl2/sdl2_Video.c \
    platform/replay/Replay.c
PLATFORM_BACKEND_DIR = platform/sdl2
endif

if HAVE_AUDIO
if USE_SDL3
PLATFORM_AUDIO_SOURCES = \
    platform/sdl3/sdl3_Audio.c
else
PLATFORM_AUDIO_SOURCES = \
    platform/sdl2/sdl2_Audio.c
endif
else
PLATFORM_AUDIO_SOURCES =
endif

libplatform_a_SOURCES = \
    $(PLATFORM_CORE_SOURCES) \
    $(PLATFORM_AUDIO_SOURCES)

libplatform_a_CFLAGS = $(AM_CFLAGS) \
    $(SDL_CFLAGS) \
    -I$(srcdir)/platform/include \
    -I$(srcdir)/$(PLATFORM_BACKEND_DIR)

if HAVE_AUDIO
libplatform_a_CFLAGS += $(SDL_MIXER_CFLAGS)
endif

# -------------------------------------------------------------------------
#  MoX
# -------------------------------------------------------------------------

libmox_a_SOURCES = \
    MoX/src/AIL.c \
    MoX/src/Allocate.c \
    MoX/src/angle.c \
    MoX/src/capture.c \
    MoX/src/CFG.c \
    MoX/src/DOS.c \
    MoX/src/EMM.c \
    MoX/src/EXIT.c \
    MoX/src/Fields.c \
    MoX/src/file_ani.c \
    MoX/src/Fonts.c \
    MoX/src/GENDRAW.c \
    MoX/src/GFILE.c \
    MoX/src/Graphics.c \
    MoX/src/FLIC_Draw.c \
    MoX/src/Help.c \
    MoX/src/Input.c \
    MoX/src/IO_Buffer.c \
    MoX/src/Keyboard.c \
    MoX/src/LBX_Load.c \
    MoX/src/LOADSAVE.c \
    MoX/src/Mouse.c \
    MoX/src/MOM_DAT.c \
    MoX/src/MOX_DAT.c \
    MoX/src/MOX_SET.c \
    MoX/src/MOX_SND.c \
    MoX/src/MOX_T4.c \
    MoX/src/paragrph.c \
    MoX/src/random.c \
    MoX/src/SOUND.c \
    MoX/src/special.c \
    MoX/src/TEXTBOX.c \
    MoX/src/Util.c \
    MoX/src/Video.c \
    MoX/src/Video2.c

libmox_a_CFLAGS = $(AM_CFLAGS) \
    $(SDL_CFLAGS) \
    -I$(srcdir)/MoX/src \
    -I$(srcdir)/platform/include

# -------------------------------------------------------------------------
#  MoM
# -------------------------------------------------------------------------

libmom_a_SOURCES = \
    MoM/src/AdvsrScr.c \
    MoM/src/AIBUILD.c \
    MoM/src/AIDATA.c \
    MoM/src/AIDUDES.c \
    MoM/src/AIMOVE.c \
    MoM/src/AISPELL.c \
    MoM/src/ALLOC.c \
    MoM/src/ArmyList.c \
    MoM/src/CITYCALC.c \
    MoM/src/CityList.c \
    MoM/src/CITYSCAP.c \
    MoM/src/CityScr.c \
    MoM/src/City_ovr55.c \
    MoM/src/CMBTAI.c \
    MoM/src/Combat.c \
    MoM/src/CONQUEST.c \
    MoM/src/CREDITS.c \
    MoM/src/DIPLOMAC.c \
    MoM/src/EVENTS.c \
    MoM/src/Explore.c \
    MoM/src/HIRE.c \
    MoM/src/Init.c \
    MoM/src/INITGAME.c \
    MoM/src/INTRO.c \
    MoM/src/ItemMake.c \
    MoM/src/Items.c \
    MoM/src/ItemScrn.c \
    MoM/src/ItemView.c \
    MoM/src/Lair.c \
    MoM/src/LOADER.c \
    MoM/src/LoadScr.c \
    MoM/src/LVLMAKE.c \
    MoM/src/MagicScr.c \
    MoM/src/MainMenu.c \
    MoM/src/MainScr.c \
    MoM/src/MainScr_Maps.c \
    MoM/src/MAPGEN.c \
    MoM/src/MGC_DATA.c \
    MoM/src/MOM_SCR.c \
    MoM/src/MOM_DBG.c \
    MoM/src/MovePath.c \
    MoM/src/NewGame.c \
    MoM/src/NEXTTURN.c \
    MoM/src/Outpost.c \
    MoM/src/OverSpel.c \
    MoM/src/ProdScr.c \
    MoM/src/RACETYPE.c \
    MoM/src/REPORT.c \
    MoM/src/Roads.c \
    MoM/src/SBookScr.c \
    MoM/src/SCastScr.c \
    MoM/src/SCORE.c \
    MoM/src/Settings.c \
    MoM/src/SETTLE.c \
    MoM/src/Spellbook.c \
    MoM/src/Spells128.c \
    MoM/src/Spells129.c \
    MoM/src/Spells130.c \
    MoM/src/Spells131.c \
    MoM/src/Spells132.c \
    MoM/src/Spells133.c \
    MoM/src/Spells137.c \
    MoM/src/SPLMASTR.c \
    MoM/src/Surveyor.c \
    MoM/src/Terrain.c \
    MoM/src/UnitList.c \
    MoM/src/UnitMove.c \
    MoM/src/UnitStat.c \
    MoM/src/UNITSTK.c \
    MoM/src/UNITTYPE.c \
    MoM/src/UnitView.c \
    MoM/src/WIZVIEW.c \
    MoM/src/WZD_o059.c \
    MoM/src/WZD_o143.c \
    MoM/src/WZD_o146.c

libmom_a_CFLAGS = $(AM_CFLAGS) \
    $(SDL_CFLAGS) \
    -I$(srcdir)/MoM/src \
    -I$(srcdir)/MoX/src \
    -I$(srcdir)/platform/include \
    -I$(srcdir)/src

# =========================================================================
#  ReMoMber executable
# =========================================================================

ReMoMber_SOURCES = \
    src/ReMoM.c

ReMoMber_CFLAGS = $(AM_CFLAGS) \
    $(SDL_CFLAGS) \
    -I$(srcdir)/MoX/src \
    -I$(srcdir)/MoM/src \
    -I$(srcdir)/platform/include \
    -I$(srcdir)/STU/src \
    -I$(srcdir)/src

ReMoMber_LDADD = \
    libmom.a \
    libmox.a \
    libplatform.a \
    libstu.a \
    $(SDL_LIBS)

if HAVE_AUDIO
ReMoMber_LDADD += $(SDL_MIXER_LIBS)
endif

ReMoMber_LDADD += -lm
