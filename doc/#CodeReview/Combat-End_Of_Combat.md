Combat-End_Of_Combat.md

SEEALSO:  Combat-Combat_Screen_Map_Draw.md
SEEALSO:  Combat-Combat_Figure_Compose.md
SEEALSO:  Combat-End_Of_Combat.md
SEEALSO:  C:\STU\devel\ReMoM\doc\Combat\MoM-CombatScreen-Grid.md



Combat_Screen()
    |-> End_Of_Combat()



It's not a result code — it's the selector for which end-of-combat scroll message to show.

The chain is direct: Battle_Result → End_Of_Combat__WIP(..., MsgType) → CMB_ScrollMsg_Type at 15047 → the switch at 16261 and the tests at 16065-16117.

The five branches in Combat_Screen (2190-2208) and the labels already in the file give the value space:

meaning	source
1	combat victory, tactical	16113
2	you lost	the else at 2208
3	you fled	2200
4	turn limit — _combat_turn > 50	2204
5	enemy fled	16115
6	combat victory, strategic	16117, set at 17860
Value 6 is what settles it. 1 and 6 are the same outcome — you won — but different messages, tactical versus strategic. A variable that distinguishes those is selecting a message, not reporting a result.

Recommend end_of_combat_message_type. That names where it's decided and what it drives, and it lines up with the parameter (MsgType) and global (CMB_ScrollMsg_Type) it flows into — both of which are worth renaming with it, to message_type and _combat_scroll_message_type.

The bigger win here isn't the name. Six values, all bare literals, at eleven sites across three functions with the meanings living only in trailing comments. An e_COMBAT_SCROLL_MESSAGE enum would kill every magic number in that table at once.



End_Of_Combat__WIP(winner_player_idx, item_count, item_list, end_of_combat_message_type);

---
