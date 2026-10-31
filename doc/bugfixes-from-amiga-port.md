
C:\STU\devel\ReMoM\doc\@TODO\ReMoM-fixes-for-Claude.txt (from TheQ)
C:\STU\devel\ReMoM\doc\@CodeReview\Amiga-Port-Reports-Triage.md

C:\STU\devel\ReMoM\doc\@TODO\BUGS_AND_QUIRKS.txt  (from binaryalgorithm)



## In Dispute
### L42 — city screen farmer/worker clicks — **Not a bug (faithful)**
"City screen farmers/workers (CityScr.c): clicking citizen i set i+1 farmers (first -> 0), so clicking any farmer but the first did nothing. Now: click farmer i -> i farmers; click worker i -> i+1 farmers; never below the minimum. [ReMoM, our reading of the intended rule]"

## New Code
Cartographer_Screen() |-> Cartograph_Screen_Draw_Map() |-> Cartograph_Screen_Draw_Map_Terrain()
// WZD o150p18  // sub_DAFF4()  int16_t Square_Is_Sailable__NIU(int16_t wx, int16_t wy, int16_t wp)
// WZD s161p24  int16_t Square_Is_Sailable(int16_t wx, int16_t wy, int16_t wp)
Powergraph_Screen() |-> Powergraph_Screen_Draw() |-> Powergraph_Draw_Wizard_History()
TaxCollector_Window()
Status_Screen() |-> Status_Screen_Draw()

---

Verdict                                 Count   Items
ReMoM (your reconstruction)	            30      L10, 11, 13, 14, 15, 17, 18, 19, 23, 26, 27, 31, 32, 33, 34, 35, 37, 40, 41, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, plus L36, which is #21 and already fixed
OGBUG (leave, add a note)               3       L12 (the - 1 transport seat), L25 (AI enchants the human's units), L28 (capital on a node)
Not a bug (the asm does exactly this)   4   L16 (Stack_Moves), L20 (road-turn estimate), L42 (farmer clicks), L54 (Notify2 box)
Architecture (needs a design decision)  5   L24, L46, L57, L58, L61: single-process and EMM-replacement side effects
Where the asm disagrees with the reporter's fix

Three of the ReMoM errors are real, but their fix isn't the faithful one:
L17: the plane write is missing from Units_In_Tower, not from Move_Units_Draw.
L41: like #19, the prototype of City_Sell_Building is swapped; the call at CityScr.c:808 is right. This one should have been caught alongside #19 and wasn't.
L44 and L50: the missing pieces (the unit loop, the Cartographer terrain) should be rebuilt from the asm, not from their versions.

Also found (not in their list)
Update_MovePathMap:
_landmasses is read without the plane offset, twice (6116, 6171), so on Myrror it checks Arcanus.
6127 calls Player_Army_At_Square on the destination where the asm uses the unit's own square.
The capacity test is inside the loop instead of after it.
UNITSTK.c:1336: the capacity test uses < where the asm allows <=.
MainScr.c:4355: this XREF note is wrong, and it misled the reporter on L16.
Unit enchantments (L26): a human cancelling the target screen still enchants a unit, not just the AI-with-no-target case.

Decisions I need from you
L24: should AIMOVE.c:6810 get the same /* HACK */ divide-by-zero guard that 1696 already has?
L46: should Continue run the WZD start-up palette steps that are already written as comments at MOM_SCR.c:131-140?
L57 and L58: load the WZD resources once, and use one reusable buffer in place of the EMM page?
L61: a per-game reset for single-process New Game. That's a design task of its own, so I'd write a PRD for it rather than fold it into this batch.

Once you've made the amiga-port-reports branch, I'll start fixing the 30 ReMoM items, crash and corruption ones first: L14, L13, L40, L43, L48, L23.
Each will follow the comment-out-and-replace rule, with a HeMoM test wherever the path can run headless.

...

Q:  L24: should AIMOVE.c:6810 get the same /* HACK */ divide-by-zero guard that 1696 already has?
A:  Yes.
Q:  L46: should Continue run the WZD start-up palette steps that are already written as comments at MOM_SCR.c:131-140?
A:  
Q:  L57 and L58: load the WZD resources once, and use one reusable buffer in place of the EMM page?
A:  
Q:  L61: a per-game reset for single-process New Game. That's a design task of its own, so I'd write a PRD for it rather than fold it into this batch.
A:  

The stale war targets on New Game are untouched. Nothing resets _ai_landmass_war_targets between games;
  the only hits for it in the setup code are its declaration in MOM_DAT.c:906.
  That part is Architecture, and it belongs with L61's per-game reset, so it isn't expected to be done yet.
