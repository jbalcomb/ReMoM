#ifndef MOM_H
#define MOM_H

#include "MoX.H"
#include "MoM_DEF.H"

#include "MoM_DBG.H"

#include "AdvsrScr.H"
#include "ArmyList.H"
#include "CityList.H"
#include "CityScr.H"
#include "LoadScr.H"
#include "MainMenu.H"
#include "MainScr.H"
#include "MagicScr.H"
#include "SBookScr.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE seg001
*/

// WZD s01p02
// OON XREF:  main()  WZD s01p02
void Screen_Control(void);



#ifdef __cplusplus
}
#endif

#endif  /* MOM_H */
