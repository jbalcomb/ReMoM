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

    set(_sdl2d_runtime_path "${SDL2D_DLL_FILE_PATH}")
    if((NOT _sdl2d_runtime_path) OR (_sdl2d_runtime_path STREQUAL ""))
        if(DEFINED SDL2_DLL_FILE_PATH AND NOT SDL2_DLL_FILE_PATH STREQUAL "")
            get_filename_component(_sdl2_runtime_dir "${SDL2_DLL_FILE_PATH}" DIRECTORY)
            set(_sdl2d_candidate "${_sdl2_runtime_dir}/SDL2d.dll")
            if(EXISTS "${_sdl2d_candidate}")
                set(_sdl2d_runtime_path "${_sdl2d_candidate}")
            endif()
        endif()
    endif()

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
        if(_sdl2d_runtime_path AND EXISTS "${_sdl2d_runtime_path}")
            add_custom_command(
                TARGET ${target_name}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${_sdl2d_runtime_path}
                    $<TARGET_FILE_DIR:${target_name}>
                COMMAND_EXPAND_LISTS
                COMMENT "Copying SDL2d.dll"
            )
        else()
            message(WARNING "COPY_SDL2D requested for ${target_name}, but SDL2d.dll path is not available")
        endif()
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
