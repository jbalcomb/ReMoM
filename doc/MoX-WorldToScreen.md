


SCastScr.C
SCastScr.H
/*
    Spell Casting Screen

    WIZARDS.EXE
    ovr070
*/



// WZD o70p04
int16_t World_To_Screen(int16_t map_wx, int16_t map_wy, int16_t * unit_wx, int16_t * unit_wy)


```
map_wx: World X Coordinate of the first square in the Movement Map
map_wy: World Y Coordinate of the first square in the Movement Map
unit_wx: World X Coordinate of the Unit
unit_wy: World Y Coordinate of the Unit
unit_mx: Movement Map X Coordinate of the Unit
unit_my: Movement Map Y Coordinate of the Unit

X,Y of Movement Map, in Squares (top-left corner)
X,Y of Unit on World Map, in Squares

Unit Is In-View
    Translate from World Coordinates to Map Coordinates
```


```
e.g.,

BEGIN: World_To_Screen(map_wx = 7, map_wy = 0, *unit_wx = 18, *unit_wy = 10)

(*unit_wx = 18) >= (map_wx = 7)
(*unit_wx = 18) <=  ((map_wx = 7) + (MAP_WIDTH = 12) = 19)

(*unit_wy = 10) >= (map_wy = 0)
(*unit_wy = 10) <= ((map_wy = 0) + (MAP_HEIGHT = 10) = 10)

Unit Map X,Y:
    unit_mx = *unit_wx - map_wx := (18 - 7) = 11
    unit_my = *unit_wy - map_wy := (10 - 0) = 10

Unit Screen X,Y:
    (*unit_wx = 220) := ((*unit_mx = 11) * (SQUARE_WIDTH = 20)) = 220
    (*unit_wy = 200) := ((unit_my = 10) * (SQUARE_HEIGHT = 18)) + (SQUARE_WIDTH = 20) = 200

END: World_To_Screen(map_wx = 7, map_wy = 0, *unit_wx = 220, *unit_wy = 200) {in_view = 1}
```

```
WTF?
unit_wy = 10
unit_my =  9
unit_wy = 182
(unit_my * SQUARE_HEIGHT) + SQUARE_WIDTH
9 * 18 + 20 = 162 + 20 = 182

12 * 20 - 20 = 240 - 20 = 220
10 * 18 - 18 = 180 - 18 = 162  ... + 20 = 182
```


```
╔═════╦═════╗
║     ║     ║
║     ║     ║
╠═════╬═════╣
║     ║     ║
║     ║     ║
╚═════╩═════╝
```

```
╔═════╤═════╗
║     │     ║
║     │     ║
╟─────┼─────╢
║     │     ║
║     │     ║
╚═════╧═════╝
```

```
World
60 x 40
{0, ..., 59}
{0, ..., 39}
Map
12 x 10
{0, ..., 11}
{0, ...,  9}
```

```
  wx   +1   +2   +3   +4   +5   +6   +7   +7   +7  +10  +11
   0    1    2    3    4    5    6    7    8    9   10   11
╔════╤════╤════╤════╤════╤════╤════╤════╤════╤════╤════╤════╗
║    │    │    │    │    │    │    │    │    │    │    │    ║  1
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  2
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  3
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  4
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  5
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  6
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  7
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  8
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║  9
╟────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────┼────╢
║    │    │    │    │    │    │    │    │    │    │    │    ║ 10
╚════╧════╧════╧════╧════╧════╧════╧════╧════╧════╧════╧════╝
   1    2    3    4    5    6    7    8    9   10   11   12
```