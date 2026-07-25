"""
lbx.py — parse Magic-the-Gathering-era LBX archives and their FLIC sprite headers.

Format (ported from tools/lbx_extract.c and MoX/src/FLIC_Draw.h):

  LBX header (0x200 bytes):
    0x00 u16  num_entries
    0x02 u16  magic = 0xFEAD
    0x04 u16  file_type
    0x08      offset table: (num_entries + 1) u32 LE; entry N = data[off[N]:off[N+1]]
    0x200     name table: 32-byte records, 9-char NUL-terminated name (only for records
              that fit below the first data offset)

  FLIC/sprite header (first 16 bytes of a sprite entry):
    0x00 i16  width
    0x02 i16  height
    0x06 i16  frame_count

Non-sprite entries (palettes, sounds, raw data) have no valid FLIC header; parse_flic()
returns None for them, so the caller can flag kind=sprite vs other.
"""

LBX_MAGIC = 0xFEAD
LBX_HDR_SIZE = 512
LBX_NAME_RECORD = 32
LBX_NAME_LEN = 9


def _u16(d, o):
    return d[o] | (d[o + 1] << 8)


def _i16(d, o):
    v = _u16(d, o)
    return v - 0x10000 if v & 0x8000 else v


def _u32(d, o):
    return d[o] | (d[o + 1] << 8) | (d[o + 2] << 16) | (d[o + 3] << 24)


def _name(d, entry_num):
    off = LBX_HDR_SIZE + entry_num * LBX_NAME_RECORD
    out = []
    for i in range(LBX_NAME_LEN):
        c = d[off + i]
        if c == 0:
            break
        out.append(chr(c))
    return "".join(out)


def parse_lbx(data):
    """Return [(entry_num, name, start, length)] or None if not a valid LBX."""
    if len(data) < LBX_HDR_SIZE:
        return None
    if _u16(data, 0x02) != LBX_MAGIC:
        return None
    num = _u16(data, 0x00)
    if num == 0:
        return None
    off_table_end = 0x08 + (num + 1) * 4
    if off_table_end > len(data):
        return None
    offsets = [_u32(data, 0x08 + i * 4) for i in range(num + 1)]
    first = offsets[0]
    name_records = 0
    if first > LBX_HDR_SIZE:
        name_records = min(num, (first - LBX_HDR_SIZE) // LBX_NAME_RECORD)
    entries = []
    for i in range(num):
        start, end = offsets[i], offsets[i + 1]
        if end < start or end > len(data):
            entries.append((i, _name(data, i) if i < name_records else "", start, 0))
            continue
        name = _name(data, i) if i < name_records else ""
        entries.append((i, name, start, end - start))
    return entries


def parse_flic(data, start, length):
    """Return a dict of FLIC sprite header fields if the entry looks like a sprite, else None.
    Keys: width, height, frames, loop, has_palette, store, remap.
      - loop        frame the animation loops back to (0x08)
      - has_palette 1 if the sprite carries its own palette (palette_header_offset 0x0E != 0),
                    else 0 (uses the shared game palette)
      - store       frame_type / "full store flag" (0x10) — full vs delta frame encoding
      - remap       remap-colors flag (0x11)
    DOS EMS paging fields (0x0A-0x0D) and the runtime current_frame (0x04) are intentionally omitted."""
    if length < 16 or start + 16 > len(data):
        return None
    width = _i16(data, start + 0)
    height = _i16(data, start + 2)
    frames = _i16(data, start + 6)
    # Plausibility gate — non-sprite entries have garbage here.
    if not (0 < width <= 640 and 0 < height <= 480 and 0 < frames <= 4000):
        return None
    return {
        "width": width,
        "height": height,
        "frames": frames,
        "loop": _i16(data, start + 0x08),
        "has_palette": 1 if _i16(data, start + 0x0E) != 0 else 0,
        "store": data[start + 0x10] if length > 0x10 else 0,
        "remap": data[start + 0x11] if length > 0x11 else 0,
    }
