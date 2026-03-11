


Where are my notes on IDA settings for working on the disassembly vs. producing dump files?





jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM (current)
$ grep -r boundaries
Binary file .vs/ReMoM/v17/Browse.VC.db matches
Binary file DIPLOMSG.LBX matches
doc/#AI_Coder/GHCP-Instructions.md:reverse-engineering constraints, module boundaries, style, CMake integration, and test/mocking expectations.
doc/MoM-Cityscape.md:¿ with 1 pixel pad on either side - hence the -2 at column boundaries {4,8,13} ?
doc/MoM-Spells-AnimVars.md:Tighter boundaries?
doc/MoM-Spells-Combat-UnitSpells.md:no sure what the boundaries are for SimTex's "Unit Spells" terminology
doc/SRD-NEWG_CreateLands.md:`60 / 16 = 3.75` and `40 / 11 = 3.6`. These magic numbers may be artifacts from an earlier map size (e.g., 80x55 would give clean 5x5 divisions). The practical effect is that section boundaries don't align evenly with the world edges, and the last column/row of sections extends beyond the map.
doc/STU-ImageScaling.md:An image (or regions of an image) can be zoomed either through pixel replication or interpolation. Figure 2 shows how pixel replication simply replaces each original image pixel by a group of pixels with the same value (where the group size is determined by the scaling factor). Alternatively, interpolation of the values of neighboring pixels in the original image can be performed in order to replace each pixel with an expanded group of pixels. Most implementations offer the option of increasing the actual dimensions of the original image, or retaining them and simply zooming a portion of the image within the old image boundaries.
doc/STU-MovementPath-DiscordNotes.md:Actually, given the code-module boundaries, ...
doc/WZD-Segments.md:¿ API boundaries ? only used per-screen ? ~ sub-system ?
doc/_Cycle_Palette_Color/MoX-Palette-Update_Cycle.md:As we suspected from the caller function, the arguments are passed as pointers (lea ax, [bp+red_min], etc.), which the assembly loads into the SI and DI registers so it can read the target boundaries.
doc/__TODO-ProjectOverview.md:32K lines across many segments. Here are the natural split boundaries based on the disassembly segments:
doc/__TODO-ProjectOverview.md:I'd start with the cleanest cuts — the ones with no shared static variables and clear segment boundaries:
Binary file IDE/VS2019/.vs/ReMoM/v17/Browse.VC.db matches
IDE/VS2019/MoM/tests/MAPGEN_TEST_DOCUMENTATION.md:5. **Boundary Cases:** Tests include edge cases for map boundaries and coordinate wrapping
Binary file IDE/VS2019/ReMoMber/DIPLOMSG.LBX matches
MoM/src/MAPGEN.c: * Coordinates are wrapped at map boundaries, so neighborhood walks continue across
out/build/MSVC-nosound-debug/html/graph_legend.html:A box with a red border denotes a documented struct or class forwhich not all inheritance/containment relations are shown. A graph is truncated if it does not fit within the specified boundaries. </li>
out/build/MSVC-nosound-debug/html/_m_a_p_g_e_n_8c.html:<p>Coordinates are wrapped at map boundaries, so neighborhood walks continue across edges. All changes are applied in-place through p_world_map.</p>
Binary file out/build/MSVC-nosound-debug/MoM/tests/Debug/DIPLOMSG.LBX matches
Binary file out/build/MSVC-nosound-debug/MoX/tests/Debug/DIPLOMSG.LBX matches
Binary file out/build/MSVC-nosound-debug/src/Debug/DIPLOMSG.LBX matches
out/build/MSVC-nosound-debug/_deps/googletest-src/googletest/src/gtest.cc:// escaped. Split them on escaped '\n' boundaries. Leave all other escaped
STU/src/STU_WAV.c:(API) boundaries?

