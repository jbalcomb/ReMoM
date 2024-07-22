
# Outpost Screen

WIZARDS.EXE
ovr077

SEEALSO:  WIZARDS.EXE  ovr100  SETTLE.C
SEEALSO:  Moo2  Module: COLONIZE




## New Outpost Flag

Main_Screen()
    /* #### Section 9.2.3.1.1      Right-Click Movement Map - City - Own - Outpost */
    Outpost_Screen(ST_FALSE);

Outpost_Screen()
    m_new_outpost_flag = flag;

Outpost_Screen_Draw()
    print "New Outpost Founded" or "Outpost Of [city name]"
