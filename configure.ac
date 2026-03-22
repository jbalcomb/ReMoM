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

AM_CONDITIONAL([USE_SDL3], [test "x$with_sdl3" = "xyes"])

# --------------------------------------------------------------------------
#  SDL detection (SDL2 default, SDL3 opt-in with --with-sdl3)
# --------------------------------------------------------------------------

AS_IF([test "x$with_sdl3" = "xyes"],
    [# SDL3
     PKG_CHECK_MODULES([SDL], [sdl3])
     AS_IF([test "x$enable_static_sdl" = "xyes"],
         [SDL_LIBS=$($PKG_CONFIG --static --libs sdl3)])
     AC_DEFINE([USE_SDL3], [1], [Use SDL3 instead of SDL2])
     sdl_backend="SDL3"
    ],
    [# SDL2 (default)
     PKG_CHECK_MODULES([SDL], [sdl2])
     AS_IF([test "x$enable_static_sdl" = "xyes"],
         [SDL_LIBS=$($PKG_CONFIG --static --libs sdl2)])
     sdl_backend="SDL2"
    ])

# --------------------------------------------------------------------------
#  SDL_mixer (optional, default=yes)
# --------------------------------------------------------------------------

AS_IF([test "x$enable_audio" = "xyes"],
    [AS_IF([test "x$with_sdl3" = "xyes"],
        [PKG_CHECK_MODULES([SDL_MIXER], [SDL3_mixer],
            [have_mixer=yes],
            [AC_MSG_WARN([SDL3_mixer not found -- building without audio])
             have_mixer=no])],
        [PKG_CHECK_MODULES([SDL_MIXER], [SDL2_mixer],
            [have_mixer=yes],
            [AC_MSG_WARN([SDL2_mixer not found -- building without audio])
             have_mixer=no])])],
    [have_mixer=no])

AS_IF([test "x$have_mixer" = "xyes"],
    [AS_IF([test "x$enable_static_sdl" = "xyes"],
        [AS_IF([test "x$with_sdl3" = "xyes"],
            [SDL_MIXER_LIBS=$($PKG_CONFIG --static --libs SDL3_mixer)],
            [SDL_MIXER_LIBS=$($PKG_CONFIG --static --libs SDL2_mixer)])])])

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
AS_IF([test "x$have_mixer" = "xyes"],
    [echo "  SDL_MIXER_LIBS:   $SDL_MIXER_LIBS"])
echo ""
