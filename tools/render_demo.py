#!/usr/bin/env python
"""
render_demo.py - render one scripted demo scene to an mp4, end to end.

Drives ReMoMber through a .hms scenario (or a .RMR replay) with in-engine A/V capture enabled, then
encodes the captured raw stream to H.264 with ffmpeg, muxing the captured game audio if there is
any.  See doc/#AI_Plans/PRD-Scripted-Demo-Capture.md.

  python tools/render_demo.py --scene overland --scenario showcase/03_overland.hms
  python tools/render_demo.py --scene intro --replay assets/DEMO_QUIT_KD.RMR --max-seconds 30
  python tools/render_demo.py --concat out/demo/*.mp4 --output out/demo/reel.mp4

Notes:
  - Scenes SHOULD end by quitting the game, so capture finalizes cleanly.  Until they do, use
    --max-seconds; the process is killed and the capture is still usable (capture_info.txt is
    written up front and the frame count is recoverable from the raw file size).
  - The raw intermediate is large (about 5.8 MB/s at 30 fps) and is deleted after a successful
    encode unless --keep-raw is given.
"""

from __future__ import print_function

import argparse
import glob
import os
import shutil
import subprocess
import sys


REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_GAME = os.path.join(REPO_ROOT, "out", "build", "MSVC-debug", "bin", "Debug", "ReMoMber.exe")
DEFAULT_OUT = os.path.join(REPO_ROOT, "out", "demo")

# 320x200 was displayed at 4:3 on VGA, so the pixels are not square.  Scaling to 1440x1080 restores
# the intended aspect and pillarboxes into 1920x1080.  flags=neighbor keeps the pixels crisp --
# smoothing them would defeat the point of the exercise.  For a square-pixel (16:10) look instead,
# use --square-pixels, which integer-scales 4x to 1280x800 and pillarboxes that.
VF_ASPECT_43 = "scale=1440:1080:flags=neighbor,pad=1920:1080:240:0:black"
VF_SQUARE = "scale=1280:800:flags=neighbor,pad=1920:1080:320:140:black"


def run(cmd, **kwargs):
    print("+ " + " ".join('"%s"' % c if " " in c else c for c in cmd))
    return subprocess.call(cmd, **kwargs)


def read_info(capture_dir):
    info = {}
    path = os.path.join(capture_dir, "capture_info.txt")
    if not os.path.exists(path):
        return info
    handle = open(path, "r")
    try:
        for line in handle:
            if "=" in line:
                key, _, value = line.partition("=")
                info[key.strip()] = value.strip()
    finally:
        handle.close()
    return info


def capture_scene(args, capture_dir):
    # Must be absolute: the game is launched with cwd set to its own directory (so it finds the
    # staged assets), which would otherwise break a relative --game path.
    args.game = os.path.abspath(args.game)
    if not os.path.exists(args.game):
        print("error: game executable not found: %s" % args.game, file=sys.stderr)
        print("       build it first: cmake --build --preset MSVC-debug", file=sys.stderr)
        return 1

    cmd = [args.game, "--capture", capture_dir, "--capture-fps", str(args.fps)]
    # --load must come before --scenario: a scene whose .hms starts by acting on a loaded game (e.g.
    # pressing Next Turn) needs the save loaded and the scr_Continue path taken FIRST; without it the
    # game sits on the Main Menu and the scenario's keys hit the wrong screen (e.g. 'N' = New Game).
    if args.load:
        cmd += ["--load", args.load]
    if args.patch:
        cmd += ["--patch", args.patch]
    if args.scenario:
        cmd += ["--scenario", os.path.abspath(args.scenario)]
    if args.replay:
        cmd += ["--replay", os.path.abspath(args.replay)]
    if args.seed is not None:
        cmd += ["--seed1", str(args.seed)]
    if args.game_args:
        cmd += args.game_args

    # The game must run with its staged assets as the working directory.
    workdir = os.path.dirname(args.game)

    print("")
    print("--- capturing (max %ds) ---" % args.max_seconds)
    process = subprocess.Popen(cmd, cwd=workdir)

    try:
        # subprocess.wait(timeout=) is Python 3 only; poll so this works on the repo's 2.7 too.
        import time
        deadline = time.time() + args.max_seconds
        while process.poll() is None and time.time() < deadline:
            time.sleep(0.25)
        if process.poll() is None:
            print("(max-seconds reached; stopping the game -- capture is still usable)")
            process.terminate()
            time.sleep(1.0)
            if process.poll() is None:
                process.kill()
    except KeyboardInterrupt:
        process.kill()
        return 1

    return 0


def repair_wav_header(wav_path):
    """
    Patch the RIFF and data sizes from the real file length.

    The capture writes a placeholder header and patches it in Platform_Capture_Stop().  When a scene
    is ended by --max-seconds the process is killed, Stop() never runs, and both size fields stay
    zero -- which ffmpeg only partially copes with.  A scene that ends by quitting does not need this.
    """
    import struct

    size = os.path.getsize(wav_path)
    if size < 44:
        return

    handle = open(wav_path, "r+b")
    try:
        handle.seek(4)
        riff_size = struct.unpack("<I", handle.read(4))[0]
        handle.seek(40)
        data_size = struct.unpack("<I", handle.read(4))[0]

        expected_data = size - 44
        if riff_size != 0 and data_size != 0:
            return   # already finalized by a clean exit

        print("(unfinalized WAV header -- patching sizes to %d data bytes)" % expected_data)
        handle.seek(4)
        handle.write(struct.pack("<I", 36 + expected_data))
        handle.seek(40)
        handle.write(struct.pack("<I", expected_data))
    finally:
        handle.close()


def encode_scene(args, capture_dir, output_path):
    info = read_info(capture_dir)
    raw = os.path.join(capture_dir, info.get("video_file", "video_rgb24.raw"))
    if not os.path.exists(raw) or os.path.getsize(raw) == 0:
        print("error: no captured video at %s" % raw, file=sys.stderr)
        return 1

    width = info.get("width", "320")
    height = info.get("height", "200")
    fps = info.get("fps", str(args.fps))

    frame_bytes = int(width) * int(height) * 3
    raw_bytes = os.path.getsize(raw)
    frames = raw_bytes // frame_bytes

    # Killing the game (--max-seconds) can interrupt a frame mid-write, leaving a partial frame that
    # makes ffmpeg log "Invalid buffer size, packet size N < expected frame_size M".  Truncate to a
    # whole number of frames so the encode is clean.  A scene that ends by quitting never hits this.
    if raw_bytes % frame_bytes:
        print("(trailing partial frame of %d bytes -- truncating to %d whole frames)"
              % (raw_bytes % frame_bytes, frames))
        handle = open(raw, "r+b")
        try:
            handle.truncate(frames * frame_bytes)
        finally:
            handle.close()
    print("")
    print("--- encoding: %s frames at %s fps (%.1fs), %sx%s ---"
          % (frames, fps, float(frames) / float(fps), width, height))

    wav = os.path.join(capture_dir, "audio.wav")
    have_audio = os.path.exists(wav) and os.path.getsize(wav) > 44
    if have_audio:
        repair_wav_header(wav)

    cmd = ["ffmpeg", "-y", "-hide_banner", "-loglevel", "error",
           "-f", "rawvideo", "-pix_fmt", "rgb24", "-s", "%sx%s" % (width, height),
           "-r", fps, "-i", raw]
    if have_audio:
        cmd += ["-i", wav]
    cmd += ["-vf", VF_SQUARE if args.square_pixels else VF_ASPECT_43,
            "-c:v", "libx264", "-crf", str(args.crf), "-preset", args.preset,
            "-pix_fmt", "yuv420p"]
    if have_audio:
        cmd += ["-c:a", "aac", "-b:a", "192k", "-shortest"]
    else:
        print("(no audio captured -- the win32 backend is silent; see the PRD)")
    cmd += [output_path]

    if run(cmd) != 0:
        return 1

    print("wrote %s (%.1f MB)" % (output_path, os.path.getsize(output_path) / 1048576.0))
    return 0


def concat(inputs, output_path):
    """Stitch rendered scenes into one reel. All inputs must share codec/resolution/rate."""
    files = []
    for pattern in inputs:
        files.extend(sorted(glob.glob(pattern)))
    if not files:
        print("error: no input files matched", file=sys.stderr)
        return 1

    list_path = output_path + ".concat.txt"
    handle = open(list_path, "w")
    try:
        for path in files:
            handle.write("file '%s'\n" % os.path.abspath(path).replace("\\", "/"))
    finally:
        handle.close()

    print("--- concatenating %d scene(s) ---" % len(files))
    for path in files:
        print("    %s" % path)

    result = run(["ffmpeg", "-y", "-hide_banner", "-loglevel", "error",
                  "-f", "concat", "-safe", "0", "-i", list_path, "-c", "copy", output_path])
    os.remove(list_path)
    if result == 0:
        print("wrote %s (%.1f MB)" % (output_path, os.path.getsize(output_path) / 1048576.0))
    return result


def main():
    parser = argparse.ArgumentParser(description="Render a scripted demo scene to mp4.")
    parser.add_argument("--scene", help="Scene name; names the output mp4 and the capture dir")
    parser.add_argument("--scenario", help="Path to a .hms scenario script")
    parser.add_argument("--replay", help="Path to a .RMR replay (alternative to --scenario)")
    parser.add_argument("--load", help="Save to load before the scenario runs (e.g. SAVETEST.GAM); passed to ReMoMber --load")
    parser.add_argument("--patch", help="Scenario patch name for TST_Patch_Game_Data (e.g. som_casting); passed to ReMoMber --patch")
    parser.add_argument("--game", default=DEFAULT_GAME, help="ReMoMber.exe to drive")
    parser.add_argument("--out-dir", default=DEFAULT_OUT, help="Where mp4s and captures go")
    parser.add_argument("--fps", type=int, default=30, help="Capture/output frame rate")
    parser.add_argument("--crf", type=int, default=18, help="x264 quality (lower = better)")
    parser.add_argument("--preset", default="medium", help="x264 preset")
    parser.add_argument("--max-seconds", type=int, default=300, help="Kill the game after this long")
    parser.add_argument("--seed", type=int, help="Pin the RNG seed (--seed1) for determinism")
    parser.add_argument("--square-pixels", action="store_true", help="16:10 square-pixel look instead of 4:3")
    parser.add_argument("--keep-raw", action="store_true", help="Do not delete the raw capture")
    parser.add_argument("--encode-only", action="store_true", help="Skip capture; encode an existing capture dir")
    parser.add_argument("--concat", nargs="+", metavar="GLOB", help="Stitch rendered mp4s into one reel")
    parser.add_argument("--output", help="Output path (with --concat)")
    parser.add_argument("game_args", nargs="*", help="Extra args passed through to the game")
    args = parser.parse_args()

    if args.concat:
        if not args.output:
            print("error: --concat requires --output", file=sys.stderr)
            return 1
        return concat(args.concat, args.output)

    if not args.scene:
        print("error: --scene is required", file=sys.stderr)
        return 1
    if not args.scenario and not args.replay and not args.encode_only:
        print("error: one of --scenario / --replay is required", file=sys.stderr)
        return 1

    capture_dir = os.path.join(args.out_dir, "capture", args.scene)
    output_path = os.path.join(args.out_dir, args.scene + ".mp4")
    if not os.path.isdir(os.path.dirname(output_path)):
        os.makedirs(os.path.dirname(output_path))

    if not args.encode_only:
        if os.path.isdir(capture_dir):
            shutil.rmtree(capture_dir)
        if capture_scene(args, capture_dir) != 0:
            return 1

    if encode_scene(args, capture_dir, output_path) != 0:
        return 1

    if not args.keep_raw:
        raw = os.path.join(capture_dir, "video_rgb24.raw")
        if os.path.exists(raw):
            os.remove(raw)
            print("(removed the raw intermediate; --keep-raw to retain it)")

    return 0


if __name__ == "__main__":
    sys.exit(main())
