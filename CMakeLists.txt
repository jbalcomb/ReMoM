cmake_minimum_required(VERSION 3.15)

# --- Release version (single source of truth) ---
# The full version string is supplied by the release pipeline from the git tag
# (e.g. tag v0.5.0 -> -DREMOM_VERSION=0.5.0).  Local / dev builds fall back to a
# placeholder.  CMake's project(VERSION) needs a clean numeric MAJOR.MINOR.PATCH,
# so we derive that from the (possibly suffixed) full string while keeping the
# full string for artifact names and the in-app version banner.
set(REMOM_VERSION "0.0.0-dev" CACHE STRING "Release version (set from the git tag by CI)")

string(REGEX MATCH "^[0-9]+\\.[0-9]+\\.[0-9]+" REMOM_VERSION_NUMERIC "${REMOM_VERSION}")
if(NOT REMOM_VERSION_NUMERIC)
    set(REMOM_VERSION_NUMERIC "0.0.0")
endif()

project(
    ReMoM
    VERSION "${REMOM_VERSION_NUMERIC}"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C
)

# Generate a header carrying the full version string for the in-app banner.
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/remom_version.h.in"
    "${CMAKE_BINARY_DIR}/generated/remom_version.h"
    @ONLY
)
include_directories("${CMAKE_BINARY_DIR}/generated")

# GoogleTest requires at least C++17
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# noworkie?  set(ENV{CTEST_OUTPUT_ON_FAILURE} ON)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
# set(ENV{CTEST_PARALLEL_LEVEL} 2)
# list(APPEND CMAKE_CTEST_ARGUMENTS "--parallel 2")
include(CTest)

# Valgrind memcheck support — Linux / GCC / Clang only
option(ENABLE_VALGRIND "Run CTest memcheck pass under Valgrind (Linux GCC/Clang only)" OFF)
if(ENABLE_VALGRIND)
    if(WIN32)
        message(WARNING "ENABLE_VALGRIND is not supported on Windows; ignoring.")
    else()
        find_program(VALGRIND_EXECUTABLE valgrind)
        if(VALGRIND_EXECUTABLE)
            set(MEMORYCHECK_COMMAND "${VALGRIND_EXECUTABLE}")
            set(MEMORYCHECK_COMMAND_OPTIONS
                "--error-exitcode=1 --leak-check=full --track-origins=yes --show-leak-kinds=all")
            message(STATUS "Valgrind: ${VALGRIND_EXECUTABLE}")
        else()
            message(WARNING "ENABLE_VALGRIND is ON but valgrind was not found in PATH.")
        endif()
    endif()
endif()

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable FetchContent update checks" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED_GOOGLETEST ON CACHE BOOL "Disable googletest update checks" FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    EXCLUDE_FROM_ALL
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Don't pull googletest's install rules into our package — they only matter at dev/test time.
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Custom Find modules for SDL2 (from chocolate-doom)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# SDL backend selection: SDL2 (default) or SDL3 (opt-in with -DUSE_SDL3=ON)
# https://wiki.libsdl.org/SDL3/README/cmake
option(USE_SDL3 "Use SDL3 instead of SDL2 (default: SDL2)" OFF)

option(USE_WIN32 "Use native Win32 backend instead of SDL (no SDL dependency)" OFF)

if(USE_WIN32)
    add_compile_definitions(NO_SOUND_LIBRARY)
elseif(USE_SDL3)
    if(WIN32)
        list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3-3.4.2")
    endif()
    find_package(SDL3 REQUIRED)
    if(NOT DISABLE_AUDIO)
        if(WIN32)
            list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3_mixer-3.2.0")
        endif()
        find_package(SDL3_mixer REQUIRED)
    else()
        add_compile_definitions(NO_SOUND_LIBRARY)
    endif()
else()
    if(WIN32)
        list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL2-2.32.2")
        set(SDL2_DIR "C:/devellib/SDL2-2.32.2" CACHE PATH "" FORCE)
    endif()
    find_package(SDL2 REQUIRED)
    if(NOT DISABLE_AUDIO)
        if(WIN32)
            list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL2_mixer-2.8.1")
            set(SDL2_MIXER_DIR "C:/devellib/SDL2_mixer-2.8.1" CACHE PATH "" FORCE)
        endif()
        find_package(SDL2_mixer REQUIRED)
    else()
        add_compile_definitions(NO_SOUND_LIBRARY)
    endif()
endif()

# All executables and DLLs go to one directory (per-config on multi-config generators).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# Single asset-staging step.  All executables (HeMoM, ReMoM, savedump,
# rmr2hms, tests, ...) share the same bin/$<CONFIG> directory, so the asset
# tree is copied there exactly once per build instead of being copied
# redundantly by each executable's POST_BUILD.
add_custom_target(copy_assets ALL
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "Staging game assets to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    VERBATIM
)

# Copy SDL runtime DLLs to both Debug and Release output dirs at configure time.
if(WIN32 AND NOT USE_WIN32)
    if(USE_SDL3)
        set(_sdl_target SDL3::SDL3)
        set(_mixer_target SDL3_mixer::SDL3_mixer)
    else()
        set(_sdl_target SDL2::SDL2)
        set(_mixer_target SDL2_mixer::SDL2_mixer)
    endif()
    get_target_property(_sdl_imported_loc ${_sdl_target} IMPORTED_LOCATION)
    if(NOT _sdl_imported_loc)
        get_target_property(_sdl_imported_loc ${_sdl_target} IMPORTED_LOCATION_RELEASE)
    endif()
    # On MSVC the imported target's IMPORTED_LOCATION is the .lib import library;
    # the runtime .dll lives beside it. Resolve the DLL so we bundle SDL2.dll (what
    # the exe needs to launch), not SDL2.lib (link-only).
    if(_sdl_imported_loc MATCHES "\\.lib$")
        get_filename_component(_sdl_dir "${_sdl_imported_loc}" DIRECTORY)
        get_filename_component(_sdl_we "${_sdl_imported_loc}" NAME_WE)
        unset(_sdl_dll CACHE)
        find_file(_sdl_dll NAMES "${_sdl_we}.dll" HINTS "${_sdl_dir}" NO_DEFAULT_PATH)
        if(_sdl_dll)
            set(_sdl_imported_loc "${_sdl_dll}")
        endif()
    endif()
    if(_sdl_imported_loc)
        file(COPY "${_sdl_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
        file(COPY "${_sdl_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
    endif()
    if(NOT DISABLE_AUDIO AND TARGET ${_mixer_target})
        get_target_property(_mixer_imported_loc ${_mixer_target} IMPORTED_LOCATION)
        if(NOT _mixer_imported_loc)
            get_target_property(_mixer_imported_loc ${_mixer_target} IMPORTED_LOCATION_RELEASE)
        endif()
        if(_mixer_imported_loc MATCHES "\\.lib$")
            get_filename_component(_mixer_dir "${_mixer_imported_loc}" DIRECTORY)
            get_filename_component(_mixer_we "${_mixer_imported_loc}" NAME_WE)
            unset(_mixer_dll CACHE)
            find_file(_mixer_dll NAMES "${_mixer_we}.dll" HINTS "${_mixer_dir}" NO_DEFAULT_PATH)
            if(_mixer_dll)
                set(_mixer_imported_loc "${_mixer_dll}")
            endif()
        endif()
        if(_mixer_imported_loc)
            file(COPY "${_mixer_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
            file(COPY "${_mixer_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
        endif()
    endif()
endif()

add_compile_definitions($<$<CONFIG:Debug>:STU_DEBUG>)

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # add_compile_options(/wd4068)
    add_compile_options(
        "/W4"
        "/WX"
        # DONT  "/wd4013"  # 'stu_strcat' undefined; assuming extern returning int
        # DONT  "/wd4057"  # '=': 'uint8_t *' differs in indirection to slightly different base types from 'int8_t *'
        "/wd4100"  # unreferenced parameter
        "/wd4101"  # unreferenced local variable
        "/wd4127"  # conditional expression is constant
        "/wd4189"  # local variable is initialized but not referenced
        "/wd4201"  # nonstandard extension used: nameless struct/union
        "/wd4206"  # nonstandard extension used: translation unit is empty
        "/wd4244"  # integer type is converted to a smaller integer type, possible loss of data
        "/wd4245"  # '=': conversion from 'type1' to 'type2', signed/unsigned mismatch
        "/wd4310"  # cast truncates constant value
        "/wd4389"  # '!=': signed/unsigned mismatch
        # DONT  "/wd4456"  # declaration of 'identifier' hides previous local declaration
        # DONT  "/wd4457"  # declaration of 'identifier' hides function parameter
        "/wd4459"  # declaration of 'identifier' hides global declaration
        "/wd4701"  # potentially uninitialized local variable
        "/wd4702"  # unreachable code
        "/wd4996"  # 'strcat': This function or variable may be unsafe. Consider using strcat_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
        # "/wd4068"
        # "/wd4365"
        # "/wd4820"
    )
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-g -O0 -fno-limit-debug-info)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        "-Wno-unsequenced"
    )
endif()
if(CMAKE_COMPILER_IS_GNUCC)
    add_compile_options(
        "-Wno-array-bounds="
        "-Wno-maybe-uninitialized"
        "-Wno-missing-braces"
        "-Wno-sequence-point"
        "-Wno-unknown-pragmas"
        "-Wno-unused-label"
        "-Wno-unused-but-set-variable"
        "-Wno-unsequenced"
    )
endif()



# Summary of Differences
# Method            Portability     When it runs
# add_dependencies  All Generators  Before the target's first source is compiled.
# OUTPUT Command    All Generators  Only if the generated file is out-of-date.
# Pro Tip: Use the cmake -E command for common tasks (like copy, make_directory, or echo) to ensure your pre-build scripts remain platform-independent. 

# # Define the custom task
# add_custom_target(run_pre_build_task
#     COMMAND ${CMAKE_COMMAND} -E echo "Running pre-build task..."
#     # Add your specific command here
#     VERBATIM
# )
# 
# # Make your main target depend on it
# add_dependencies(my_executable run_pre_build_task)

# add_custom_command(
#     OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated.h"
#     COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/generate_header.cmake"
#     DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/template.h.in"
#     VERBATIM
# )
# 
# # The command will run because the executable depends on 'generated.h'
# add_executable(my_executable main.cpp "${CMAKE_CURRENT_BINARY_DIR}/generated.h")



add_subdirectory(STU)
add_subdirectory(platform)
add_subdirectory(MoX)
add_subdirectory(MoM)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)
add_subdirectory(demo)

# --- Installation / Packaging ---
#
# Public releases are ENGINE-ONLY: we install just the player executable plus a
# couple of docs / templates.  The player supplies their own original Master of
# Magic data files (every *.LBX plus CONFIG.MOM) at runtime — those are NOT
# shipped.  The dev/QA tools (HeMoM, savedump, rmr2hms, check_*, compare_*) stay
# buildable but are deliberately excluded from the installed/packaged set.
# GNUInstallDirs gives CMAKE_INSTALL_BINDIR (default "bin"), so a plain
# `cmake --install --prefix /usr/local` lands ReMoM in /usr/local/bin, matching
# the autotools `make install`.  The release/CPack builds override
# CMAKE_INSTALL_BINDIR=. (see .github/workflows/release.yml) to keep the portable
# ZIP / NSIS layout flat (ReMoM at the package root).
include(GNUInstallDirs)

# Installed directories get 0755 regardless of the builder's umask.  Without this
# a umask of 002 leaks 0775 dirs into the package (lintian: non-standard-dir-perm),
# which makes the artifact depend on who built it.  Must precede any install().
set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

install(TARGETS ReMoM
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Two install shapes, distinguished by CMAKE_INSTALL_BINDIR:
#   "flat" (BINDIR ".")  -> the portable ZIP / TGZ / NSIS layout: ReMoM and its
#                           docs sit at the package root, no desktop integration.
#   "FHS"  (BINDIR "bin") -> /usr, /usr/local, or the .deb staging tree: docs under
#                           share/doc/, plus a .desktop entry and an icon so the
#                           game shows up in the desktop application menu.
# See doc/#AI_Plans/PRD-Linux-Package-Install.md.
if(CMAKE_INSTALL_BINDIR STREQUAL ".")
    set(REMOM_INSTALL_FLAT TRUE)
else()
    set(REMOM_INSTALL_FLAT FALSE)
endif()

# Doc dir is named for the *package* (remom), not the CMake project (ReMoM), so an
# FHS install matches the Debian package name -- GNUInstallDirs would otherwise
# default CMAKE_INSTALL_DOCDIR to share/doc/ReMoM, which lintian flags.
if(REMOM_INSTALL_FLAT)
    set(REMOM_INSTALL_DOCDIR ".")
else()
    set(REMOM_INSTALL_DOCDIR "${CMAKE_INSTALL_DATAROOTDIR}/doc/remom")
endif()

# Bundle SDL DLLs on Windows so the player doesn't need them installed.
# No-op for the MSVC-release preset (native Win32 backend, links no SDL); this
# only matters for SDL-based Windows packaging.
if(WIN32 AND NOT USE_WIN32)
    if(_sdl_imported_loc)
        install(FILES "${_sdl_imported_loc}" DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
    if(NOT DISABLE_AUDIO AND _mixer_imported_loc)
        install(FILES "${_mixer_imported_loc}" DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()

# Player-facing docs only.  No game data, saves, test fixtures, or dev/modder
# config templates from assets/ are bundled.  Power-user features (the ReMoM.ini
# --newgame template, seeds, logging control) are deferred to a separate
# Dev/Modder effort — see the "Future phase" section in RELEASES.md.
if(EXISTS "${PROJECT_SOURCE_DIR}/PLAYING.md")
    install(FILES "${PROJECT_SOURCE_DIR}/PLAYING.md" DESTINATION ${REMOM_INSTALL_DOCDIR})
endif()

# Desktop integration -- FHS installs on Linux only (a .deb, or `cmake --install
# --prefix /usr/local`).  Deliberately absent from the flat portable archives and
# from Windows / macOS, whose package contents must stay unchanged.  These are the
# same two files linuxdeploy feeds the AppImage, so the entry and icon can't drift.
if(UNIX AND NOT APPLE AND NOT REMOM_INSTALL_FLAT)
    install(FILES "${PROJECT_SOURCE_DIR}/packaging/remom.desktop"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
    )
    install(FILES "${PROJECT_SOURCE_DIR}/packaging/remom.png"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps"
    )

    # Man page + Debian changelog, both gzipped as Debian policy requires
    # (lintian: no-manual-page, no-changelog).  `gzip -n` omits the timestamp so
    # the compressed bytes don't change between builds of identical input.
    find_program(REMOM_GZIP NAMES gzip)
    if(REMOM_GZIP)
        # Man page -- section 6 (games).
        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/packaging/ReMoM.6.gz"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/packaging"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    "${PROJECT_SOURCE_DIR}/packaging/ReMoM.6"
                    "${CMAKE_BINARY_DIR}/packaging/ReMoM.6"
            COMMAND "${REMOM_GZIP}" -9 -n -f -k "${CMAKE_BINARY_DIR}/packaging/ReMoM.6"
            DEPENDS "${PROJECT_SOURCE_DIR}/packaging/ReMoM.6"
            COMMENT "Compressing ReMoM.6 man page"
            VERBATIM
        )

        # Debian changelog.  Native package -> the file is "changelog.gz", not
        # "changelog.Debian.gz".  The date must be RFC 5322.
        string(TIMESTAMP REMOM_DEB_CHANGELOG_DATE "%a, %d %b %Y %H:%M:%S +0000" UTC)
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jim Balcomb <jim.balcomb@gmail.com>")
        configure_file(
            "${PROJECT_SOURCE_DIR}/packaging/changelog.in"
            "${CMAKE_BINARY_DIR}/packaging/changelog"
            @ONLY
        )
        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/packaging/changelog.gz"
            COMMAND "${REMOM_GZIP}" -9 -n -f -k "${CMAKE_BINARY_DIR}/packaging/changelog"
            DEPENDS "${CMAKE_BINARY_DIR}/packaging/changelog"
            COMMENT "Compressing Debian changelog"
            VERBATIM
        )

        add_custom_target(remom_package_docs ALL
            DEPENDS "${CMAKE_BINARY_DIR}/packaging/ReMoM.6.gz"
                    "${CMAKE_BINARY_DIR}/packaging/changelog.gz"
        )

        install(FILES "${CMAKE_BINARY_DIR}/packaging/ReMoM.6.gz"
            DESTINATION "${CMAKE_INSTALL_MANDIR}/man6"
        )
        install(FILES "${CMAKE_BINARY_DIR}/packaging/changelog.gz"
            DESTINATION "${REMOM_INSTALL_DOCDIR}"
        )
    else()
        message(WARNING "gzip not found -- the package will ship without a man page or changelog (lintian will flag both).")
    endif()

    # Copyright notice.  Debian policy requires every package to carry one at
    # <docdir>/copyright (lintian: no-copyright-file); it is deliberately NOT
    # gzipped.  Wording is the project owner's -- see packaging/copyright.
    install(FILES "${PROJECT_SOURCE_DIR}/packaging/copyright"
        DESTINATION "${REMOM_INSTALL_DOCDIR}"
    )

    # Lintian overrides travel with the package; each entry carries its reason.
    install(FILES "${PROJECT_SOURCE_DIR}/packaging/lintian-overrides"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/lintian/overrides"
        RENAME "remom"
    )
endif()

# Put the MSVC runtime DLLs next to the executable (CMAKE_INSTALL_BINDIR is "." in
# the flat release package, "bin" for a normal install) so a clean Windows box
# without the VC++ redistributable can still launch ReMoM.
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}")
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "ReMoM")
set(CPACK_PACKAGE_VERSION "${REMOM_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "ReMoM-${REMOM_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_PACKAGE_NAME "ReMoM")
    set(CPACK_NSIS_DISPLAY_NAME "ReMoM ${PROJECT_VERSION}")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "ReMoM")
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "ReMoM.exe")
    # Per-user install (no UAC): land in %LOCALAPPDATA%\ReMoM. The engine needs a
    # user-writable install dir -- the player drops their .LBX game data next to
    # ReMoM.exe and saves/logs are written there, so Program Files (read-only
    # for non-admins) is wrong. Our repo-local cmake/NSIS.template.in overrides
    # CPack's stock template to set `RequestExecutionLevel user` and install
    # per-user; this variable feeds that template's InstallDir root. CPack finds
    # the override via CPACK_MODULE_PATH (defaults to CMAKE_MODULE_PATH, which
    # already includes cmake/ -- see the list(APPEND CMAKE_MODULE_PATH ...) above).
    set(CPACK_NSIS_INSTALL_ROOT "$LOCALAPPDATA")

    # Start Menu shortcuts.  Without CPACK_PACKAGE_EXECUTABLES the installer creates
    # only an "Uninstall" link -- the player gets no way to launch the game.
    # CPACK_NSIS_EXECUTABLES_DIRECTORY must track CMAKE_INSTALL_BINDIR: the release
    # package is flat (BINDIR=.), so the default "bin" would generate a shortcut to a
    # non-existent $INSTDIR\bin\ReMoM.exe.
    set(CPACK_PACKAGE_EXECUTABLES "ReMoM" "ReMoM")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "${CMAKE_INSTALL_BINDIR}")

    # The player must copy their own .LBX game data into the install directory, and a
    # per-user install lands under %LOCALAPPDATA% -- a hidden folder.  Ship a Start Menu
    # link straight to it so "where do I put my LBX files?" is one click, matching the
    # missing-data dialog ReMoM shows on launch.
    # NOTE: four backslashes.  These values round-trip through CMake parsing twice
    # (here -> CPackConfig.cmake -> re-parsed by cpack), so a two-backslash form
    # collapses to "\R" and cpack dies with "Invalid character escape".
    set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\ReMoM Game Data Folder.lnk' '$INSTDIR'")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$SMPROGRAMS\\\\$MUI_TEMP\\\\ReMoM Game Data Folder.lnk'")
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP;TGZ")
elseif(REMOM_INSTALL_FLAT)
    # Linux portable archives -- the flat layout, ReMoM at the archive root.
    set(CPACK_GENERATOR "ZIP;TGZ")
else()
    # Linux native distro package -- built from the FHS layout by the
    # `linux-package-release` preset.  See doc/#AI_Plans/PRD-Linux-Package-Install.md.
    #
    # This is the counterpart to the AppImage, not a replacement: the AppImage
    # BUNDLES SDL2 so it runs on any distro, while the .deb DEPENDS on the distro's
    # SDL2 -- which is the whole point of a system package.  Nothing is bundled here.
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

    # Debian policy: package names are lower-case.  The archive artifacts keep the
    # "ReMoM" name; only the .deb diverges.
    set(CPACK_DEBIAN_PACKAGE_NAME "remom")
    set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")   # remom_<version>_<arch>.deb
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jim Balcomb <jim.balcomb@gmail.com>")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/jbalcomb/ReMoM")
    # Ship a stripped binary (lintian: unstripped-binary-or-object).  Debug info
    # belongs in the dev build, not in a ~4 MB player download.
    set(CPACK_STRIP_FILES TRUE)
    # Architecture is left unset so CPackDeb derives it from `dpkg --print-architecture`.

    # Derive Depends: (libsdl2, libsdl2-mixer, libc6, ...) from the built ELF via
    # dpkg-shlibdeps.  Hand-written dependency lists desynchronise silently when a
    # library is added or dropped; a derived one cannot.
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Reassembly of Master of Magic v1.31 (Simtex, 1995)")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
"ReMoM is a C reconstruction of the 1994 turn-based fantasy strategy game\n\
Master of Magic, rebuilt from a disassembly of the original DOS executable.\n\
.\n\
This package contains the game engine only.  It does NOT include any game\n\
data: you must own a copy of Master of Magic v1.31 (DOS) and supply its data\n\
files (every *.LBX plus CONFIG.MOM) yourself.  ReMoM finds them via a search\n\
path -- see /usr/share/doc/remom/PLAYING.md.\n\
.\n\
Saved games, settings and logs are written to the usual per-user locations\n\
(~/.local/share/ReMoM, ~/.config/ReMoM, ~/.local/state/ReMoM); your original\n\
game data is never modified.")
endif()

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "ReMoM-${REMOM_VERSION}-Source")
set(CPACK_SOURCE_IGNORE_FILES
    "/assets/"
    "/\\.LBX/"
    "\\.LBX\\$"
    "/\\.git/"
    "/build/"
    "/out/"
)
include(CPack)
