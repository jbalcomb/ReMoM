


Elsewhere, ...
data segment
    buncha stuff
        dseg:00AA                                                 BEGIN: Data Segment - Initialized Data
        ...
        dseg:2080                                                 ¿  BEGIN: meaningful boundary ?
        dseg:2080 00 10                                           __ovrbuffer dw 4096                     ; DATA XREF: __OvrPrepare+19r ...  ; declared in overlay.lib\OVRBUFF
        dseg:2080                                                 ¿  BEGIN: meaningful boundary ?
        ...
        dseg:2082 41 6C 63 68 65 6D 79                            cnst_Alchemy db 'Alchemy'               ; DATA XREF: dseg:wizard_abilities_nameso
        dseg:2089 00                                              empty_string__MAIN db 0                 ; DATA XREF: _main+226o ...
        ...so many strings...
        dseg:2874 45 78 70 61 6E 73 69 6F 6E 69 73 74 00          aExpansionist db 'Expansionist',0       ; DATA XREF: dseg:1FDCo

    main()
    ovr050 
        dseg:2904                                                 BEGIN:  ovr050
        first overlay

¿ maybe strings are just at the beginning of each initialized data segment ?

Up o _main+226  mov     ax, offset empty_string__MAIN   ; empty string
Up o dseg:rtn14 dw offset empty_string__MAIN                          

empty_string__MAIN
    seems particularly meaningful
    could be the start of where everything else was brought in from includes

dseg:1ED6 89 20                                           rtn14 dw offset empty_string__MAIN

¿ could mark boundary for MoX ?







¿ deduce the modules purpose/responsibility ? cause naming things is hard
¿ API boundaries ? only used per-screen ? ~ sub-system ?
¿ management of overlays and loading location/timing ?
e.g., Draw Unit Enchantment Outline is *shared* across Unit Window, Movement Map, and Combat
¿ assumptions ? City_Screen assumes it is after/under the Main Screen ?

Logical Groupings:
Main Screen
-load screen images - background, buttons, ... ? all terrain ? all figures ?
-font glyphs ^== palette
-add fields
-get input
-match fields
Draw - Screen - Main Screen
-per component/window/view/widget

¿ data accessed by ovr063 ?
¿ data only accessed by ovr063 ?
¿ functions accessed by ovr063 ?
¿ functions only accessed by ovr063 ?
¿ ovr063 functions accessed by non-ovr063 functions ?
¿ ovr063 functions accessed by ovr063 ?
¿ ovr063 functions only accessed by ovr063 ?


Is dealing with the Unit Stack a whole separate area of its own?
  _unit_stack, _unit_stack_count
  ¿ function passes in/out parameter of array of 9 int to get short list of idx of unit stack ?
  short-list of units where active == ST_TRUE
  when does the status change?
  Build_Stack?


MoO2  Module: SHIPSTAK

ovr071 
    OVL_GetMoveTypes, STK_GetMoveTypes
    UNIT_IsFlying, UNIT_IsWindWalker, UNIT_IsOceanFaring, UNIT_IsSailing, UNIT_IsSwimming
    UNIT_ReturnZero
    UNIT_HasItemSwim, UNIT_HasItemFlight, UNIT_IsInvisible, UNIT_HasEnduranceUE, UNIT_HasPlanarItem, UNIT_IsNonCorporeal
"Movement Types"
"Special Abilities"
¿ all impact movement point calculations ?
No, ... not for invisible, planar travel, or non-corporeal
¿ "IsOceanFaring" ~== can move on ocean terrain type sqaures ? sailing, swimming, flying, wind walking ?
¿ multi-use: movement type icon & terrain passage & movement points & planar travel ?
