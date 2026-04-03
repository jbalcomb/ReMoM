AC_INIT([ReMoM], [0.0.1], [], [remom],
        [https://github.com/jbalcomb/ReMoM])

AC_CONFIG_AUX_DIR([autotools])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR
PKG_PROG_PKG_CONFIG

# --------------------------------------------------------------------------
#  Options
# --------------------------------------------------------------------------

AC_ARG_ENABLE([audio],
    AS_HELP_STRING([--disable-audio], [Build without sound support]),
    [], [enable_audio=yes])

AC_ARG_ENABLE([static-sdl],
    AS_HELP_STRING([--enable-static-sdl], [Link SDL statically (no DLLs needed at runtime)]),
    [], [enable_static_sdl=no])

AC_ARG_WITH([sdl3],
    AS_HELP_STRING([--with-sdl3], [Use SDL3 instead of SDL2 (default: SDL2)]),
    [], [with_sdl3=no])

AC_ARG_WITH([headless],
    AS_HELP_STRING([--with-headless], [Headless build — no display, no audio, no SDL dependency]),
    [], [with_headless=no])

AM_CONDITIONAL([USE_SDL3], [test "x$with_sdl3" = "xyes"])
AM_CONDITIONAL([USE_HEADLESS], [test "x$with_headless" = "xyes"])

# --------------------------------------------------------------------------
#  SDL detection (SDL2 default, SDL3 opt-in with --with-sdl3)
#
#  PKG_CHECK_MODULES must be at the top level (not nested inside AS_IF)
#  because autoconf's M4 tracer cannot expand macros through shell quoting.
#  We probe both SDL2 and SDL3, then pick the right result with shell code.
# --------------------------------------------------------------------------

if test "x$with_headless" = "xyes"; then
    SDL_CFLAGS=""
    SDL_LIBS=""
    AC_DEFINE([USE_HEADLESS], [1], [Headless build — no display, no audio, no SDL])
    AC_DEFINE([NO_SOUND_LIBRARY], [1], [Build without audio support])
    sdl_backend="headless"
    enable_audio=no
else
    PKG_CHECK_MODULES([SDL2], [sdl2], [have_sdl2=yes], [have_sdl2=no])
    PKG_CHECK_MODULES([SDL3], [sdl3], [have_sdl3=yes], [have_sdl3=no])

    if test "x$with_sdl3" = "xyes"; then
        if test "x$have_sdl3" != "xyes"; then
            AC_MSG_ERROR([--with-sdl3 was given but sdl3 was not found by pkg-config])
        fi
        SDL_CFLAGS="$SDL3_CFLAGS"
        SDL_LIBS="$SDL3_LIBS"
        if test "x$enable_static_sdl" = "xyes"; then
            SDL_LIBS=$($PKG_CONFIG --static --libs sdl3)
        fi
        AC_DEFINE([USE_SDL3], [1], [Use SDL3 instead of SDL2])
        sdl_backend="SDL3"
    else
        if test "x$have_sdl2" != "xyes"; then
            AC_MSG_ERROR([sdl2 was not found by pkg-config (use --with-sdl3 for SDL3)])
        fi
        SDL_CFLAGS="$SDL2_CFLAGS"
        SDL_LIBS="$SDL2_LIBS"
        if test "x$enable_static_sdl" = "xyes"; then
            SDL_LIBS=$($PKG_CONFIG --static --libs sdl2)
        fi
        sdl_backend="SDL2"
    fi
fi
AC_SUBST([SDL_CFLAGS])
AC_SUBST([SDL_LIBS])

# --------------------------------------------------------------------------
#  SDL_mixer (optional, default=yes)
# --------------------------------------------------------------------------

PKG_CHECK_MODULES([SDL2_MIXER], [SDL2_mixer], [have_sdl2_mixer=yes], [have_sdl2_mixer=no])
PKG_CHECK_MODULES([SDL3_MIXER], [SDL3_mixer], [have_sdl3_mixer=yes], [have_sdl3_mixer=no])

have_mixer=no
if test "x$enable_audio" = "xyes"; then
    if test "x$with_sdl3" = "xyes"; then
        if test "x$have_sdl3_mixer" = "xyes"; then
            have_mixer=yes
            SDL_MIXER_CFLAGS="$SDL3_MIXER_CFLAGS"
            SDL_MIXER_LIBS="$SDL3_MIXER_LIBS"
            if test "x$enable_static_sdl" = "xyes"; then
                SDL_MIXER_LIBS=$($PKG_CONFIG --static --libs SDL3_mixer)
            fi
        else
            AC_MSG_WARN([SDL3_mixer not found -- building without audio])
        fi
    else
        if test "x$have_sdl2_mixer" = "xyes"; then
            have_mixer=yes
            SDL_MIXER_CFLAGS="$SDL2_MIXER_CFLAGS"
            SDL_MIXER_LIBS="$SDL2_MIXER_LIBS"
            if test "x$enable_static_sdl" = "xyes"; then
                SDL_MIXER_LIBS=$($PKG_CONFIG --static --libs SDL2_mixer)
            fi
        else
            AC_MSG_WARN([SDL2_mixer not found -- building without audio])
        fi
    fi
fi
AC_SUBST([SDL_MIXER_CFLAGS])
AC_SUBST([SDL_MIXER_LIBS])

AM_CONDITIONAL([HAVE_AUDIO], [test "x$have_mixer" = "xyes"])
AS_IF([test "x$have_mixer" != "xyes"],
    [AC_DEFINE([NO_SOUND_LIBRARY], [1], [Build without audio support])])

# --------------------------------------------------------------------------
#  Platform checks
# --------------------------------------------------------------------------

AC_CHECK_LIB([m], [floor], [LIBS="$LIBS -lm"])

# --------------------------------------------------------------------------
#  Output
# --------------------------------------------------------------------------

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

# --------------------------------------------------------------------------
#  Summary
# --------------------------------------------------------------------------

echo ""
echo "  ReMoM $PACKAGE_VERSION"
echo ""
echo "  SDL backend:      $sdl_backend"
echo "  Audio support:    $have_mixer"
echo "  Static SDL:       $enable_static_sdl"
echo ""
echo "  CFLAGS:           $CFLAGS"
echo "  SDL_CFLAGS:       $SDL_CFLAGS"
echo "  SDL_LIBS:         $SDL_LIBS"
if test "x$have_mixer" = "xyes"; then
    echo "  SDL_MIXER_LIBS:   $SDL_MIXER_LIBS"
fi
echo ""
