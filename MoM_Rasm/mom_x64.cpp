// ~== handmade.cpp

#include "mom_x64.h"
//#include "ST_HEAD.H"
//#include "MOM_HEAD.H"
#include "MOM_DEF.H"  // MoX_Quit(); EMM_PAGES_REQUIRED, EMM_MIN_KB, RAM_MIN_KB
#include "MGC_HEAD.H"
#include "ST_EMM.H"  // g_EMM_Pages_Reserved
#include "ST_GUI.H"
#include "ST_VGA.H"
#include "STU_DBG.H"

#include "ST_EMM.C"  // g_EMM_Pages_Reserved



void 
MainLoop()
{
    void *FileContents = PlatformLoadFile("assets.bmp");
}

int MGC_Main(void)
{
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;



    return 0;
}
