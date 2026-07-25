"""
test_lbx.py — hermetic parse test over a synthetic LBX built in memory.

Two entries: a valid FLIC sprite (40x15, 2 frames) and a too-short 'other' entry.
Verifies header parsing, name-table reads, and the sprite/other classification.
"""
from ..lbx import parse_flic, parse_lbx


def _set_u16(buf, off, val):
    buf[off] = val & 0xFF
    buf[off + 1] = (val >> 8) & 0xFF


def _set_u32(buf, off, val):
    buf[off] = val & 0xFF
    buf[off + 1] = (val >> 8) & 0xFF
    buf[off + 2] = (val >> 16) & 0xFF
    buf[off + 3] = (val >> 24) & 0xFF


def build_synthetic_lbx():
    # layout: 512-byte header, 2x32 name records at 0x200, entry0 (FLIC) at 576, entry1 at 596
    off0, off1, off2 = 576, 596, 606
    buf = bytearray(off2)
    _set_u16(buf, 0x00, 2)        # num_entries
    _set_u16(buf, 0x02, 0xFEAD)   # magic
    _set_u16(buf, 0x04, 0)        # file_type
    _set_u32(buf, 0x08, off0)     # offset[0]
    _set_u32(buf, 0x0C, off1)     # offset[1]
    _set_u32(buf, 0x10, off2)     # offset[2]
    buf[0x200:0x200 + 6] = b"SPRITE"     # name record 0
    buf[0x220:0x220 + 4] = b"DATA"       # name record 1
    # entry 0: FLIC header — width=40, height=15, frame_count=2, loop=1, own palette, remap on
    _set_u16(buf, off0 + 0x00, 40)
    _set_u16(buf, off0 + 0x02, 15)
    _set_u16(buf, off0 + 0x06, 2)
    _set_u16(buf, off0 + 0x08, 1)      # loop_frame
    _set_u16(buf, off0 + 0x0E, 26)     # palette_header_offset != 0 -> has_palette
    buf[off0 + 0x10] = 1               # store / frame_type
    buf[off0 + 0x11] = 1               # remap flag
    # entry 1: 10 bytes, too short for a FLIC header -> 'other'
    return bytes(buf)


def test_parse_entries_and_names():
    entries = parse_lbx(build_synthetic_lbx())
    assert entries is not None and len(entries) == 2
    assert entries[0][1] == "SPRITE" and entries[1][1] == "DATA"
    assert entries[0][2] == 576 and entries[0][3] == 20   # start, length
    assert entries[1][3] == 10


def test_flic_sprite_classification():
    data = build_synthetic_lbx()
    entries = parse_lbx(data)
    s0, l0 = entries[0][2], entries[0][3]
    s1, l1 = entries[1][2], entries[1][3]
    assert parse_flic(data, s0, l0) == {
        "width": 40, "height": 15, "frames": 2, "loop": 1,
        "has_palette": 1, "store": 1, "remap": 1,
    }  # sprite
    assert parse_flic(data, s1, l1) is None          # too short -> other


def test_not_an_lbx():
    assert parse_lbx(b"\x00" * 600) is None           # wrong magic
    assert parse_lbx(b"\x00" * 4) is None              # too small


def _main():
    tests = [v for k, v in sorted(globals().items()) if k.startswith("test_") and callable(v)]
    failed = 0
    for t in tests:
        try:
            t()
            print(f"PASS {t.__name__}")
        except AssertionError as e:
            failed += 1
            print(f"FAIL {t.__name__}: {e}")
    print(f"\n{len(tests) - failed}/{len(tests)} passed")
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(_main())
