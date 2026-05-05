# Test CPACK_SOURCE_IGNORE_FILES regex patterns against sample paths.
# Uses the same cmsys::RegularExpression engine CPack uses, so a match here
# means CPack SHOULD treat the file as ignored.
#
# Usage:  cmake -P tools/test_cpack_ignore.cmake
# Edit PATTERNS and PATHS below to test your own combinations.

set(PATTERNS
    "/assets/"
    "/\\.LBX/"
    "\\.LBX$"
    "/\\.git/"
    "/build/"
    "/out/"
)

set(PATHS
    "C:/STU/devel/ReMoM/assets/FONTS.LBX"
    "C:/STU/devel/ReMoM/assets/data.json"
    "C:/STU/devel/ReMoM/.git/HEAD"
    "C:/STU/devel/ReMoM/build/foo.o"
    "C:/STU/devel/ReMoM/out/build/MSVC-debug/CMakeCache.txt"
    "C:/STU/devel/ReMoM/src/main.c"
)

foreach(path ${PATHS})
    set(matched_by "")
    foreach(pat ${PATTERNS})
        if("${path}" MATCHES "${pat}")
            list(APPEND matched_by "${pat}")
        endif()
    endforeach()
    if(matched_by)
        message("IGNORED   ${path}")
        foreach(p ${matched_by})
            message("   matched by:  ${p}")
        endforeach()
    else()
        message("kept      ${path}")
    endif()
endforeach()
