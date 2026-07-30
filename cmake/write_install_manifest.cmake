# write_install_manifest.cmake -- record exactly which files this version installed.
#
# Run as the FINAL install(SCRIPT) of the project (see the bottom of the root
# CMakeLists.txt).  CMAKE_INSTALL_MANIFEST_FILES accumulates as each file is installed,
# so a script that runs last sees the complete payload -- and only the payload.  The
# NSIS installer reads the previous version's copy of this file on upgrade and deletes
# what it lists before laying down the new payload, which is how files we RETIRE
# (renamed or dropped) stop lingering in the install folder forever.
#
# Why a manifest and not a wildcard sweep: the install folder also holds the user's
# copied Master of Magic game data, their logs/ and COMBAT.TMP.  Only names that appear
# here -- i.e. that we ourselves installed -- are ever deleted.

set(_remom_manifest_name "remom_installed_files.txt")
set(_remom_lines "")

foreach(_remom_file IN LISTS CMAKE_INSTALL_MANIFEST_FILES)
    file(RELATIVE_PATH _remom_rel "${CMAKE_INSTALL_PREFIX}" "${_remom_file}")
    # Never list the manifest itself (it is written after the manifest list is built,
    # and the installer deletes it explicitly).
    if(NOT _remom_rel STREQUAL "${_remom_manifest_name}")
        string(APPEND _remom_lines "${_remom_rel}\n")
    endif()
endforeach()

file(WRITE "${CMAKE_INSTALL_PREFIX}/${_remom_manifest_name}" "${_remom_lines}")

list(LENGTH CMAKE_INSTALL_MANIFEST_FILES _remom_count)
message(STATUS "ReMoM: install manifest lists ${_remom_count} file(s) -> ${_remom_manifest_name}")
