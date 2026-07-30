
# =========================================================================
#  Standalone Tools
#
#  Small CLI utilities that don't depend on the game engine.  These can be
#  run against save files produced by any version of Master of Magic (OG,
#  ReMoM, HeMoM).
# =========================================================================


# --- savedump: dump a .GAM save file to structured text ---

add_executable(savedump
    savedump.c
    ${PROJECT_SOURCE_DIR}/src/Game_Save_Dump.c
)

target_include_directories(savedump PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(savedump PRIVATE STU)

set_target_properties(savedump PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# --- rmr2hms: translate a .RMR replay recording to a .hms scenario script ---

add_executable(rmr2hms
    rmr2hms.c
)

set_target_properties(rmr2hms PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# --- lbx_hashes: generate the compiled-in known-good LBX SHA-256 manifest ---
# Point it at a Master of Magic install dir + version tag; it prints C rows for
# STU/src/lbx_manifest.c.  See that file's header for the workflow.

add_executable(lbx_hashes
    lbx_hashes.c
)

target_link_libraries(lbx_hashes PRIVATE STU)

set_target_properties(lbx_hashes PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# --- lbx_extract: extract LBX archive entries to standalone files ---
# Standalone container reader; no engine dependencies.  Strips the 16-byte
# 0xDEAF sound wrapper so MUSIC.LBX / SOUNDFX.LBX entries come out as plain
# .XMI / .VOC that external tools (wildmidi, etc.) accept.
#   lbx_extract --list assets/MUSIC.LBX
#   lbx_extract -o out assets/MUSIC.LBX 0     -> out/MUSIC_000.XMI

add_executable(lbx_extract
    lbx_extract.c
)

set_target_properties(lbx_extract PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# --- gtl2op2: Miles/AIL Global Timbre Library -> DMX GENMIDI (.op2) FM bank ---
# Transcodes FAT.OPL / FAT.AD into a bank WildMIDI can load, so MoM's own
# timbres voice the music instead of generic General MIDI.
#   gtl2op2 assets/__FAT/FAT.OPL out/xmi/fatman.op2
#   wildmidi -c out/xmi/fatman.op2 -o track.wav out/xmi/MUSIC_100.XMI

add_executable(gtl2op2
    gtl2op2.c
)

set_target_properties(gtl2op2 PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)


# --- Extract_FONTS_LBX: extract FONTS.LBX data into embeddable C source ---
# cd ~/STU/devel/ReMoM/ && cmake --build build --target Extract_FONTS_LBX
# cd ~/STU/devel/ReMoM/build/bin/Debug && ./Extract_FONTS_LBX
# The tool is now at build/bin/Debug/Extract_FONTS_LBX. To generate the real data, run it from the directory where FONTS.LBX lives and copy the output to MoX/src/Init_Data/FONTS_LBX.c.
# jbalcomb@iMustAi:~/STU/devel/ReMoM/assets$ ../build/bin/Debug/Extract_FONTS_LBX 
# >>> Extract_FONTS_LBX: wrote FONTS_LBX.c
# jbalcomb@iMustAi:~/STU/devel/ReMoM/assets$ mv FONTS_LBX.c ../MoX/src/Init_Data/
add_executable(Extract_FONTS_LBX Extract_FONTS_LBX.c)

target_include_directories(Extract_FONTS_LBX PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(Extract_FONTS_LBX PRIVATE
    STU
    MOX
    momlib
    Platform_Headless
)

set_target_properties(Extract_FONTS_LBX PROPERTIES POSITION_INDEPENDENT_CODE ON)


# --- remom_video_probe: process-per-attempt SDL2 graphics-stack probe ---
#  Diagnoses "crash on startup in SDL_CreateRenderer" reports (GLX-over-XWayland on Linux,
#  a faulting D3D/OpenGL ICD on Windows) by testing every video x render driver combo in
#  its own child process, so a fatal exit can't take the probe down.  Also measures
#  present-cadence jitter for "choppy framerate" reports (--timing).
#  See doc/#Devel/Devel-Graphics.md.
#
#  Isolation is fork() on UNIX and spawn-self on Windows, so the only hard requirement is
#  SDL2 itself.  Skipped under SDL3 (USE_SDL3=ON): the probe is deliberately written
#  against the SDL2 API that ReMoM's shipping backend uses.
if(TARGET SDL2::SDL2)
    add_executable(remom_video_probe remom_video_probe.c)
    target_link_libraries(remom_video_probe PRIVATE SDL2::SDL2)
    if(UNIX)
        target_link_libraries(remom_video_probe PRIVATE m)  # m: sqrt() in the timing stats
    endif()
    if(WIN32)
        # dxgi/dxguid: adapter enumeration (which GPU is faulting -- SDL2 exposes none of it).
        # winmm: timeBeginPeriod, so the paced --timing rows measure the display rather than
        # Windows' default ~15.6 ms sleep granularity.
        target_link_libraries(remom_video_probe PRIVATE dxgi dxguid winmm)
    endif()
    set_target_properties(remom_video_probe PROPERTIES POSITION_INDEPENDENT_CODE ON)

    # Windows ships the probe INSIDE the normal package: the ZIP/NSIS already carries
    # ReMoM_diagnostic.exe and SDL2.dll (see the root CMakeLists), so folding in a ~100 KB
    # probe costs nothing and spares Windows testers a separate download.  Linux keeps its
    # own diagnostics zip instead, because its diagnostic build is NOT in the main package.
    if(MSVC)
        install(TARGETS remom_video_probe RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
