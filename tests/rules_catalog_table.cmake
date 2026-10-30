# Regenerates the rules catalog table from the triples and compares it with the committed one.
execute_process(
    COMMAND "${PYTHON}" -I "${SCRIPT}" "${TTL}"
    OUTPUT_FILE "${OUT}"
    RESULT_VARIABLE rc
)
if(NOT rc EQUAL 0)
    message(FATAL_ERROR "ttl_to_table.py failed (${rc})")
endif()
execute_process(COMMAND "${CMAKE_COMMAND}" -E compare_files "${OUT}" "${EXPECTED}" RESULT_VARIABLE diff)
if(NOT diff EQUAL 0)
    message(FATAL_ERROR "doc/rules-catalog/rules_catalog.md is out of date; regenerate it with tools/rules_catalog/ttl_to_table.py")
endif()
