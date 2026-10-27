"""
tables.py -- the record layouts and valid ranges of the LBX data tables (see schema.py).

Every range below comes from how ReMoM's code uses the field: the array it indexes, the switch it feeds, a Random()
argument that must be >= 1, and so on.  The cited lines are the evidence; when a range here turns out to be too
narrow, `python3 -m tools.dummy_lbx --check <original LBX folder>` names the field.

Paths in the citations are relative to the repository root.
"""

import json
import os
import sys

from .schema import Field, Table

# --------------------------------------------------------------------------------------------------------------------
# SPELLDAT.LBX entry 0 -- s_SPELL_DATA, 215 records x 36 bytes (MoX/src/MOM_DAT.h:1031)
#
# The record index is the spell: about 220 spl_* constants compare against it, realm = (idx - 1) / 40
# (MoM/src/Spells128.c:458, NEXTTURN.c:3776) and rarity is the position in blocks of 10 inside a realm.  So the 215
# records stay in order, record 0 stays "None" (type / category / realm = -1), and the realm follows the index.
# --------------------------------------------------------------------------------------------------------------------

# e_SPELL_CASTING_CATEGORY 0..23; 7, 8 and 17 are not defined (switches: Combat.c:8765, Combat.c:9520, OverSpel.c:817).
SPELL_TYPES = tuple(t for t in range(24) if t not in (7, 8, 17))
SPELL_SOUNDS = tuple(range(-20, 0)) + tuple(range(0, 119))   # >= 0: SOUNDFX.LBX (119 entries); < 0: NEWSOUND.LBX via abs() (21)
FANTASTIC_UNITS = (154, 197)                                  # ut_Magic_Spirit .. last unit type (_unit_type_table[198])


def spell_realm(index):
    return -1 if index == 0 else (index - 1) // 40           # 0..5 (sbr_ Nature..Arcane); indexes spellranks[], SPECFX.LBX


class SpellEffectPool:
    """City / global enchantment slots must be unique across spells (reverse lookups: OverSpel.c:142, SPLMASTR.c:1397)."""

    def __init__(self):
        self.city = list(range(26))                           # ce_idx 0..25, NUM_CITY_ENCHANTMENTS (MOM_DAT.h:1764)
        self.globals = list(range(24))                        # oe_idx 0..23, _players[].Globals[24] (MOM_DAT.h:1530)

    def reset(self, rng):
        self.__init__()
        rng.shuffle(self.city)
        rng.shuffle(self.globals)


SPELL_POOL = SpellEffectPool()


# The code treats spell indexes as specific spells (spl_Magic_Spirit = 201 is summoned by index, spell_data_table[spl_*]
# .unit_type is compared directly: Combat.c:16052, UNITSTK.c:1720).  So a spell's *kind* -- category, book, where it
# can be cast, AI group and what it summons / enchants -- is taken from the repo's documented table when present;
# names, costs and sounds stay generated.
SPELL_STRUCTURE_DOC = os.path.join(os.path.dirname(__file__), "..", "..", "doc", "spell_data_table.json")
SPELL_STRUCTURE_FIELDS = ("type", "spell_book_category", "Eligibility", "AI_Group")


def load_spell_structure(path=SPELL_STRUCTURE_DOC):
    try:
        with open(path) as fp:
            rows = json.load(fp)["spell_data_table_at_0x00"]["spell_data_table"]
    except (OSError, KeyError, ValueError) as ex:
        sys.stderr.write("dummy_lbx: no spell structure from %s (%r); spell kinds will be random\n" % (path, ex))
        return None
    structure = []
    for row in rows:
        effect = (row["Param0"] & 0xFF) | ((row["Param1"] & 0xFF) << 8) | ((row["Params2_3"] & 0xFFFF) << 16)
        structure.append(dict({f: row[f] for f in SPELL_STRUCTURE_FIELDS}, effect=effect))
    return structure


SPELL_STRUCTURE = load_spell_structure()


def spell_structure(index, field, fallback):
    """The documented value of a structural field for spell `index`, or `fallback()` when there is no document."""
    if SPELL_STRUCTURE is not None and index < len(SPELL_STRUCTURE):
        return SPELL_STRUCTURE[index][field]
    return fallback()


def gen_spell_type(rng, index, record):
    if SPELL_STRUCTURE is not None:
        return spell_structure(index, "type", None)
    if index == 0:
        return -1
    if index == 1:
        SPELL_POOL.reset(rng)
    choices = [t for t in SPELL_TYPES
               if not (t in (2, 3) and not SPELL_POOL.city) and not (t == 9 and not SPELL_POOL.globals)]
    return rng.choice(choices)


def gen_spell_effect(rng, index, record):
    """The 4-byte union at 0x20, whose meaning depends on `type` (MOM_DAT.h:1046)."""
    if SPELL_STRUCTURE is not None:
        return spell_structure(index, "effect", None)
    t = record["type"]
    if t == 0:                                                # summoning: unit_type (Spells137.c:305)
        return rng.randint(*FANTASTIC_UNITS)
    if t == 5:                                                # special: only Lycanthropy reads a unit type (Spells132.c:1580)
        return 0
    if t in (1, 15):                                          # unit enchantment: one UE_ bit (OverSpel.c:1144)
        return 1 << rng.randrange(32)
    if t in (2, 3):                                           # city enchantment: unique ce_idx
        return SPELL_POOL.city.pop()
    if t in (4, 22):                                          # direct damage: strength byte, no immunities / attributes
        return rng.randint(1, 15)
    if t == 6:                                                # target wizard: Param0 0..3 (OverSpel.c:1463)
        return rng.randint(0, 3)
    if t == 9:                                                # global enchantment: unique oe_idx, also SPECFX / MUSIC entry
        return SPELL_POOL.globals.pop()
    if t in (10, 21):                                         # battlefield / counter: even slot 0..28 (Combat.c:9034)
        return 2 * rng.randint(0, 14)
    if t in (12, 13, 14, 16):                                 # combat effect: one bue_ bit in the low word (Combat.c:9839)
        return 1 << rng.randrange(16)
    return 0                                                  # 11, 18..20, 23: not read


# Battlefield spells that do not set a combat-enchantment slot (Combat.c:9709): their slot holds damage data instead.
BATTLEFIELD_INSTANTS = (109, 115, 149, 150, 194)              # spl_Flame_Strike, Call_Chaos, Mass_Healing, Holy_Word, Death_Spell


def check_spell(record):
    index = record["_index"]
    if record["magic_realm"] != spell_realm(index):
        return "magic_realm %d, but record %d is in realm %d ((index - 1) / 40: Spells128.c:458)" % (record["magic_realm"], index, spell_realm(index))
    if index not in (0, 213) and record["research_cost"] < 1:
        return "research_cost %d must be >= 1 (read signed: Spells128.c:454)" % record["research_cost"]
    index_is_none = record["type"] == -1
    if index_is_none != (record["spell_book_category"] == -1) or index_is_none != (record["magic_realm"] == -1):
        return "type / spell_book_category / magic_realm must be -1 together (record 0 only)"
    t, effect = record["type"], record["effect"]
    if t == 0 and not FANTASTIC_UNITS[0] <= effect <= FANTASTIC_UNITS[1]:
        return "a summoning spell needs a fantastic unit type %d..%d in the effect slot, got %d" % (FANTASTIC_UNITS + (effect,))
    # type 5 (special): the slot's meaning is per spell (e.g. Lycanthropy's unit type, Spells132.c:1580); no general rule.
    if t in (2, 3) and not 0 <= effect <= 25:
        return "city enchantment index %d outside 0..25" % effect
    if t == 9 and not 0 <= effect <= 23:
        return "global enchantment index %d outside 0..23" % effect
    if t in (10, 21) and index not in BATTLEFIELD_INSTANTS and (effect % 2 or not 0 <= effect <= 28):
        return "combat enchantment slot %d is not an even 0..28" % effect
    if t == 6 and not 0 <= effect <= 3:
        return "target-wizard parameter %d outside 0..3" % effect
    return None


SPELLDAT = Table(36, [
    Field(0x00, "text", "name", size=19, gen=lambda rng, i, r: "None" if i == 0 else "Spell %d" % i),   # _fstrcpy'd: Combat.c:10660
    Field(0x13, "i8", "AI_Group", 0, 90,                      # indexes possessed_categories[92] (AISPELL.c:79)
          gen=lambda rng, i, r: spell_structure(i, "AI_Group", lambda: rng.randint(0, 90))),
    Field(0x14, "i8", "AI_Value", 0, 0),                      # never read
    Field(0x15, "i8", "type", values=(-1,) + SPELL_TYPES, gen=gen_spell_type),
    Field(0x16, "i8", "spell_book_category", -1, 5,           # sbc_ 0..5 (switch: Spellbook.c:499)
          gen=lambda rng, i, r: spell_structure(i, "spell_book_category", lambda: -1 if i == 0 else rng.randint(0, 5))),
    Field(0x17, "i8", "magic_realm", -1, 5, gen=lambda rng, i, r: spell_realm(i)),
    Field(0x18, "i8", "Eligibility", -2, 5,                   # -2/-1 combat, 1 overland, 5 neither, else both (Spellbook.c:411)
          gen=lambda rng, i, r: spell_structure(i, "Eligibility", lambda: rng.randint(-2, 5))),
    Field(0x1A, "i16", "casting_cost", 1, 32767,              # 0 divides by zero (Spellbook.c:1106); originals 5..5000
          gen=lambda rng, i, r: rng.randint(5, 5000)),
    Field(0x1C, "i16", "research_cost",                       # > 0 read signed (Spells128.c:454); Spell of Mastery (213)
          gen=lambda rng, i, r: rng.randint(20, 15000)),      # is read unsigned (INITGAME.c:874): original 60000 = -5536
    Field(0x1E, "i8", "Sound", values=SPELL_SOUNDS),          # SBookScr.c:1210, Combat.c:15857
    Field(0x20, "u32", "effect", gen=gen_spell_effect),       # union, checked against `type` by check_spell
], count=215, check=check_spell)

# --------------------------------------------------------------------------------------------------------------------
# ITEMPOW.LBX entry 0 -- s_ITEM_POWER, 67 records x 30 bytes (MOM_DAT.h:1152)
#
# The index decides what a power is (MoM/src/ItemMake.c:1658-1689): 0..32 are the basic bonuses in seven contiguous
# groups (the bonus is taken from the index), 33..64 are specials whose `powers` bit is OR'd into the item, 65 is
# Spell Charges (ItemMake.c:891) and 66 is unused.
# --------------------------------------------------------------------------------------------------------------------

ITEM_POWER_GROUPS = ((0, 5, "Attack"), (6, 11, "Defense"), (12, 14, "To Hit"), (15, 18, "Move"),
                     (19, 24, "Resist"), (25, 28, "Skill"), (29, 32, "Save"))


def item_power_group(index):
    for group, (lo, hi, label) in enumerate(ITEM_POWER_GROUPS):
        if lo <= index <= hi:
            return group, index - lo + 1, label
    return None


def gen_item_power_name(rng, index, record):
    basic = item_power_group(index)
    if basic:
        # The game overwrites name[3] with NUL and prints name[4..] (ItemMake.c:514), so " +N Label" with one digit.
        return " +%d %s" % (basic[1], basic[2])
    return "Empty" if index == 66 else "Power %d" % index


def gen_item_power_type(rng, index, record):
    basic = item_power_group(index)
    if basic:
        return basic[0]                                       # 0..6: indexes Attrib_Suffixes[7] (ItemMake.c:742)
    return rng.choice((7, 8, 9))                              # 7 and 8 are mutually exclusive groups (ItemMake.c:1226)


def check_item_power(record):
    basic = record["type"] <= 6
    name = record["name"]
    if basic and not (len(name) >= 5 and name[:2] == " +" and name[2].isdigit() and name[3] == " "):
        return "a basic power's name must look like ' +N Label' (ItemMake.c:514), got %r" % name
    if not basic and bin(record["powers"]).count("1") > 1:
        return "a special power sets at most one powers bit"
    return None


ITEMPOW = Table(30, [
    Field(0x00, "text", "name", size=18, gen=gen_item_power_name),
    Field(0x12, "i16", "item_types", 1, 0x3FF),               # bit per it_ 0..9 (ItemMake.c:1387)
    Field(0x14, "i16", "cost", 0, 32767, gen=lambda rng, i, r: rng.randint(50, 2000)),   # >= 0 (ItemMake.c:904)
    Field(0x16, "i8", "type", 0, 9, gen=gen_item_power_type),
    Field(0x17, "i8", "magic_realm", 0, 4),                   # indexes spellranks[5] (ItemMake.c:1818)
    Field(0x18, "i16", "Books_or_Bonus", 0, 22, gen=lambda rng, i, r: rng.randint(0, 7)),  # 22 = Merging, unreachable
    Field(0x1A, "u32", "powers",                              # 33..64: one ip_* bit each, in order; 0 otherwise
          gen=lambda rng, i, r: (1 << (i - 33)) if 33 <= i <= 64 else 0),
], count=67, check=check_item_power)

# --------------------------------------------------------------------------------------------------------------------
# ITEMDATA.LBX entry 0 -- s_ITEM_DATA, 250 prefab items x 56 bytes (MOM_DAT.h:1169).  The first 50 bytes are copied
# as an s_ITEM after moves *= 2 (ItemMake.c:2213); one is drawn with Random(250) - 1 (ItemMake.c:2445).
# --------------------------------------------------------------------------------------------------------------------

# it_ type -> (Slot_Type (ItemMake.c:1621), icon range in item_icons_seg[116] (MOM_DAT.h:3059, ItemView.c:297))
ITEM_TYPES = {
    0: (1, 0, 8),       # Sword
    1: (1, 9, 19),      # Mace
    2: (1, 20, 28),     # Axe
    3: (2, 29, 37),     # Bow
    4: (4, 38, 46),     # Staff
    5: (4, 107, 115),   # Wand
    6: (6, 72, 106),    # Misc
    7: (5, 62, 71),     # Shield
    8: (5, 47, 54),     # Chainmail
    9: (5, 55, 61),     # Platemail
}

# Spell indexes an item can carry: combat-castable is decided by Eligibility, which is random here, so any 1..127
# record is a valid index; the int8 field makes > 127 negative and breaks the `> 0` test (UnitView.c:2465).
ITEM_SPELLS = (1, 127)


def check_item(record):
    slot, lo, hi = ITEM_TYPES[record["type"]]
    if record["Slot_Type"] != slot:
        return "Slot_Type %d does not match item type %d (needs %d)" % (record["Slot_Type"], record["type"], slot)
    if not lo <= record["icon_idx"] <= hi:
        return "icon %d is outside item type %d's icons %d..%d" % (record["icon_idx"], record["type"], lo, hi)
    if (record["embed_spell_idx"] == 0) != (record["embed_spell_cnt"] == 0):
        return "embed_spell_cnt must be 0 exactly when there is no embedded spell"
    return None


def gen_item_type(rng, index, record):
    return rng.randrange(10)


ITEMDATA = Table(56, [
    Field(0x00, "text", "name", size=30, gen=lambda rng, i, r: "Item %d" % i),
    Field(0x1E, "i8", "icon_idx", 0, 115, gen=lambda rng, i, r: rng.randint(*ITEM_TYPES[r["type"]][1:])),
    Field(0x1F, "i8", "unused_1F", 0, 0),
    Field(0x20, "i8", "Slot_Type", values=(1, 2, 4, 5, 6), gen=lambda rng, i, r: ITEM_TYPES[r["type"]][0]),
    Field(0x21, "i8", "type", 0, 9, gen=gen_item_type, first=True),
    Field(0x22, "i16", "cost", 1, 32767, gen=lambda rng, i, r: rng.randint(150, 9100)),  # <= 0 = empty slot (ItemMake.c:2476)
    Field(0x24, "i8", "attack", 0, 127, gen=lambda rng, i, r: rng.randint(0, 6)),
    Field(0x25, "i8", "tohit", 0, 127, gen=lambda rng, i, r: rng.randint(0, 3)),
    Field(0x26, "i8", "defense", 0, 127, gen=lambda rng, i, r: rng.randint(0, 6)),
    Field(0x27, "i8", "moves", 0, 63, gen=lambda rng, i, r: rng.randint(0, 4)),   # doubled on load into an int8
    Field(0x28, "i8", "resistance", 0, 127, gen=lambda rng, i, r: rng.randint(0, 6)),
    Field(0x29, "i8", "spell_skill", 0, 127, gen=lambda rng, i, r: rng.choice((0, 5, 10, 15, 20))),
    Field(0x2A, "i8", "spell_save", 0, 127, gen=lambda rng, i, r: rng.randint(0, 4)),
    Field(0x2B, "i8", "embed_spell_idx", 0, ITEM_SPELLS[1],
          gen=lambda rng, i, r: rng.randint(*ITEM_SPELLS) if rng.random() < 0.2 else 0),
    Field(0x2C, "i16", "embed_spell_cnt", 0, 4,
          gen=lambda rng, i, r: rng.randint(1, 4) if r["embed_spell_idx"] else 0),
    Field(0x2E, "u32", "Powers", gen=lambda rng, i, r: (1 << rng.randrange(32)) if rng.random() < 0.5 else 0),
    Field(0x32, "u8", "spellranks", 0, 22, count=5,           # minimum books per realm (ItemMake.c:2452); 22 = Merging
          gen=lambda rng, i, r: rng.randint(0, 3)),
    Field(0x37, "u8", "flag", 0, 1),
], count=250, check=check_item)

# --------------------------------------------------------------------------------------------------------------------
# BUILDDAT.LBX entry 0 -- s_BLDG_DATA, 36 records x 52 bytes (MOM_DAT.h:1553).  Record 0 is "None" with -1 links;
# the loops that follow links start at 1 (ProdScr.c:880, MAPGEN.c:5778).
# --------------------------------------------------------------------------------------------------------------------

NUM_BUILDINGS = 36
TERRAIN_PREREQUISITES = (101, 104, 110, 200)                  # e_BUILDING_TYPE_PREREQUISITE (MOM_DAT.h:660)


def gen_reqd_bldg_1(rng, index, record):
    if index == 0:
        return -1
    roll = rng.random()
    if roll < 0.1:
        return rng.choice(TERRAIN_PREREQUISITES)
    if roll < 0.5 and index > 3:
        return rng.randint(3, index - 1)                      # an earlier building: no prerequisite cycles
    return 0


def check_building(record):
    index = record["_index"]
    if index == 0:
        if (record["reqd_bldg_1"], record["reqd_bldg_2"], record["replace_bldg"]) != (-1, -1, -1):
            return "record 0 (None) keeps -1 prerequisites and replacement"
        return None
    r1 = record["reqd_bldg_1"]
    if not (0 <= r1 < NUM_BUILDINGS or r1 in TERRAIN_PREREQUISITES):
        return "reqd_bldg_1 %d is neither a building 0..35 nor a terrain code %s (indexes bldg_status[36]: City_ovr55.c:1251)" % (r1, TERRAIN_PREREQUISITES)
    if not 0 <= record["reqd_bldg_2"] < NUM_BUILDINGS:
        return "reqd_bldg_2 %d is not a building 0..35 (indexed unguarded: ProdScr.c:692)" % record["reqd_bldg_2"]
    rep = record["replace_bldg"]
    if rep != -1 and not (1 <= rep < NUM_BUILDINGS and rep != index):
        return "replace_bldg %d must be -1 or another building 1..35 (City_ovr55.c:2264)" % rep
    if 3 <= index <= 34 and not 0 <= record["shape"] <= 3:
        return "shape %d: buildings 3..34 need e_BUILDING_SHAPE 0..3 (cityscape_rubble_seg[4]: CITYSCAP.c:1415)" % record["shape"]
    return None


BUILDDAT = Table(52, [
    Field(0x00, "text", "name", size=20, gen=lambda rng, i, r: "None" if i == 0 else "Building %d" % i),
    Field(0x14, "i16", "reqd_bldg_1", -1, 200, gen=gen_reqd_bldg_1),
    Field(0x16, "i16", "reqd_bldg_2", -1, NUM_BUILDINGS - 1,
          gen=lambda rng, i, r: -1 if i == 0 else (rng.randint(3, i - 1) if i > 3 and rng.random() < 0.2 else 0)),
    Field(0x18, "i16", "replace_bldg", -1, NUM_BUILDINGS - 1, gen=lambda rng, i, r: -1),
    Field(0x1A, "i16", "Grants20XP", 0, 1),                    # not read
    Field(0x1C, "i16", "Grants60XP", 0, 1),                    # not read
    Field(0x1E, "i16", "Alchemist", 0, 1),                     # not read
    Field(0x20, "i16", "maintenance_cost", -10, 10,            # only abs() is used; one 9-px coin icon each (UnitStat.c:980)
          gen=lambda rng, i, r: rng.randint(-6, 0)),
    Field(0x22, "i16", "AIVal_Growth", 0, 15),                 # not read
    Field(0x24, "i16", "Unknown_24h", 0, 0),                   # not read
    Field(0x26, "i16", "Unknown_26h", 0, 15),                  # not read
    Field(0x28, "i16", "AIVal_Religion", 0, 15),               # not read
    Field(0x2A, "i16", "AIVal_Research", 0, 15),               # not read
    Field(0x2C, "i16", "construction_cost", 1, 1000,           # coin icons cost/10, sell cost/3 (City_ovr55.c:1341)
          gen=lambda rng, i, r: rng.randint(30, 1000)),
    Field(0x2E, "i16", "Unknown_2Eh", 0, 0),                   # not read
    Field(0x30, "i16", "shape", -1, 3,
          gen=lambda rng, i, r: rng.randint(0, 3) if 3 <= i <= 34 else -1),
    Field(0x32, "i16", "Category", 0, 9),                      # ai_build_*_weights_table[10] (AIBUILD.c:329)
], count=NUM_BUILDINGS, check=check_building)

# --------------------------------------------------------------------------------------------------------------------
# TERRSTAT.LBX -- entry 0: 770 terrain types x 6 half-move costs (record index = terrain value in p_world_map;
# copied at MAPGEN.c:6758).  -1 is impassable (INF, MovePath.c:94); any other negative cost makes the path relaxation
# loop forever (MovePath.c:235).  Entry 1: the 14 x 14 race-unrest table (LOADER.c:319); x10 into unrest percent.
# --------------------------------------------------------------------------------------------------------------------

MOVE_COSTS = (-1, 2, 4, 6, 8)
TERRSTAT_MOVES = Table(6, [
    Field(i, "i8", name, -1, 127, gen=lambda rng, idx, r: rng.choice(MOVE_COSTS))
    for i, name in enumerate(("unused_mode", "walking", "forester", "mountaineer", "swimming", "sailing"))
], count=770)

TERRSTAT_UNREST = Table(196, [
    Field(0, "i8", "unrest", -128, 127, count=196, gen=lambda rng, i, r: rng.randint(-2, 4)),
], count=1)

# --------------------------------------------------------------------------------------------------------------------
# TERRTYPE.LBX entry 0 -- one int16 array of 5 x 256 words used for autotiling (MAPGEN.c:3729).  Record 0 maps an
# 8-neighbour mask to a shore tile 2..161 (later offset into the desert and tundra ranges: MAPGEN.c:3838, :4379);
# record 1 maps it to a mountain tile 0xA4 or 0x103..0x112 (MAPGEN.c:4203).  Records 2..4 are never read.
# --------------------------------------------------------------------------------------------------------------------

MOUNTAIN_TILES = (0xA4,) + tuple(range(0x103, 0x113))


def gen_terrtype(rng, index, record):
    if index == 0:
        return rng.randint(2, 161)
    if index == 1:
        return rng.choice(MOUNTAIN_TILES)
    return 0


def check_terrtype(record):
    index, words = record["_index"], record["words"][1:]      # word 0 of each record is never read (masks are 1..255)
    if index == 0 and not all(2 <= w <= 161 for w in words):
        return "shore tiles must be 2..161 (tt_Shore1_Fst..Lst)"
    if index == 1 and not all(w in MOUNTAIN_TILES for w in words):
        return "mountain tiles must be 0xA4 or 0x103..0x112"
    return None


TERRTYPE = Table(512, [Field(0, "i16", "words", count=256, gen=gen_terrtype)], count=5, check=check_terrtype)

# --------------------------------------------------------------------------------------------------------------------
# DIPLOMSG.LBX -- entry 0: 74 x 6 (s_DIPLOMSG_RECORD, MOM_DAT.h:1353); entry 1: 1110 = 74 x 15 message texts.
# IDK_count feeds Random(): 0 exits the game ("RND no 0's", random.c:309), > 15 reads into the next row (DIPLOMAC.c:5209).
# --------------------------------------------------------------------------------------------------------------------


def check_diplomsg(record):
    if record["_index"] > 0 and record["IDK_count"] < 1:
        return "IDK_count 0 feeds Random(0), which exits the game (random.c:309)"
    return None


DIPLOMSG_RECORDS = Table(6, [
    Field(0, "i16", "IDK_mood", 0, 2),                         # Start_Diplomacy_Music (DIPLOMAC.c:1977)
    Field(2, "i16", "IDK_group", 0, 7, gen=lambda rng, i, r: rng.randint(0, 5)),   # switches break on default (DIPLOMAC.c:2032)
    Field(4, "i16", "IDK_count", 0, 15, gen=lambda rng, i, r: rng.randint(1, 15)),
], count=74, check=check_diplomsg)

# --------------------------------------------------------------------------------------------------------------------
# HLPENTRY.LBX -- every entry: 10-byte s_HELP_FIELD records (MoX/src/Input.h:30) at most 50 per entry (_help_entries
# is 500 bytes).  help_idx -1 disables the field, else it is a HELP.LBX entry-2 record (Input.c:1635).
# --------------------------------------------------------------------------------------------------------------------

NUM_HELP_RECORDS = 1614


def gen_help_rect(rng, index, record):
    x1, y1 = rng.randint(0, 300), rng.randint(0, 180)
    record["_rect"] = (x1, y1, rng.randint(x1, 319), rng.randint(y1, 199))
    return record["_rect"][0]


def check_help_field(record):
    if record["x1"] > record["x2"] or record["y1"] > record["y2"]:
        return "help rectangle (%d,%d)-(%d,%d) is inverted" % (record["x1"], record["y1"], record["x2"], record["y2"])
    return None


HLPENTRY_FIELDS = Table(10, [
    Field(0, "i16", "help_idx", -1, NUM_HELP_RECORDS - 1),
    Field(2, "i16", "x1", 0, 319, gen=gen_help_rect),
    Field(4, "i16", "y1", 0, 199, gen=lambda rng, i, r: r["_rect"][1]),
    Field(6, "i16", "x2", 0, 319, gen=lambda rng, i, r: r["_rect"][2]),
    Field(8, "i16", "y2", 0, 199, gen=lambda rng, i, r: r["_rect"][3]),
], check=check_help_field)

# --------------------------------------------------------------------------------------------------------------------
# HELP.LBX entry 2 -- s_HELP_DATA, 1614 records x 1048 bytes (MoX/src/Input.h:95, read in MoX/src/TEXTBOX.c).
# lbx_name "" = no picture.  Include_Entry: 0 ends the chain, -1 = also show the next record, N = also show record N;
# a chain must end and stay <= 10 records (help_pict_segs[10], TEXTBOX.c:74).  The generator writes 0 everywhere.
# --------------------------------------------------------------------------------------------------------------------


def check_help(record):
    if record["lbx_name"] and not 0 <= record["lbx_entry"] < 200:
        return "lbx_entry %d is not an entry of %s" % (record["lbx_entry"], record["lbx_name"])
    return None


HELP_DATA = Table(1048, [
    Field(0, "text", "title", size=30, gen=lambda rng, i, r: "Help %d" % i),
    Field(30, "text", "lbx_name", size=14, gen=lambda rng, i, r: ""),
    Field(44, "i16", "lbx_entry", gen=lambda rng, i, r: 0),
    Field(46, "i16", "Include_Entry", -1, NUM_HELP_RECORDS - 1, gen=lambda rng, i, r: 0),
    Field(48, "text", "help_text", size=1000, gen=lambda rng, i, r: "Placeholder help text %d." % i),
], count=NUM_HELP_RECORDS, check=check_help)

# --------------------------------------------------------------------------------------------------------------------
# Text tables: each record is one NUL-terminated string the size of the record.
# --------------------------------------------------------------------------------------------------------------------


def text_table(size, count, label):
    return Table(size, [Field(0, "text", "text", size=size, gen=lambda rng, i, r: "%s %d" % (label, i))], count=count)


CITYNAME = Table(280, [                                       # one record per race: 20 names of 14 characters
    Field(n * 14, "text", "name_%d" % n, size=14, gen=(lambda n: lambda rng, i, r: "City %d-%d" % (i, n))(n))
    for n in range(20)
], count=14)

HERODATA = Table(12, [], count=125)                            # not read by ReMoM; zero-filled

# (archive, entry) -> Table.  HLPENTRY.LBX is handled per archive: every entry holds HLPENTRY_FIELDS records.
TABLES = {
    ("SPELLDAT.LBX", 0): SPELLDAT,
    ("BUILDDAT.LBX", 0): BUILDDAT,
    ("ITEMPOW.LBX", 0): ITEMPOW,
    ("ITEMDATA.LBX", 0): ITEMDATA,
    ("TERRSTAT.LBX", 0): TERRSTAT_MOVES,
    ("TERRSTAT.LBX", 1): TERRSTAT_UNREST,
    ("TERRTYPE.LBX", 0): TERRTYPE,
    ("DIPLOMSG.LBX", 0): DIPLOMSG_RECORDS,
    ("DIPLOMSG.LBX", 1): text_table(200, 1110, "Diplomsg"),
    ("HELP.LBX", 2): HELP_DATA,
    ("CITYNAME.LBX", 0): CITYNAME,
    ("HERODATA.LBX", 0): HERODATA,
    ("BUILDESC.LBX", 0): text_table(200, 36, "Buildesc"),
    ("DESC.LBX", 0): text_table(110, 241, "Desc"),
    ("EVENTMSG.LBX", 0): text_table(200, 126, "Eventmsg"),
    ("LISTDAT.LBX", 0): text_table(200, 75, "Listdat"),
    ("MESSAGE.LBX", 0): text_table(150, 100, "Message"),
    ("NAMES.LBX", 0): text_table(13, 200, "Names"),
}
RECORD_ARCHIVES = {"HLPENTRY.LBX": HLPENTRY_FIELDS}
