include(CMakeParseArguments)

function(ReMoM_Add_Target_Support_Files target_name)
    set(options
        COPY_SDL2
        COPY_SDL2_MIXER
        COPY_SDL2D
        COPY_GMOCK
        COPY_LBX
    )

    cmake_parse_arguments(RM "${options}" "" "" ${ARGN})

    if(WIN32 AND RM_COPY_SDL2)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL2_DLL_FILE_PATH}
                $<TARGET_FILE_DIR:${target_name}>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying SDL2.dll"
        )
    endif()

    if(WIN32 AND RM_COPY_SDL2D)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL2D_DLL_FILE_PATH}
                $<TARGET_FILE_DIR:${target_name}>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying SDL2d.dll"
        )
    endif()

    if(WIN32 AND RM_COPY_GMOCK)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GMOCK_DLL_FILE_PATH}
                $<TARGET_FILE_DIR:${target_name}>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying gmock.dll"
        )
    endif()

    if(WIN32 AND RM_COPY_SDL2_MIXER AND NOT DISABLE_AUDIO)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SDL2_MIXER_DLL_FILE_PATH}
                $<TARGET_FILE_DIR:${target_name}>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying SDL2_mixer.dll"
        )
    endif()

    if(RM_COPY_LBX)
        file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx")
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${LBX_FILE_LIST}
                $<TARGET_FILE_DIR:${target_name}>
            COMMAND_EXPAND_LISTS
            COMMENT "Copying LBX files"
        )
    endif()
endfunction()
