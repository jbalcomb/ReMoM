

outline unit static figure  (~== MoO2 ship_icon / ship_bitmap)

Bottom-Up: How to decide which pixels to replace with the outline color?
...traversing vertically, top to bottom...
...transition from NULL to FALSE...
...eventually, transition from FALSE to TRUE...
...at the point of transition, do we have enough information to make a decision?
...assuming we do...
...set the previous poisition...
...we were on the outside, now we're on the inside...
...if we transition from outside to inside, we get a border/edge/outline pixel..

...initial state assume {outside,inside} = {FALSE,FALSE}...
...so no possible initial transition?


Normally, ...
starts with pixels are transparent
then transitions to pixels that are non-transparent
So, ...
    while(pict_size--)
        if data_byte ==/!= ST_TRANSPARENT...
        ¿ state transition ?  was/is

Blarg!!!
start in an unknown state, and assume so
¿ state-space ?
unknown -> transparent
unknown -> non-transparent

State 0:
round 1


from unknown, to non-transparent
from unknown, to transparent


from transparent, to non-transparent
from non-transparent, to transparent

Yay(data_byte == ST_TRANSPARENT || data_byte == outline_color || data_byte >= 224)
Nay(data_byte == ST_TRANSPARENT || data_byte == outline_color || data_byte >= 224)

from nothing,   to something
from something, to nothing

from NULL, to FALSE
from NULL, to TRUE




Macro?

@@Increment
end of a column / end of a row

Horizontal / Height Wise / Column Wise
itr_height++
if(itr_height >= height)

Veritcal / Width Wise / Row Wise
src_ofst--
src_ofst += height
if( !(src_ofst < pict_size) )
    src_ofst -= pict_size
    src_ofst++
    inside_state = FALSE; outside_state = FALSE;
