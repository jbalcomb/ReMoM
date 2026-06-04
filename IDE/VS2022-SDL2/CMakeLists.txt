    // ...existing code...
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Suppress LNK4006: __NULL_IMPORT_DESCRIPTOR already defined.
    # This is a harmless MSVC linker warning that occurs when linking multiple SDL libraries (e.g., SDL2 + SDL2_mixer).
    add_link_options("/ignore:4006")
    # add_compile_options(/wd4068)
    add_compile_options(
        "/W4"
// ...existing code...