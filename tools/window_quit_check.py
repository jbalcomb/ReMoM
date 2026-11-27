#!/usr/bin/env python3
"""Check how a Windows ReMoM build quits, with real OS input (not the Automation Driver, which injects below the window).

Launches ReMoM.exe, waits, then sends one of:
  close   WM_CLOSE posted to the game window -- what the window's X and Alt+F4 send. Must exit at once.
  hotkey  Ctrl+Alt+Shift+Q through SendInput (the system input queue, so GetKeyState / SDL see the modifiers). Must exit at once.
  onlyq   Alt+Q alone. Must NOT quit (the quit hotkey needs all three modifiers).
  all     the three above, one launch each.
For each it reports how long the exit took and whether a captured cursor's clip (ClipCursor, system-wide) was released afterwards.
Exit status 0 when every check passed, 1 otherwise.

Windows only. ReMoM opens (fullscreen by default) for a few seconds per check: don't type while it runs; the hotkey checks need the
game to be the foreground window.

    python3 tools/window_quit_check.py out/build/MSVC-win32-debug/bin/Debug/ReMoM.exe all
    python3 tools/window_quit_check.py out/build/MSVC-debug/bin/Debug/ReMoM.exe close --wait 6
"""
import argparse
import ctypes
import os
import subprocess
import sys
import time

if sys.platform != "win32":
    sys.exit("window_quit_check: Windows only (it uses user32 SendInput / PostMessage)")

import ctypes.wintypes as wt

user32 = ctypes.WinDLL("user32", use_last_error=True)
user32.SetProcessDpiAwarenessContext(ctypes.c_void_p(-4))  # per-monitor v2: real pixels, as the game uses -- else clip rects are scaled

WM_CLOSE = 0x0010
KEYEVENTF_KEYUP = 0x0002
INPUT_KEYBOARD = 1
VK_SHIFT, VK_CONTROL, VK_MENU, VK_Q = 0x10, 0x11, 0x12, 0x51
EXIT_TIMEOUT_S = 3.0


class KEYBDINPUT(ctypes.Structure):
    _fields_ = [("wVk", wt.WORD), ("wScan", wt.WORD), ("dwFlags", wt.DWORD), ("time", wt.DWORD), ("dwExtraInfo", ctypes.c_size_t)]


class _INPUT_UNION(ctypes.Union):
    _fields_ = [("ki", KEYBDINPUT), ("pad", ctypes.c_byte * 32)]


class INPUT(ctypes.Structure):
    _fields_ = [("type", wt.DWORD), ("u", _INPUT_UNION)]


MAPVK_VK_TO_VSC = 0


def key_event(vk, up):
    # The hardware scan code goes with the virtual-key code, as a real keyboard sends. SDL2 identifies keys by scan code: with wScan = 0
    # it dropped every key but Ctrl (which it reads by virtual-key code), so Ctrl+Alt+Shift+Q never reached it. The Win32 backend reads
    # the virtual-key code and worked either way.
    event = INPUT()
    event.type = INPUT_KEYBOARD
    event.u.ki = KEYBDINPUT(vk, user32.MapVirtualKeyW(vk, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP if up else 0, 0, 0)
    return event


def find_window(pid):
    found = []

    @ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)
    def callback(hwnd, _):
        owner = wt.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(owner))
        if owner.value == pid and user32.IsWindowVisible(hwnd):
            found.append(hwnd)
        return True

    user32.EnumWindows(callback, 0)
    return found[0] if found else None


def clip_rect():
    rect = wt.RECT()
    user32.GetClipCursor(ctypes.byref(rect))
    return (rect.left, rect.top, rect.right, rect.bottom)


def send_keys(hwnd, modifiers):
    user32.SetForegroundWindow(hwnd)
    time.sleep(0.3)
    if user32.GetForegroundWindow() != hwnd:
        print("  FAIL: could not make the game the foreground window -- SendInput would go elsewhere")
        return False
    sequence = [key_event(vk, False) for vk in modifiers] + [key_event(VK_Q, False), key_event(VK_Q, True)] + [key_event(vk, True) for vk in reversed(modifiers)]
    # One key per SendInput, 50 ms apart, as a person presses them.
    for event in sequence:
        events = (INPUT * 1)(event)
        if user32.SendInput(1, events, ctypes.sizeof(INPUT)) != 1:
            print("  FAIL: SendInput failed (error %d)" % ctypes.get_last_error())
            return False
        time.sleep(0.05)
    return True


def run_check(exe, mode, wait_s):
    print("== %s" % mode)
    baseline = clip_rect()
    exe = os.path.abspath(exe)  # a relative path would be resolved against cwd= below, not ours
    proc = subprocess.Popen([exe], cwd=os.path.dirname(exe))
    try:
        time.sleep(wait_s)
        if proc.poll() is not None:
            print("  FAIL: ReMoM exited on its own after %.1f s (code %d) -- nothing to test" % (wait_s, proc.returncode))
            return False
        hwnd = find_window(proc.pid)
        if hwnd is None:
            print("  FAIL: no visible window for pid %d after %.1f s" % (proc.pid, wait_s))
            return False
        print("  cursor clip: before launch %s, while running %s" % (baseline, clip_rect()))
        t0 = time.time()
        if mode == "close":
            if not user32.PostMessageW(hwnd, WM_CLOSE, 0, 0):
                print("  FAIL: PostMessage(WM_CLOSE) failed (error %d)" % ctypes.get_last_error())
                return False
        elif not send_keys(hwnd, [VK_CONTROL, VK_MENU, VK_SHIFT] if mode == "hotkey" else [VK_MENU]):
            return False
        try:
            proc.wait(timeout=EXIT_TIMEOUT_S)
            exited = True
            print("  exited after %.2f s, code %d" % (time.time() - t0, proc.returncode))
        except subprocess.TimeoutExpired:
            exited = False
            print("  still running %.0f s later" % EXIT_TIMEOUT_S)
    finally:
        if proc.poll() is None:
            # Close it the normal way first, so the game flushes its log (a killed process loses the buffered tail); kill only if that fails.
            window = find_window(proc.pid)
            if window is not None:
                user32.PostMessageW(window, WM_CLOSE, 0, 0)
            try:
                proc.wait(timeout=EXIT_TIMEOUT_S)
            except subprocess.TimeoutExpired:
                proc.kill()
                proc.wait()
    time.sleep(0.3)
    released = (clip_rect() == baseline)
    print("  cursor clip after exit %s -- %s" % (clip_rect(), "released" if released else "NOT released (still trapped?)"))
    if mode == "onlyq":
        passed = (exited is False) and released
    else:
        passed = exited and released
    print("  %s" % ("PASS" if passed else "FAIL"))
    return passed


def main():
    parser = argparse.ArgumentParser(description="Check how a Windows ReMoM build quits, with real OS input.")
    parser.add_argument("exe", help="path to ReMoM.exe")
    parser.add_argument("mode", choices=["close", "hotkey", "onlyq", "all"])
    parser.add_argument("--wait", type=float, default=4.0, help="seconds to let the game start before sending (default 4)")
    args = parser.parse_args()
    if not os.path.isfile(args.exe):
        sys.exit("window_quit_check: no such file: %s" % args.exe)
    modes = ["close", "hotkey", "onlyq"] if args.mode == "all" else [args.mode]
    results = [run_check(args.exe, mode, args.wait) for mode in modes]
    print("%d of %d passed" % (sum(results), len(results)))
    return 0 if all(results) else 1


if __name__ == "__main__":
    sys.exit(main())
