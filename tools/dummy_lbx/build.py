"""
build.py -- generate stand-in .LBX files (noise pictures, noise sounds, placeholder data) for running ReMoM and
HeMoM without the original Master of Magic data.

The shape comes from tools/fields/assets.fwv (the asset catalog built from the original files): every archive gets
the same entry count and names, every picture the same width / height / frame count, every data table the same
record count and record size.  Only the content is invented:

  - pictures: valid FLIC files of random pixels, each exactly the original entry's size (the game sizes its buffers
              from those): raw noise where it fits, runs of one colour where space is tight, transparent columns last
  - sounds:   0xDEAF sound entries -- type 2 = a short noise VOC, type 1 = a minimal silent XMIDI
  - data:     every field of every record drawn from the range the game code accepts (tables.py), and checked
              against it on the way out (schema.py)
  - anything else (raw tile sheets, cursors, drivers, ...): random bytes of the original length

Usage (from the repo root):
    python3 -m tools.dummy_lbx OUT_DIR [--seed N]
then point the game at it:
    REMOM_DATA_DIR=OUT_DIR ./HeMoM --newgame

The output is stand-in data for development and tests.  It is not Master of Magic and does not make the game
playable in any meaningful sense; numbers in the tables are placeholders.
"""

import argparse
import os
import random
import struct
import sys

from tools.field_catalog import fwv

from .check import check_folder
from .tables import RECORD_ARCHIVES, TABLES

LBX_MAGIC = 0xFEAD
LBX_HDR_SIZE = 0x200
LBX_NAME_RECORD = 32
SOUND_MAGIC = 0xDEAF
SOUND_HDR_SIZE = 16

CATALOG = os.path.join(os.path.dirname(__file__), "..", "fields", "assets.fwv")

# Archives whose entries are 0xDEAF sound entries (catalog kind "other").  MUSIC is XMIDI (type 1), the rest VOC (type 2).
SOUND_ARCHIVES = {"SOUNDFX.LBX": 2, "NEWSOUND.LBX": 2, "CMBTSND.LBX": 2, "INTROSFX.LBX": 2, "INTROSND.LBX": 2, "MUSIC.LBX": 1}

def put_text(record, offset, size, text):
    raw = text.encode("ascii")[: size - 1]
    record[offset:offset + len(raw)] = raw


def table_for(lbx, entry):
    """The record layout of a data-table entry, or None if the entry is not a data table."""
    if (lbx, entry) in TABLES:
        return TABLES[(lbx, entry)]
    return RECORD_ARCHIVES.get(lbx)


def build_data_table(table, length, where, rng):
    """A data table filling exactly `length` bytes: (count, size) header, then range-checked records."""
    size = table.record_size
    count, rest = divmod(length - 4, size)
    if rest or (table.count is not None and count != table.count):
        raise ValueError("%s: %d bytes is not 4 + %s x %d" % (where, length, table.count or "n", size))
    out = bytearray(struct.pack("<hh", count, size))
    for index in range(count):
        out += table.build_record(rng, index, where)
    return bytes(out)


def encode_column(height, budget, rng):
    """One column of a frame in at most `budget` bytes: raw noise if it fits, else runs of one colour, else skipped."""
    if budget >= height + 4:
        return bytes((0x00, height + 2, height, 0)) + bytes(rng.randrange(1, 224) for _ in range(height))
    for run in range(2, 33):
        runs = -(-height // run)
        if 4 + 2 * runs <= budget and 2 * runs <= 253:
            body = bytearray()
            left = height
            while left > 0:
                length = min(run, left)
                body += bytes((224 + length - 1, rng.randrange(1, 224)))
                left -= length
            return bytes((0x80, len(body) + 2, len(body), 0)) + bytes(body)
    return b"\xff"                                                         # transparent column


def build_frame(width, height, budget, rng):
    """A complete frame (lead byte + one packet per column) in at most `budget` bytes."""
    budget -= 1
    per_column = budget // max(1, width)
    columns = [encode_column(height, per_column, rng) for _ in range(width)]
    if all(c == b"\xff" for c in columns) and width > 0:
        # Too little room for every column: draw as many 32-pixel-run columns as fit, evenly spaced, skip the rest.
        sample = encode_column(height, 4 + 2 * (-(-height // 32)), rng)
        drawable = max(0, (budget - width) // max(1, len(sample) - 1))
        step = (width / drawable) if drawable else 0
        picked = {int(i * step) for i in range(drawable)} if drawable else set()
        columns = [encode_column(height, len(sample), rng) if i in picked else b"\xff" for i in range(width)]
    return b"\x01" + b"".join(columns)


def build_flic(width, height, frames, loop, has_palette, store, length, rng):
    """A FLIC picture of random pixels, exactly `length` bytes (the original entry's size: the game sizes its buffers from it)."""
    frames = max(1, frames)
    height = min(height, 253)                                              # packet sizes are one byte
    header_end = 0x12 + 4 * (frames + 1)
    palette = b""
    palette_offset = 0
    if has_palette:
        count = max(0, min(256, (length - header_end - 8 - frames * (width + 1)) // 3))
        palette_offset = header_end
        colors = bytes(rng.randrange(64) for _ in range(count * 3))         # VGA DAC values are 6-bit
        palette = struct.pack("<hhhh", palette_offset + 8, 0, count, 0) + colors
    base = header_end + len(palette)
    frame_budget = (length - base) // frames
    body = bytearray()
    offsets = []
    for _ in range(frames):
        offsets.append(base + len(body))
        body += build_frame(width, height, frame_budget, rng)
    offsets.append(base + len(body))
    header = struct.pack("<hhhhhbbHh", width, height, 0, frames, min(loop, frames - 1), 0, 0, 0, palette_offset)
    header += bytes((1 if store else 0, 0))                                 # frame type, remap flag (no remap)
    header += b"".join(struct.pack("<I", o) for o in offsets)
    data = header + palette + bytes(body)
    if len(data) > length:
        raise ValueError("%dx%d x%d frames does not fit in %d bytes (needs %d)" % (width, height, frames, length, len(data)))
    return data + bytes(length - len(data))


def build_voc(rng, samples=2000):
    voc = b"Creative Voice File\x1a" + struct.pack("<HHH", 0x1A, 0x010A, 0x1129)
    block = bytes((256 - 1000000 // 11025, 0)) + bytes(rng.randrange(96, 160) for _ in range(samples))
    voc += bytes((1,)) + struct.pack("<I", len(block))[:3] + block + b"\x00"
    return voc


def build_xmidi():
    """Smallest XMIDI the converter accepts: one sequence whose only event is end-of-track."""
    def chunk(tag, payload):
        pad = b"\x00" if len(payload) % 2 else b""
        return tag + struct.pack(">I", len(payload)) + payload + pad
    evnt = chunk(b"EVNT", b"\xff\x2f\x00")
    timb = chunk(b"TIMB", struct.pack("<H", 0))
    form_xmid = chunk(b"FORM", b"XMID" + timb + evnt)
    form_xdir = chunk(b"FORM", b"XDIR" + chunk(b"INFO", struct.pack("<H", 1)))
    return form_xdir + chunk(b"CAT ", b"XMID" + form_xmid)


def build_sound(sound_type, length, rng):
    """A 0xDEAF sound entry of exactly `length` bytes: noise VOC sized to fill it, or a silent XMIDI padded to it."""
    room = length - SOUND_HDR_SIZE
    if sound_type == 1:
        payload = build_xmidi()
    else:
        payload = build_voc(rng, samples=max(1, room - 26 - 4 - 2 - 1))
    payload = payload[:room] + bytes(max(0, room - len(payload)))
    return struct.pack("<HH", SOUND_MAGIC, sound_type) + bytes(SOUND_HDR_SIZE - 4) + payload


def build_entry(row, rng):
    lbx, entry, length = row["lbx"], int(row["entry"]), int(row["length"])
    if length == 0:
        return b""
    table = table_for(lbx, entry)
    if table is not None:
        return build_data_table(table, length, "%s entry %d" % (lbx, entry), rng)
    if lbx in SOUND_ARCHIVES:
        return build_sound(SOUND_ARCHIVES[lbx], length, rng)
    if row["kind"] == "sprite":
        return build_flic(int(row["width"]), int(row["height"]), int(row["frames"]), int(row["loop"]),
                          row["has_palette"] == "1", row["store"] == "1", length, rng)
    if lbx == "FONTS.LBX" and entry >= 2:
        # Palette entries: 256 6-bit RGB colours, then the rest of the original block as random bytes.
        return bytes(rng.randrange(64) for _ in range(768)) + bytes(rng.randrange(256) for _ in range(length - 768))
    return bytes(rng.randrange(256) for _ in range(length))


def terrain_tile_offset(page, slot):
    """Where Draw_Map_Terrain() (MainScr_Maps.c) finds a tile in TERRAIN.LBX entry 0, from its entry-1 (page, slot) pair."""
    if page == 0:
        return 0xC0 + (slot - 2) * 384
    return page * 16384 + slot * 384 - 0xC0 - 384


def build_terrain(rows, rng):
    """TERRAIN.LBX: entry 0 is a sheet of 384-byte tiles (16-byte picture header + 20x18 pixels), entry 1 maps each
    terrain type to a (page, slot) byte pair pointing into it, entry 2 is the minimap colour per terrain type."""
    lengths = [int(r["length"]) for r in rows]
    sheet = bytearray(rng.randrange(1, 224) for _ in range(lengths[0]))
    index = bytearray()
    for _ in range(lengths[1] // 2):
        while True:
            page = rng.randrange(0, 41)
            slot = rng.randrange(2, 42) if page == 0 else rng.randrange(0, 42)
            offset = terrain_tile_offset(page, slot)
            if 0 <= offset and offset + 384 <= len(sheet):
                break
        index += bytes((page, slot))                                       # bit 0x80 of page (animated) left clear
        sheet[offset:offset + 16] = struct.pack("<hhhhhbbHh", 20, 18, 0, 1, 0, 0, 0, 0, 0)  # width, height; loop 0 = no remap
    colors = bytes(rng.randrange(1, 224) for _ in range(lengths[2]))
    return [bytes(sheet), bytes(index), colors]


def build_lbx(rows, rng):
    names = [row["name"] for row in rows]
    if rows[0]["lbx"] == "TERRAIN.LBX":
        entries = build_terrain(rows, rng)
    else:
        entries = [build_entry(row, rng) for row in rows]
    count = len(entries)
    data_start = LBX_HDR_SIZE + LBX_NAME_RECORD * count
    offsets = [data_start]
    for data in entries:
        offsets.append(offsets[-1] + len(data))
    header = struct.pack("<HHHH", count, LBX_MAGIC, 0, 0) + b"".join(struct.pack("<I", o) for o in offsets)
    if len(header) > LBX_HDR_SIZE:
        raise ValueError("too many entries for one LBX header: %d" % count)
    header += bytes(LBX_HDR_SIZE - len(header))
    name_table = bytearray()
    for name in names:
        record = bytearray(LBX_NAME_RECORD)
        put_text(record, 0, 9, name)
        put_text(record, 9, 23, "dummy")
        name_table += record
    return bytes(header) + bytes(name_table) + b"".join(entries)


def load_catalog(path):
    rows = fwv.read(path)
    rows = rows[1] if isinstance(rows, tuple) else rows
    archives = {}
    for row in rows:
        archives.setdefault(row["lbx"], []).append(row)
    for lbx in archives:
        archives[lbx].sort(key=lambda r: int(r["entry"]))
    return archives


def main(argv=None):
    parser = argparse.ArgumentParser(description="Generate stand-in LBX files (noise pictures and sounds, placeholder data).")
    parser.add_argument("out_dir", help="folder to write the .LBX files into (created if missing), or to check with --check")
    parser.add_argument("--check", action="store_true", help="validate the .LBX files in out_dir instead of writing them")
    parser.add_argument("--seed", type=int, default=12345, help="random seed; the same seed gives byte-identical files")
    parser.add_argument("--catalog", default=CATALOG, help="asset catalog (default: tools/fields/assets.fwv)")
    args = parser.parse_args(argv)

    archives = load_catalog(args.catalog)
    if args.check:
        problems, counts = check_folder(args.out_dir, archives)
        for problem in problems:
            print(problem)
        print("checked %(archives)d archives (%(missing)d not present): %(tables)d data tables, %(records)d records, "
              "%(pictures)d pictures" % counts)
        print("%d problem(s)" % len(problems))
        return 1 if problems else 0
    os.makedirs(args.out_dir, exist_ok=True)
    for lbx in sorted(archives):
        rng = random.Random("%d:%s" % (args.seed, lbx))
        data = build_lbx(archives[lbx], rng)
        with open(os.path.join(args.out_dir, lbx), "wb") as fp:
            fp.write(data)
    print("wrote %d archives to %s" % (len(archives), args.out_dir))
    return 0


if __name__ == "__main__":
    sys.exit(main())
