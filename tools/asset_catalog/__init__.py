"""asset_catalog — inventory of every graphics asset in the game's LBX archives.

Parses assets/*.LBX directly (no game run) into a per-sprite catalog: archive,
entry number, name, and 320x200-space width/height/frame count from each FLIC
header. Feeds the graphics upscale/rearrange projects and (with the runtime
draw-trace) resolves a click to the specific drawn sprite for HMS naming.
"""
