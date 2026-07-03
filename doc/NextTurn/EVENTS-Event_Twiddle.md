EVENTS-Event_Twiddle.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr080\Event_Twiddle.asm   (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr080\Event_Twiddle.c     (Gemini translation of the .asm — second opinion)

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Event_Twiddle()       EVENTS

---

# `Event_Twiddle` — Walkthrough

**Location:** [EVENTS.c:654-1310](../../MoM/src/EVENTS.c#L654-L1310) (~657 lines, ends [line 1310](../../MoM/src/EVENTS.c#L1310)).
**WZD overlay:** ovr080, s080p02 (drake178 IDA label: `EVNT_Progress`; `MoO2` module `Events :: Event_Twiddle_`).
**Called from:** [`Next_Turn_Calc`](../../MoM/src/NEXTTURN.c) — normal end-of-turn (Wave 8B), the counterpart to `Determine_Event` (8A).

> Verified against the **`.asm`** (1222 lines). The `.c` is a Gemini translation of that asm; the asm bytes decide. **Production is 1:1 with the asm — no divergences.** Builds clean (Debug, MSVC). All 18 event blocks checked block-by-block.

## Purpose

Advances every event that is currently **starting (`es_Starting` = 1)** or **ongoing (`es_Ongoing` = 2)**. For each of the 18 event types it applies the effect (one-shot events fire and clear to `es_Nothing` = 0; multi-turn events — Plague, Population Boom, moons, conjunctions, Mana Short — announce on turn 1, tick a duration, and roll to expire), sets the message globals, and calls `Show_Event_Message`. Runs once per turn.

## Signature

```c
void Event_Twiddle(void)
```

Per-block pattern: read `*_Player`/`*_Data` into `player_idx`/`m_event_city_idx`, set `m_current_event` + `m_event_player_idx` + `EVNT_MsgDataValue`, pick `m_event_message_type` (`0` human / `4` other for player-facing events, `3`/`99` for world events), apply the effect, `Show_Event_Message`.

## Code walk (block order = asm order)

Line refs are production [EVENTS.c](../../MoM/src/EVENTS.c); cross-checked against `Event_Twiddle.asm`.

1. **Meteor** ([679-714](../../MoM/src/EVENTS.c#L679-L714), asm:26-116) — if the target city changed owner, just clear; else `Apply_Call_The_Void`, tally lost buildings/population/units, `Show_Event_Message`.
2. **Gift** ([718-749](../../MoM/src/EVENTS.c#L718-L749), asm:119-192) — announce; for the human, run the item-grant screen sequence and `Process_Item_Pool`. **See ReMoM deviation (DARKEN_SCREEN).**
3. **Disjunction** ([757-782](../../MoM/src/EVENTS.c#L757-L782), asm:194-240) — zero every player's `Globals[0..NUM_OVERLAND_ENCHANTMENTS)` (`24`), announce.
4. **Marriage** ([786-828](../../MoM/src/EVENTS.c#L786-L828), asm:242-...) — hand the neutral city + its garrison to the player, set it to Trade Goods, recalc, announce.
5. **Earthquake** ([832-870](../../MoM/src/EVENTS.c#L832-L870), asm:...-500) — `Apply_Earthquake`, tally losses, and only announce/process items if something was actually lost.
6. **Pirates** ([874-896](../../MoM/src/EVENTS.c#L874-L896), asm:502-556) — announce, deduct `Pirates_Data` gold from `player_idx`, floor **that** player at 0 ([891](../../MoM/src/EVENTS.c#L891)).
7. **Plague** ([900-955](../../MoM/src/EVENTS.c#L900-L955), asm:558-679) — `Random(10) < population` may drop population (min 2); turn-1 (`es_Starting`, tested live) announces and goes ongoing; ongoing rolls `Random(20)==1 && Duration>3` to end, and force-ends at population `< 2`.
8. **Rebellion** ([959-992](../../MoM/src/EVENTS.c#L959-L992), asm:680-768) — normal-race units go neutral, summoned units dismissed, city → neutral, announce.
9. **Donation** ([996-1017](../../MoM/src/EVENTS.c#L996-L1017), asm:769-804) — `gold_reserve[player] += Donation_Data`, announce.
10. **Depletion** ([1021-1045](../../MoM/src/EVENTS.c#L1021-L1045), asm:805-857) — `EVNT_TargetDepletion__STUB`, `Clear_Terrain_Specials`, announce.
11. **New Mine** ([1049-1073](../../MoM/src/EVENTS.c#L1049-L1073), asm:858-909) — `EVNT_FindNewMineral__STUB`, `TILE_ReplaceMinerals__STUB`, announce.
12. **Population Boom** ([1077-1117](../../MoM/src/EVENTS.c#L1077-L1117), asm:910-968) — turn-1 announce/ongoing; ongoing `Random(20)==1 && Duration>3` ends it.
13-18. **Good Moon / Bad Moon / Conjunction Chaos / Conjunction Nature / Conjunction Sorcery / Mana Short** ([1119-1309](../../MoM/src/EVENTS.c#L1119-L1309), asm:969-1215) — identical shape: `m_event_player_idx = 99`, message `emt_Happened (3)`; turn-1 announce → ongoing (Duration 0); ongoing `Random(20) <= (Duration - 2) && Duration > 3` → end (message `emt_Ended (1)`), then `Duration++`. Each sets its own `m_current_event` id (`et_Good_Moon` … `et_Mana_Short`).

## ReMoM deviations (deliberate — not OG-faithful, not bugs)

- **Gift screen: `DARKEN_SCREEN()` replaces `Gradient_Fill`** ([743](../../MoM/src/EVENTS.c#L743)) — the OG `Gradient_Fill(0,0,319,199,3,…)` (asm:169-180) is replaced by `DARKEN_SCREEN()`, a platform-layer substitute for the darken effect. Intentional.
- **`DBG_trigger_event` reset** ([672](../../MoM/src/EVENTS.c#L672)) — debug hook, not in the asm.

## Faithful (do not "fix")

- **Write-only OG globals `niu_event_twiddle_flag` / `niu_event_twiddle_type_flag`** (declared [199](../../MoM/src/EVENTS.c#L199) / [180](../../MoM/src/EVENTS.c#L180); were `CRP_EVNT_Var_1` / `CRP_EVNT_Var_2`) — zeroed at [674](../../MoM/src/EVENTS.c#L674) (per-turn) and at the Meteor/Gift/Disjunction block heads ([689](../../MoM/src/EVENTS.c#L689)/[721](../../MoM/src/EVENTS.c#L721)/[760](../../MoM/src/EVENTS.c#L760)), matching the asm's `mov …, 0` (asm:24/53/130/202). **Never read anywhere** in the codebase — no drake178 label, purpose unknown (`NIU` = Not In Use). Keep the zero-stores (they're in the asm); revisit the name if `Show_Event_Message` is ever reconstructed to consume one.
- **Multi-turn expiry logic** — moons/conjunctions/Mana-Short end on `Random(20) <= (Duration - 2) && Duration > 3` (asm:990-1006 etc.), Plague/Boom on `Random(20) == 1 && Duration > 3`. The `Duration - 2` guard means they cannot end before Duration reaches 3. Faithful; consistent with `Random` being 1-based (`1..20`).
- **World-event sentinel `m_event_player_idx = 99`** (Disjunction [763](../../MoM/src/EVENTS.c#L763), moons/conjunctions) — matches the asm's `mov …, 99`.

## Related references

- `…\ovr080\Event_Twiddle.asm` — IDA Pro 5.5 disassembly (ground truth, 1222 lines).
- `…\ovr080\Event_Twiddle.c` — Gemini translation of the asm.
- Sibling: [`Determine_Event`](../../MoM/src/EVENTS.c#L221) (s080p01, Wave 8A) — arms the events this function advances.
- Callees: `Army_At_City`, `Army_At_Square_2`, `Apply_Call_The_Void`, `Apply_Earthquake`, `Show_Event_Message`, `Process_Item_Pool`, `Player_Process_Item_Pool`, `Change_City_Ownership`, `Dismiss_Unit`, `Do_City_Calculations`, `Clear_Terrain_Specials`, `EVNT_TargetDepletion__STUB`, `EVNT_FindNewMineral__STUB`, `TILE_ReplaceMinerals__STUB`, `Random` (1-based `1..n`).
- Constants/fields: `events_table->*` (per-event `*_Status`/`*_Duration`/`*_Data`/`*_Player`), `et_Meteor … et_Mana_Short`, `es_Starting=1`/`es_Ongoing=2`/`es_Nothing=0`, `emt_Happened=3`/`emt_Ended=1`, `NUM_OVERLAND_ENCHANTMENTS = 24`, `NEUTRAL_PLAYER_IDX = 5`, `HUMAN_PLAYER_IDX = 0`, `rt_Arcane`, `bt_TradeGoods`, `_human_player_idx`.
- Tracked as **Wave 8B** in [`__TODO-NextTurn.md`](../__TODO-NextTurn.md).
