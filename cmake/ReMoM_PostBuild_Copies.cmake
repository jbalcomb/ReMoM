include(CMakeParseArguments)

function(ReMoM_Add_Target_Support_Files target_name)
    set(options
        COPY_SDL3
        COPY_SDL3_MIXER
        COPY_GMOCK
        COPY_LBX
    )

    cmake_parse_arguments(RM "${options}" "" "" ${ARGN})

    if(WIN32 AND RM_COPY_SDL3)
        if(SDL3_DLL_FILE_PATH AND EXISTS "${SDL3_DLL_FILE_PATH}")
            add_custom_command(
                TARGET ${target_name}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${SDL3_DLL_FILE_PATH}
                    $<TARGET_FILE_DIR:${target_name}>
                COMMAND_EXPAND_LISTS
                COMMENT "Copying SDL3.dll"
            )
        else()
            message(WARNING "COPY_SDL3 requested for ${target_name}, but SDL3.dll path is not available")
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

    if(WIN32 AND RM_COPY_SDL3_MIXER AND NOT DISABLE_AUDIO)
        if(SDL3_MIXER_DLL_FILE_PATH AND EXISTS "${SDL3_MIXER_DLL_FILE_PATH}")
            add_custom_command(
                TARGET ${target_name}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${SDL3_MIXER_DLL_FILE_PATH}
                    $<TARGET_FILE_DIR:${target_name}>
                COMMAND_EXPAND_LISTS
                COMMENT "Copying SDL3_mixer.dll"
            )
        else()
            message(WARNING "COPY_SDL3_MIXER requested for ${target_name}, but SDL3_mixer.dll path is not available")
        endif()
    endif()

    if(RM_COPY_LBX)
        file(GLOB LBX_FILE_LIST "${CMAKE_SOURCE_DIR}/*.lbx" "${CMAKE_SOURCE_DIR}/*.LBX")
        if (NOT LBX_FILE_LIST)
            # If the list is empty, issue a fatal error to stop the configuration
            message(FATAL_ERROR "No files found: ${CMAKE_SOURCE_DIR}/*.lbx or ${CMAKE_SOURCE_DIR}/*.LBX")
        endif()
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
