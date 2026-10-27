# Vendored third-party code.  Provenance, hashes and vetting notes live next to each library
# (ext/lua/PROVENANCE.md, ext/sol2/PROVENANCE.md) per doc/@Devel/Dependency-Vetting.md.

# -------------------------------------------------------------------------
#  Lua 5.4 (C) -- the mod scripting runtime.
# -------------------------------------------------------------------------
set(LUA_SOURCES
    lua/lapi.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lcode.c
    lua/lcorolib.c
    lua/lctype.c
    lua/ldblib.c
    lua/ldebug.c
    lua/ldo.c
    lua/ldump.c
    lua/lfunc.c
    lua/lgc.c
    lua/linit.c
    lua/liolib.c
    lua/llex.c
    lua/lmathlib.c
    lua/lmem.c
    lua/loadlib.c
    lua/lobject.c
    lua/lopcodes.c
    lua/loslib.c
    lua/lparser.c
    lua/lstate.c
    lua/lstring.c
    lua/lstrlib.c
    lua/ltable.c
    lua/ltablib.c
    lua/ltm.c
    lua/lundump.c
    lua/lutf8lib.c
    lua/lvm.c
    lua/lzio.c
)

add_library(lua_static STATIC ${LUA_SOURCES})

target_include_directories(lua_static PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lua
)

# Upstream code: build it as upstream intends, not under the project's warning policy.
if(MSVC)
    target_compile_options(lua_static PRIVATE /W3 /WX-)
else()
    target_compile_options(lua_static PRIVATE -w)
    # LUA_USE_POSIX: mkstemp instead of tmpnam (avoids the linker's tmpnam warning).
    target_compile_definitions(lua_static PRIVATE LUA_USE_POSIX)
    target_link_libraries(lua_static PUBLIC m)
endif()

# -------------------------------------------------------------------------
#  sol2 3.x (C++17, header-only) -- C++ binding for Lua.  Only MOD links it.
# -------------------------------------------------------------------------
add_library(sol2 INTERFACE)

target_include_directories(sol2 SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/sol2/include
)

target_compile_definitions(sol2 INTERFACE
    SOL_ALL_SAFETIES_ON=1
    SOL_USING_CXX_LUA=0
    SOL_USE_LUA_HPP=0
    SOL_PRINT_ERRORS=0   # MOD_Lua logs script errors itself (LOG_CAT_MOD)
)

target_link_libraries(sol2 INTERFACE lua_static)
