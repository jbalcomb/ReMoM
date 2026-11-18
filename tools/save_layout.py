#!/usr/bin/env python3
"""
save_layout.py -- byte offsets of the arrays in a MoM SAVEn.GAM, and a small reader/patcher.

The offsets follow the read order of Load_SAVE_GAM() in MoX/src/LOADSAVE.c; field offsets inside
the records come from the generated field maps (STU/src/gd_*_fields.h). Run directly to list each
save's human cities and the human wizard's fortress / casting state:

    python3 tools/save_layout.py assets/SAVE3.GAM [...]
    python3 tools/save_layout.py --make-spell-of-return assets/SAVE6.GAM assets/SAVE_SPELL_OF_RETURN.GAM
    python3 tools/save_layout.py --make-strategic-combat-set assets/MAGIC.SET assets/MAGIC_STRATEGIC_COMBAT.SET
"""
import struct
import sys

NUM_PLAYERS = 6
NUM_HERO_TYPES = 35
SIZEOF_HERO = 12
SIZEOF_WIZARD = 1224
NUM_PLANES = 2
NUM_NODES = 30
NUM_FORTRESSES = 6
NUM_TOWERS = 6
NUM_LAIRS = 102
NUM_ITEMS = 138
NUM_CITIES = 100
SIZEOF_CITY = 114
NUM_UNITS = 1009
SIZEOF_UNIT = 32

# Load_SAVE_GAM() read order
OFS_HEROES = 0
OFS_HEADER = OFS_HEROES + NUM_PLAYERS * NUM_HERO_TYPES * SIZEOF_HERO          # _num_players .. _unit, 8 words
OFS_NUM_CITIES = OFS_HEADER + 8                                                  # _cities (5th word)
OFS_PLAYERS = OFS_HEADER + 16
OFS_WORLD_MAPS = OFS_PLAYERS + NUM_PLAYERS * SIZEOF_WIZARD
OFS_CONNECTIVITY = OFS_WORLD_MAPS + NUM_PLANES * 4800
OFS_LANDMASSES = OFS_CONNECTIVITY + 2 * NUM_PLANES * 96
OFS_NODES = OFS_LANDMASSES + 2400 * NUM_PLANES
OFS_FORTRESSES = OFS_NODES + NUM_NODES * 48
OFS_TOWERS = OFS_FORTRESSES + NUM_FORTRESSES * 4
OFS_LAIRS = OFS_TOWERS + NUM_TOWERS * 4
OFS_ITEMS = OFS_LAIRS + NUM_LAIRS * 24
OFS_CITIES = OFS_ITEMS + NUM_ITEMS * 50
OFS_UNITS = OFS_CITIES + NUM_CITIES * SIZEOF_CITY

# record field offsets (STU/src/gd_wizard_fields.h, gd_fortress_fields.h, gd_city_fields.h)
WIZ_CASTING_COST_REMAINING = 0x004E
WIZ_CASTING_COST_ORIGINAL = 0x0050
WIZ_CASTING_SPELL_IDX = 0x0052
WIZ_NOMINAL_SKILL = 0x0056
WIZ_MANA_RESERVE = 0x025C
FORT_ACTIVE = 0x0003
CITY_NAME, CITY_WX, CITY_WY, CITY_WP, CITY_OWNER, CITY_SIZE, CITY_POPULATION = 0x00, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14

SPL_SPELL_OF_RETURN = 214  # MoM/src/Spellbook.h


def u16(b, o):
    return struct.unpack_from('<H', b, o)[0]


def i16(b, o):
    return struct.unpack_from('<h', b, o)[0]


def player(p):
    return OFS_PLAYERS + p * SIZEOF_WIZARD


def city(c):
    return OFS_CITIES + c * SIZEOF_CITY


def cities(b):
    n = i16(b, OFS_NUM_CITIES)
    out = []
    for c in range(n):
        o = city(c)
        name = b[o + CITY_NAME:o + CITY_NAME + 14].split(b'\0')[0].decode('latin-1')
        out.append(dict(idx=c, name=name, wx=b[o + CITY_WX], wy=b[o + CITY_WY], wp=b[o + CITY_WP],
                        owner=struct.unpack_from('<b', b, o + CITY_OWNER)[0], size=b[o + CITY_SIZE], population=b[o + CITY_POPULATION]))
    return out


def summary(path):
    b = open(path, 'rb').read()
    p0 = player(0)
    human = [c for c in cities(b) if c['owner'] == 0]
    print('%s  (%d bytes; units end at %d)' % (path, len(b), OFS_UNITS + NUM_UNITS * SIZEOF_UNIT))
    print('  cities: %d total, %d human' % (i16(b, OFS_NUM_CITIES), len(human)))
    for c in human:
        print('    #%-3d %-14s size %d pop %-2d  at (%d,%d) plane %d' % (c['idx'], c['name'], c['size'], c['population'], c['wx'], c['wy'], c['wp']))
    print('  human: casting_spell_idx %d, cost remaining %d / %d, Nominal_Skill %d, mana %d, fortress active %d' % (
        u16(b, p0 + WIZ_CASTING_SPELL_IDX), u16(b, p0 + WIZ_CASTING_COST_REMAINING), u16(b, p0 + WIZ_CASTING_COST_ORIGINAL),
        u16(b, p0 + WIZ_NOMINAL_SKILL), i16(b, p0 + WIZ_MANA_RESERVE), struct.unpack_from('<b', b, OFS_FORTRESSES + FORT_ACTIVE)[0]))


UNIT_WX, UNIT_WY, UNIT_WP, UNIT_OWNER = 0x00, 0x01, 0x02, 0x03  # STU/src/gd_unit_fields.h
ST_UNDEFINED = -1


def make_spell_of_return(src, dst, winner_idx=1):
    """
    Copy src to dst in the state Banish_Wizard() (MoM/src/CONQUEST.c) leaves the human in after the
    fortress city falls, with Spell of Return one mana from done: the next turn casts it and opens
    the Spell of Return city picker (Cast_Spell_Of_Return, MoM/src/Spells132.c).
      - the city on the human fortress square goes to winner_idx; the human units there are killed
      - _FORTRESSES[0].active = ST_FALSE
      - _players[0].casting_spell_idx = spl_Spell_Of_Return, casting cost remaining / original = 1
    """
    b = bytearray(open(src, 'rb').read())
    fo = OFS_FORTRESSES
    fwx, fwy, fwp = struct.unpack_from('<bbb', b, fo)
    fallen = [c for c in cities(b) if (c['wx'], c['wy'], c['wp']) == (fwx, fwy, fwp)]
    if len(fallen) != 1 or fallen[0]['owner'] != 0:
        sys.exit('%s: expected one human city on the fortress square (%d,%d,%d), found %r' % (src, fwx, fwy, fwp, fallen))
    struct.pack_into('<b', b, city(fallen[0]['idx']) + CITY_OWNER, winner_idx)
    killed = 0
    for u in range(i16(b, OFS_HEADER + 10)):  # _units
        o = OFS_UNITS + u * SIZEOF_UNIT
        if struct.unpack_from('<bbbb', b, o) == (fwx, fwy, fwp, 0):
            struct.pack_into('<b', b, o + UNIT_OWNER, ST_UNDEFINED)
            killed += 1
    struct.pack_into('<b', b, fo + FORT_ACTIVE, 0)
    p0 = player(0)
    struct.pack_into('<H', b, p0 + WIZ_CASTING_SPELL_IDX, SPL_SPELL_OF_RETURN)
    struct.pack_into('<H', b, p0 + WIZ_CASTING_COST_REMAINING, 1)
    struct.pack_into('<H', b, p0 + WIZ_CASTING_COST_ORIGINAL, 1)
    open(dst, 'wb').write(b)
    print('%s -> %s: %s (#%d) to player %d, %d human units killed, fortress 0 inactive, Spell of Return 1 mana from done' % (
        src, dst, fallen[0]['name'], fallen[0]['idx'], winner_idx, killed))


MAGIC_SET_SIZE = 466                    # MoX/src/MOX_SET.h SETTINGS_FILE_SIZE
MAGIC_SET_STRATEGIC_COMBAT_ONLY = 0x14  # MoX/src/MOX_SET.h s_MAGIC_SET.strategic_combat_only


def make_strategic_combat_set(src, dst):
    """
    Copy a MAGIC.SET with "Strategic Combat Only" on, so an AI attack on a human stack resolves without the
    tactical battle screen (which waits for orders) -- what DeMoM's fixture modes set in code.
    """
    b = bytearray(open(src, 'rb').read())
    if len(b) != MAGIC_SET_SIZE:
        sys.exit('%s: %d bytes, expected %d' % (src, len(b), MAGIC_SET_SIZE))
    struct.pack_into('<H', b, MAGIC_SET_STRATEGIC_COMBAT_ONLY, 1)
    open(dst, 'wb').write(b)
    print('%s -> %s: strategic_combat_only = 1' % (src, dst))


if __name__ == '__main__':
    if len(sys.argv) == 4 and sys.argv[1] == '--make-strategic-combat-set':
        make_strategic_combat_set(sys.argv[2], sys.argv[3])
        sys.exit(0)
    if len(sys.argv) == 4 and sys.argv[1] == '--make-spell-of-return':
        make_spell_of_return(sys.argv[2], sys.argv[3])
        summary(sys.argv[3])
    else:
        for p in sys.argv[1:]:
            summary(p)
