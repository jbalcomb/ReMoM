

¿ module/parent is DIPLOMAC/DIPLODEF ?
¿ something that encompases everything from 'Magic Screen', Alchemy Screen', 'Mirror Screen', and 'Diplomacy Screen' ?

'Mirror Screen' shares the lilwiz gem portraits array
'Mirror Screen' borrows the null-terminator from the last string in the diplomacy relations strings array



Data Segment - Uninitialized Data
Data Segment - Uninitialized Data

// wZD dseg:347E                            ¿ BEGIN: DIPLOMAC || Magic Screen - Initialized Data ?
// WZD dseg:347E 01 00                                           GAME_AlchemyDir dw 1                    ; DATA XREF: GAME_AlchemyWindow+19Dr ...
...
...
...
// WZD dseg:369A 4D 41 47 49 43 00                               mirror_magic_lbx_file db 'MAGIC',0      ; DATA XREF: IDK_MirrorScreen_s6343B+78o ...
// WZD dseg:369A                            ¿ END: DIPLOMAC || Magic Screen - Initialized Data ?

// WZD dseg:C1EA                            ¿ BEGIN: DIPLOMAC || Magic Screen - Uninitialized Data ?
// WZD dseg:C1EA 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+magic_dipl_icon_segs dw 0Fh dup(0)      ; DATA XREF: Magic_Screen_Draw:loc_619C6r ...
...
...
...
// WZD dseg:C25C 00 00                                           word_42CFC dw 0                         ; DATA XREF: Mirror_Screen_Draw+478r ...
// WZD dseg:C25C                            ¿ END: DIPLOMAC || Magic Screen - Uninitialized Data ?



What's up with this chunk of data?
Two different code modules / translation units / libraries?

// WZD dseg:958E

// WZD dseg:95CE










Gems
GRAY GEM
    The gray gem indicates that either you have not come into contact with the wizard yet or, if there are fewer than 5 players in the game, the extra gems appear as gray.
SHATTERED GEM
    The shattered gem indicates that the wizard has been eliminated from the game or has been banished.





iterates over _num_players, starting from 1
uses itr?
IDK_CnctWzdsCnt_w42CB4 incremented on each loop
    so, always ends up equal to itr? and _num_players?
    starts from 0
max is 5 ... human_player + Opponents, max of 4
itr = 1, <> 5
    {1,2,3,4}
IDK_CnctWzdsCnt_w42CB4
    {0,1,2,3}
so, 5 players, 4 opponents
    ~=== enemy wizard count
    but, just for 'Magic Screen'

IDK_CnctWzdsCnt_w42CB4

G_GUI_TgtWiz_Labels



// WZD o73p02
Magic_Screen_Draw()

clear help, set help for active and contacted
draw background, ok, alchemy
draw staves
    - lots of calc around locked and pct/pos are sane
print MP, RP, SP
print csting skill, magic reserve, power base, casting, researching, summon to
