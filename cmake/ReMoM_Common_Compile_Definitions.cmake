function(ReMoM_Apply_Common_Compile_Definitions target_name)
    target_compile_definitions(${target_name} PRIVATE
        _CRT_SECURE_NO_WARNINGS=TRUE
        $<$<CONFIG:Debug>:STU_DEBUG=TRUE>
        $<$<CONFIG:Debug>:STU_TRACE=TRUE>
        $<$<CONFIG:Debug>:_STU_SDL2=TRUE>
    )
endfunction()
