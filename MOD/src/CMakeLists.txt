# MOD -- mod support: hook dispatcher (C), Lua runtime (C++ through sol2), mod folder loader (C).
# The game's C code includes only the C headers; MOD_Lua.cpp is the one C++ translation unit.

add_library(MOD STATIC
    CAS.c
    MOD_Hook.c
    MOD_Load.c
    MOD_Lua.cpp
)

target_include_directories(MOD PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MOD
    PUBLIC
        MOX
        STU
    PRIVATE
        sol2
        lua_static
)
