"""field_catalog — clean-room static catalog of the game's input fields.

Builds a per-call-site record of every Add_*Field registration by parsing the
source directly (no game run): identity + hotkey for every site, and concrete
320x200 geometry wherever the coordinate expression folds to constants. Sites
whose geometry depends on runtime state (loop indices, popup origins, sprite
extents) are flagged runtime=1 for the runtime log to fill in.
"""
