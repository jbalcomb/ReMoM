#!/usr/bin/env python3
"""check_combat_legality.py -- flag illegal combat moves/attacks and streaky draws.

Consumes the test-only LOG_CAT_COMBAT_TEST lines (enable with `COMBAT_TEST = true`
in ReMoM.ini) emitted by Auto_Move_Unit, Battle_Unit_Attack, and Combat_Grid_Entities:

    MOVE turn=T unit=N target=K dst=(dx,dy) final=(fx,fy) mp=M pathlen=L
    MOVE_STEP unit=N seq=i cell=(x,y)
    ATTACK turn=T attacker=N defender=K attacker_cell=(ax,ay) defender_cell=(dx,dy) range=R mode=melee|ranged
    DRAW turn=T unit=N src=(sx,sy) dst=(tx,ty) ctr=C cnt=Q mid=1

Three invariants:

  * Move legality  -- every consecutive pair of path cells must be grid-adjacent
                      (Chebyshev distance <= 1).  A non-adjacent step is the
                      "streak across the map" in the MOVE LOGIC.
  * Attack legality -- a melee attack (mode=melee) requires the attacker to be
                      adjacent to the defender (range <= 1).  A melee attack at
                      range > 1 is a unit striking a target too far away.
  * Draw smoothness -- the DRAW lines record where the moving figure is actually
                      blitted each frame: it is interpolated from cell src toward
                      cell dst by ctr/cnt.  Two things can go wrong in the DRAW
                      (independently of the logic, which the MOVE check covers):
                        - geometry: src and dst not grid-adjacent AND ctr>0 -> the
                          sprite slides across >1 cell in a single interpolation.
                          (At ctr==0 the draw is exactly src, so a non-adjacent
                          target is harmless and expected: a ranged attacker's
                          target_cgx is the far defender cell, drawn parked while
                          the missile flies, and an idle figure's target encodes
                          facing.  Only ctr>0 non-adjacent is a real streak.)
                        - pacing: too few frames drawn per cell, so the figure
                          leaps a large fraction of a cell between frames.  That
                          is the visible "streak" when the MOVE logic is clean.
                      Headless (--combat-tactical / animations off) draws each
                      step exactly once at ctr=cnt-1; that single-frame signature
                      is reported as "animations off -- pacing not assessed", not
                      flagged, because no animation actually ran.

Usage:
    python3 check_combat_legality.py <logfile> [--max-frac F]

    --max-frac F   Flag a draw episode when the figure jumps more than F of a
                   cell between consecutive frames (default 0.5).  A smooth
                   MOVE_ANIM_CNT-frame walk advances ~1/cnt per frame.

Exit codes: 0 = clean, 1 = violations found, 2 = usage/IO error.
"""

import re
import sys


MOVE_RE = re.compile(r'MOVE turn=(-?\d+) unit=(-?\d+) target=(-?\d+) '
                     r'dst=\((-?\d+),(-?\d+)\) final=\((-?\d+),(-?\d+)\) '
                     r'mp=(-?\d+) pathlen=(-?\d+)')
STEP_RE = re.compile(r'MOVE_STEP unit=(-?\d+) seq=(-?\d+) cell=\((-?\d+),(-?\d+)\)')
ATTACK_RE = re.compile(r'ATTACK turn=(-?\d+) attacker=(-?\d+) defender=(-?\d+) '
                       r'attacker_cell=\((-?\d+),(-?\d+)\) '
                       r'defender_cell=\((-?\d+),(-?\d+)\) '
                       r'range=(-?\d+) has_ranged=(-?\d+) action=(-?\d+) mode=(melee|ranged)')
DRAW_RE = re.compile(r'DRAW turn=(-?\d+) unit=(-?\d+) '
                     r'src=\((-?\d+),(-?\d+)\) dst=\((-?\d+),(-?\d+)\) '
                     r'ctr=(-?\d+) cnt=(-?\d+) mid=(-?\d+)')


def cheb(ax, ay, bx, by):
    return max(abs(ax - bx), abs(ay - by))


def frac_pos(f):
    # interpolated cell position of the figure this frame: src + (dst-src)*ctr/cnt
    sx, sy, tx, ty, ctr, cnt = f['sx'], f['sy'], f['tx'], f['ty'], f['ctr'], f['cnt']
    if cnt == 0:
        return (float(sx), float(sy))
    return (sx + (tx - sx) * ctr / float(cnt),
            sy + (ty - sy) * ctr / float(cnt))


def analyze_episode(ep, max_frac):
    # ep: list of DRAW frames (one moving unit, contiguous).  Returns (violations, note).
    violations = []
    turn = ep[0]['turn']
    unit = ep[0]['unit']
    cnt = ep[0]['cnt']

    # geometry: a figure only slides across a gap when it is actually interpolated,
    # i.e. ctr>0.  At ctr==0 the draw position is exactly src (parked), so a
    # non-adjacent target is harmless and expected -- a ranged attacker's target_cgx
    # is the defender's cell (drawn parked while the missile flies), and an idle
    # unit's target_cgx encodes facing.  Only ctr>0 with a non-adjacent target is a
    # real streak.  Collapse per-frame repeats to one report per (src,dst), keeping
    # the max ctr the slide reached.
    bad = {}
    for f in ep:
        if f['ctr'] <= 0:
            continue
        d = cheb(f['sx'], f['sy'], f['tx'], f['ty'])
        if d > 1:
            k = (f['sx'], f['sy'], f['tx'], f['ty'])
            rec = bad.get(k)
            if rec is None or f['ctr'] > rec[1]:
                bad[k] = (d, f['ctr'])
    for (sx, sy, tx, ty), (d, maxctr) in bad.items():
        violations.append(
            "STREAK (cross-cell slide): turn=%s unit=%s src=(%d,%d) dst=(%d,%d) are %d cells apart, drawn at ctr=%d -- moving figure interpolated across a non-adjacent gap"
            % (turn, unit, sx, sy, tx, ty, d, maxctr))

    # distinct steps = distinct (src,dst) segments, in order
    steps = []
    for f in ep:
        key = (f['sx'], f['sy'], f['tx'], f['ty'])
        if not steps or steps[-1] != key:
            steps.append(key)
    n_steps = len(steps)
    n_frames = len(ep)

    # headless / animations-off signature: every frame at ctr == cnt-1 (the
    # MOVE_ANIM_MAX single-frame draw).  No animation ran -> pacing is N/A.
    animated = any(f['ctr'] != (f['cnt'] - 1) for f in ep)
    if not animated:
        note = ("turn=%s unit=%s: %d cell(s), %d frame(s) all at ctr=cnt-1 -- animations off (single-frame draw), pacing not assessed"
                % (turn, unit, n_steps, n_frames))
        return violations, note

    # pacing: largest per-frame jump of the interpolated figure position
    max_jump = 0.0
    prev = None
    for f in ep:
        p = frac_pos(f)
        if prev is not None:
            dx = p[0] - prev[0]
            dy = p[1] - prev[1]
            jump = (dx * dx + dy * dy) ** 0.5
            if jump > max_jump:
                max_jump = jump
        prev = p
    fpc = n_frames / float(n_steps) if n_steps else 0.0

    if max_jump > max_frac:
        violations.append(
            "STREAK (choppy draw): turn=%s unit=%s -- figure jumps %.2f cell/frame (max), %.1f frame(s)/cell over %d cell(s); a smooth walk draws ~%d frames/cell (<=%.2f/frame)"
            % (turn, unit, max_jump, fpc, n_steps, cnt, max_frac))
        note = None
    else:
        note = ("turn=%s unit=%s: %d cell(s), %.1f frame(s)/cell, max jump %.2f cell/frame -- smooth"
                % (turn, unit, n_steps, fpc, max_jump))
    return violations, note


def check(path, max_frac):
    violations = []
    notes = []
    moves = 0
    attacks = 0
    draws = 0
    cur = None       # current MOVE being accumulated: {'turn','unit','steps':[...]}
    ep = None        # current DRAW episode: list of frames for one contiguous unit

    def close_move(m):
        s = m['steps']
        for i in range(1, len(s)):
            d = cheb(s[i - 1][0], s[i - 1][1], s[i][0], s[i][1])
            if d > 1:
                violations.append(
                    "ILLEGAL MOVE (jump): turn=%s unit=%s step %d %s -> %s is %d cells apart (not adjacent); full path: %s"
                    % (m['turn'], m['unit'], i, s[i - 1], s[i], d,
                       ";".join("(%d,%d)" % c for c in s)))

    def close_ep(e):
        if e:
            vs, note = analyze_episode(e, max_frac)
            violations.extend(vs)
            if note:
                notes.append(note)

    with open(path, 'r', errors='replace') as fh:
        for line in fh:
            d = DRAW_RE.search(line)
            if d:
                draws += 1
                f = {'turn': d.group(1), 'unit': d.group(2),
                     'sx': int(d.group(3)), 'sy': int(d.group(4)),
                     'tx': int(d.group(5)), 'ty': int(d.group(6)),
                     'ctr': int(d.group(7)), 'cnt': int(d.group(8))}
                if ep is not None and ep[0]['unit'] == f['unit']:
                    ep.append(f)
                else:
                    close_ep(ep)
                    ep = [f]
                continue
            # any non-DRAW event breaks a contiguous draw burst
            if ep is not None:
                close_ep(ep)
                ep = None

            m = MOVE_RE.search(line)
            if m:
                if cur is not None:
                    close_move(cur)
                cur = {'turn': m.group(1), 'unit': m.group(2), 'steps': []}
                moves += 1
                continue
            s = STEP_RE.search(line)
            if s:
                if cur is not None and s.group(1) == cur['unit']:
                    cur['steps'].append((int(s.group(3)), int(s.group(4))))
                continue
            a = ATTACK_RE.search(line)
            if a:
                if cur is not None:
                    close_move(cur)
                    cur = None
                attacks += 1
                turn, atk, dfn = a.group(1), a.group(2), a.group(3)
                ax, ay = int(a.group(4)), int(a.group(5))
                dx, dy = int(a.group(6)), int(a.group(7))
                rng = int(a.group(8))
                has_ranged = int(a.group(9))
                mode = a.group(11)
                dist = cheb(ax, ay, dx, dy)
                # An attack at range>1 is legal ONLY for a unit that actually has a
                # ranged attack.  A unit with has_ranged=0 striking a non-adjacent
                # target is the "streak across the map to attack" -- the game tagged
                # it mode=ranged (ranged_attack_flag) purely because range>1, without
                # ever checking capability, so the mode field cannot be trusted here.
                if dist > 1 and not has_ranged:
                    violations.append(
                        "ILLEGAL ATTACK (non-ranged unit striking at range): turn=%s attacker=%s @(%d,%d) defender=%s @(%d,%d) -- %d cells away (game range=%d, tagged mode=%s) but the unit has NO ranged attack (has_ranged=0)"
                        % (turn, atk, ax, ay, dfn, dx, dy, dist, rng, mode))
                elif mode == 'melee' and dist > 1:
                    violations.append(
                        "ILLEGAL ATTACK (melee at range): turn=%s attacker=%s @(%d,%d) defender=%s @(%d,%d) -- melee against a target %d cells away (reported range=%d)"
                        % (turn, atk, ax, ay, dfn, dx, dy, dist, rng))
        if cur is not None:
            close_move(cur)
        close_ep(ep)

    return violations, notes, moves, attacks, draws


def main(argv):
    args = [a for a in argv[1:] if not a.startswith('--')]
    max_frac = 0.5
    for i, a in enumerate(argv):
        if a == '--max-frac' and i + 1 < len(argv):
            try:
                max_frac = float(argv[i + 1])
            except ValueError:
                sys.stderr.write("check_combat_legality: bad --max-frac value\n")
                return 2
    if len(args) != 1:
        sys.stderr.write("Usage: %s <logfile> [--max-frac F]\n" % argv[0])
        return 2
    try:
        violations, notes, moves, attacks, draws = check(args[0], max_frac)
    except IOError as e:
        sys.stderr.write("check_combat_legality: %s\n" % e)
        return 2

    print("scanned %d moves, %d attacks, %d draw frames" % (moves, attacks, draws))
    for n in notes:
        print("  note: " + n)
    if moves == 0 and attacks == 0 and draws == 0:
        sys.stderr.write("check_combat_legality: no COMBAT_TEST events found -- was the category enabled (COMBAT_TEST = true) and did combat run?\n")
        return 2
    if violations:
        for v in violations:
            print(v)
        print("FAIL: %d combat legality violation(s)" % len(violations))
        return 1
    print("PASS: no illegal moves, attacks, or streaky draws")
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
