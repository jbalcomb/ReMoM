
Borland Overlay Manager
VROOMM (Virtual Run-time Object-Oriented Memory Manager)(as of v2.0)

IVT
Interrupt 3F

start()
    |-> Initialize()
        InitStart: _setupio(); _c0crtinit(); __setenvp(); __OvrPrepare();
        ExitStart: __OVREXIT()
__OvrPrepare()
    __InitModules()
    _farmalloc() ...__ovrbuffer, __OvrMinHeapSize
    __OVRINIT()
__EXENAME__ ... "wizards.exe"
"FBOV" signature in EXE file





DOSBox

find obrland DSEG sig
find "FBOV"



Where in the dosbox-0.74-3 code could we see that the borland startup code has installed the interrupt hook?

mem.h
RealSetVec()

Right at src/dos/dos.cpp:357-359:


case 0x25:    /* Set Interrupt Vector */
    RealSetVec(reg_al, RealMakeSeg(ds, reg_dx));
    break;
This is DOS's INT 21h function 25h. Standard Borland C startup calls it with AL=0x3F, DS:DX = overlay_manager.
Drop a one-line log there gated on reg_al == 0x3F and you'll see the install:

case 0x25:    /* Set Interrupt Vector */
    if (reg_al == 0x3F) {
        LOG_MSG("[OG_OVR] Borland installing INT 3F handler: ds:dx=%04X:%04X",
                SegValue(ds), reg_dx);
    }
    RealSetVec(reg_al, RealMakeSeg(ds, reg_dx));
    break;
Other places the install could happen:

RealSetVec() at include/mem.h:205 — the underlying IVT write. Putting the log there catches DOS-vector-set calls, BIOS init, and any internal DOSBox setup that touches the same vector — broader than INT 21/25h but noisier.
Direct IVT memory writes at linear addresses 0xFC..0xFF (0x3F * 4). If Borland writes the vector directly via mov es:[3F*4], ax rather than going through DOS, the INT 21/25h hook misses it. We'd need a memory-write watch on those 4 bytes. The CRT pattern usually uses the DOS call, so worth checking the INT 21/25h path first.
The IVT contents at runtime, observed via mem_readd(0x3F*4) from any code with memory access. Once installed, the value persists; reading it after the program has started gives the handler seg:off without needing a callback.
For our case the cleanest is: install the log at dos.cpp:357 to see the install happen, plus a one-time read of mem_readd(0xFC) from the og_sort_trace hook (e.g. inside og_detect_wizards_ds, after WIZARDS' DS has been detected) to cache the handler address. From there the existing INT 3F instruction probe is independent of where the install came from.



PRD written to src/misc/og_overlay_probe.PRD.md. Sections:

Context — why the trace produces opaque runtime CS values today.
Problem — the cross-reference tax on every diagnosis cycle.
Goals — detection, runtime-CS mapping, og_resolve_cs helper, remap resilience, near-zero per-instruction cost.
Non-goals — function-name resolution, INT 3F chain handling, persistence.
Design — CD 3F byte-pattern detection, two strategies (A: read the stub's embedded JMP FAR seg byte at fire time; B: CS-change watcher), fixed 64-entry runtime-CS map, new [OG_OVR] trace lines, (seg_idx=NNNN) annotation on existing probe lines.
Implementation plan — 5 steps.
Risks & open questions — handler reentry, stub byte-layout verification, timing, aliasing.
Success criteria — 4 concrete checks.
Review and tell me what to change before I write code.





https://nbree.blogspot.com/2011/07/turbo-c-overlay-manager.html

https://github.com/NationalSecurityAgency/ghidra/issues/5543


Glossary of Terms
CS      Code Segment
DS      Data Segment
Segment 
IVT     Interrupt Vector Table
