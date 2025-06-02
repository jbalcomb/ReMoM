
Spellbook Screen

AKA Small Spellbook
NOT Big Spellbook
NOT Combat Spellbook
HLPENTRY Description: "Spell Book Help"

uses Copy_Off_To_Back() && Copy_Back_To_Off() paradigm
background graphic is _spellbook_small_seg




Spellbook_Screen()

    // TODO  sub_B8AD0();
    Spellbook_Build__WIP(0, 6);  // Book_Type, Page_Size

            ; creates a list of spells for the specified book type,
                                        ; then organizes them into the global near dynamic
                                        ; allocation SBK_BookPages@ (created here)
                                        ; Book_Type corresponds to one of the following:
                                        ;   {0: overland spellbook, 1: combat spellbook, 2: apprentice spellbook}





SPECIAL2.LBX

SMLBOOK
BCORNERS
BCORNERS
BOOKSYMB
BOOKSYMB
BOOKSYMB
BOOKSYMB
BOOKSYMB
BOOKSYMB
SMALTEXT





##### Naming Things Is Hard

