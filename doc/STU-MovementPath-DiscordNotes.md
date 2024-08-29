

JimBalcomb — 10/15/2023 10:14 PM
Looking for the moment when it sets the unit's x,y to destination map square x,y.
Figuring on working back into where it checks the unit's 'movement mode' against the 'terrain type', the unit's 'movement points' against the 'terrain type' 'movement cost', and the unit's attributes that impact the 'movement cost'.
Probably just going to hard-code everything to 1, for the time being.


JimBalcomb — 10/15/2023 11:00 PM
Hrrmmm... looks like there is some sort 'stack move type' variable...
Also, some 'type' for the entity that is on the destination map square.
...presumably, where it handles running into a city, enemy city, stack, enemy stack, lair, node, tower of wizardry, etc.
(I shall handle none of these in this phase of the effort)


JimBalcomb — 10/15/2023 11:12 PM
default is 0; first thing 'get stack paths'; passes 'movement modes' from the array individually; 
Does the whole path logic, even for a path length of 1 - the very next map square


JimBalcomb — 10/15/2023 11:29 PM
Alright. So, at the least, the only program-paths that outright bail are 0 'movement points' and 0 'path length'.
Shoved off the entirety, so no idea how you'd get a 'movement path' with a 'path length' of 0.


JimBalcomb — 10/15/2023 11:36 PM
And, this is where the evening's effort falls off.
It needs the 'movement path' ¿array? to do the balance of the logic.
And, I have not previously had cause to come to terms with what a 'movement path' is for MoM. 
So, pause here, until I work that out.
I have the highest of hopes that it is just arrays of X and Y coordinates in series, which should be super-simple to fabricate.
...it's stack-wise so it shouldn't/couldn't include any additional unit-level information - no 'movement mode', 'movement cost', etc.
...could include 'terrain type', but that's dynamic per-move/turn, so probably wouldn't


JimBalcomb — 10/16/2023 8:24 AM
There is a lot more going on here that I had hoped there would be.
...especially complicated by the fact that there is a 'movement path' cache in Expanded Memory.
Looks like drake178 has explored every corner of this, for some particular patch he made, but his notes do not include the nature or format of a 'movement path'.
(lots of notes on bugs) 
Not sure it matters for my effort, here and now, but this is also used for the unit movement in combat.


JimBalcomb — 10/16/2023 8:44 AM
'movement path' is a struct? sizeof 118? but, max 'movement path' is 140?
(Not quite. OVL_Path_Costs, Xs, Ys are [35]) 
something else is a struct? sizeof 60? ... ~== Y Per X?
...allocation is 28,832 bytes...


JimBalcomb — 10/16/2023 9:17 AM
Still not sure what you'd call it or if it's even covered by some CS concept, but the programming paradigm here is like with the lists for the ArmyList and City, where it has separate arrays for each factor of the algorithm...
...path length total, path length of available movement points, path length to 'obstacle', path length to 'enemy', 'movement cost per path step', etc. 
...table of path lengths by path length
I'm assuming there is something in here that gets to selecting the Best Way, like open-shortest-path-first.


JimBalcomb — 10/16/2023 10:38 AM
Huzzah! Finally found the code that sets the unit's x,y to the destination x,y.


JimBalcomb — 10/16/2023 10:47 AM
If I am interpreting this correctly, how it backs into the 'movement path' table is that, at this point, the 'path length' has been updated to be the length of the path that the 'Stack' can afford to make for this one 'stack move', based on the useable 'movement points' determined for the whole 'Stack' and the accumulated cost of the steps in the 'movement path'.
Then, it just loops through that many 'path steps' of the 'movement path', updating each 'Unit' as it steps and redrawing the screen to show the stack moving, if it's for the 'human player' or 'Display: Enemy Moves' is set. 
So, I just need to hack in some code to create a 'movement path' of 1, where the only element in the arrays is for the destination x,y that was calculated by the direction keys function.


JimBalcomb — 10/16/2023 9:50 PM
Not quite. It gets the destination x,y by indexing the array with ['path_length' - 1]. 
So, the 'path array' is the coordinate per step, starting from step-0.


JimBalcomb — 10/17/2023 2:57 PM
Got through to the moving of the units code. So, as soon as I finish working out how these movement path arrays work, the units should just start moving.
Looks like about 30% of the balance of the code is just for dealing with moving the movement map view, as the units progress through the movement path, for the amount of the path length that they can afford to move.
Somewhere in there, it did occur to me that this will not work for the left click movement, because if you left click on a map square that is not adjacent to the current map square, what I have coded up will just cause the units to jump to where you clicked.
So, I'm going to push forward from here just to effect the state where the next turn button becomes available. And, then work through the next turn procedure.
But, the very next thing probably ought to be at least adding the proper path building logic.


JimBalcomb — 10/17/2023 6:32 PM
Yeah, the underlying pathfinding algorithms are a whole other business of their own. I have not yet had cause to implement one, so that part should be quite interesting.
I saw some of Drake's notes about patching MoM's algorithm and it sounded like it actually just uses a line drawing algorithm and then a bunch of logic to make the path make sense given the terrain type, stack attributes, and movement cost modifiers.


JimBalcomb — 10/18/2023 10:36 PM
There's a little something extra in Stack_Moves()  - under some (curious) condition, it has the option of updating some/one unit's 'moves' and 'finished flag'.


JimBalcomb — 10/19/2023 12:21 PM
More good news, looks like the 'movement path cache' is only used for 'computer player' moves.
So, I don't need to bother with that, for now, if ever.
drake178 has a note that he removed it completely in his "overland djikstra patch".
Tid-Bit: There's debug variables in MoM to track the count of 'movement path cache' hits and misses.


JimBalcomb — 10/19/2023 2:38 PM
Hrrrmm... the function that fetches the movement path or builds it has parameters for a flag and value that get passed ST_TRUE and movement_points, but promptly overwrites the passed in parameters with ST_TRUE and 8.
Don't seem to recall there being a hard-limit on movement points. Wonder what happens in OG-MoM if you set a Go-To with a left-click for a stack/unit that has >8 movement points.


JimBalcomb — 10/19/2023 3:09 PM
Found the function that is the 'movement path' pathing finding algorithm.
It's about ~1000 of assembly code... 😐
Looking like it just scans across the map, row-wise, and just keeps checking every adjacent map square for a lower movement point cost.
Not seeing if/where it takes in to account the movement point cost modification based on the stack's movement mode attributes.


JimBalcomb — 10/19/2023 3:17 PM
In any case, this'd be where I'd need to find the closest option for including assembly code directly in the project or figure out how to code up the equivalent in C directly and figure out how to capture (enough) data from OG-MoM to confirm that my code is equivalent.
...which makes this also feel like where it'd be worth my while to figure out how to capture and replay input, to avoid many tedious hours of building up the data-set.


JimBalcomb — 10/19/2023 3:25 PM
In the mean-time, I'ma just hack up the dumbest diagonal, X-major algo possible.
...after I go back to working on what I am supposed to be working on... :/


JimBalcomb — 10/19/2023 3:35 PM
PS. It works off the index into the world map array, not the x,y.


JimBalcomb — 10/19/2023 3:40 PM
There's a function called before that that gets passed the movement modes and builds the array of movement costs for the whole map.
...which means this whole 'movement modes' business is all/only for stack-level attributes


JimBalcomb — 10/19/2023 3:59 PM
Still not sure what's up with these movement path arrays...
...some code cross-references the address directly, some code is indexing an array, some code includes a -1 on the address, some code includes a -2 on the address... 
...all after doing a memcpy() on the individual members of the movement paths structure.


JimBalcomb — 10/20/2023 2:32 PM
Still can't seem to whittle this down to or break it into chunks that will fit in my working memory.


JimBalcomb — 10/20/2023 2:37 PM
Did determined that there is only one block that indexes the movement path arrays with -2 and that it is a loop that is just shifting the values up one element.
Looks to only be for 'Build Road'.
And, only for some circumstance where the stack is on a 'movement path', but not at the start of the 'movement path'?
Pretty sure that shifting logic also inherently means there must be an additional element before the last one that it shifts, or it would underrun.


JimBalcomb — 10/20/2023 3:37 PM
Yeah, alright... I mixed something up at some point with which parts just for the world map cost/passible and which parts just use that to build the movement paths.
So, there's a big array that is a copy of both planes populated with the movement points cost for the given terrain type, which gets updated when there's a terrain change.


JimBalcomb — 10/20/2023 3:44 PM
Then, another array that is a copy of that for the current plane upon which is applied the movement point cost modifiers for roads and the stack's movement modes. It just if-thens through the movement modes and bulk applies the modification to the entire temporary array here.
There appears to be some inherent hierarchy to the movement modes/terrain types, because it doesn't test the movement mode against the terrain type.
There's another function that runs back through and updates that temporary array for impassible map squares.


JimBalcomb — 10/20/2023 3:54 PM
That temporary array of modified movement costs is what gets used by the pathfinding algorithm.


JimBalcomb — 10/20/2023 4:11 PM
No chance of me making sense of the disassembly code for the path finding algorithm, but, based on what happens next, it must be updating that temporary array such that the value at each world map idx is the world map idx to use to reach the current one.
So, you initially index it with ((destination y * WORLD_WIDTH) + destination x), that gets you the next world map idx and you iterate until there's no valid value or you hit max path.
You invert the calculation to get x,y while you are iterating and however many iterations it took is the path length.


JimBalcomb — 10/20/2023 5:59 PM
Made everything in the disassembly look like it makes sense for the addresses and offsets, by settings the first two values for x and y to be independent variables...
...but, also makes it look like nothing is setting those values...


JimBalcomb — 10/20/2023 8:04 PM
Can't seem to figure out what the C code would look like to make this make sense, but it seems to be working well-enough with just casting to pointers and dereferencing from offsets.
At the least, it's back to compiling, running, and not crashing.
So, now I'm into manufacturing the data for the x, y, and cost arrays to see about getting 'Move Units' do a/the move.


JimBalcomb — 10/21/2023 6:23 PM
Hey... Howzer'bout I got to the code that updates the 'movement points' after the round of the 'movement path'.
So, somehow, because it gets the minimum movement points for the stack and collapses the array of movement modes, with their costs in the temporary movement path array, and pre-calculates the total movement cost for this portion of the movement path, it just iterates over the units and subtracts the final movement cost from their movement points.


JimBalcomb — 10/21/2023 6:34 PM
This whole program-path is so tightly coupled/fragile, it's going to have get thrown right out the window, as soon as we want to change any part of the game-behavior around movement, movement modes, or movement points.
The only thing I can see reasonably shimming in here is like how drake178 did with the path-finding algorithm.
...and, dropping the movement path cache, because we now have thousands of millions of CPU cycles to spare.


JimBalcomb — 10/22/2023 3:36 PM
Oh. Suddenly just came to understand what's going on with all this 'move stages' code.
Nothing to do with moving the map or the moves for the path length.
It's all just for animating the stack moving from one map square to the next.
So, it draws the unit picture at its location on the screen, plus the amount of map square width / stages, for the count of stages.
It is still weird that it does the calculation, but then is hard-coded to 2 stages.
So, it draws the unit picture where it actually is, draws the unit picture shifted half a map square towards its destination, and then draws the unit picture shifted another half a map square towards its destination, which is now just actually its destination.


JimBalcomb — 10/22/2023 5:11 PM
Interesting.
So, 'Move Units' sets the x,y for all the units in the stack to the final destination x,y...
...then gets around to drawing what that movement would actually look like.
Buncha business floating around what with what might need to be updated for what should be shown on the screen.
But, other than the unit's x,y there, the handling of all the rest of the impact on the game-data is back in 'Move Stack'.


JimBalcomb — 10/22/2023 5:18 PM
All of which would seem to mean that what I've been calling 'Move Units' is really just 'Move Stack Draw', with the assumption of the side-effect that the stack will just magically end up where it now belongs on the world map.


JimBalcomb — 10/22/2023 5:25 PM
Actually, given the code-module boundaries, ...
Do_Move_Stack() |-> Move_Stack() |-> Move_Units() should be
Move_Stack() |-> Move_Units() |-> Move_Units_Draw()
...and, gonna break the hell out of my notes. 😐


JimBalcomb — 10/22/2023 6:05 PM
Bit more of the struggle with the 'movement path' arrays...
...but, got the right values into the right places...

