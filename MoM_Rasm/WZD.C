
/*
    SEE: MoM-WIZARDS_EXE.md
*/

#include "WZD_main.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


int main(int argc, char *argv[])
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif

#if defined(__VSCode__) || defined(__DOS16__) || defined(DEBUG) || defined(TEST) || defined(STU_DEBUG) || defined(STU_TEST)
    abort();
#endif


    WZD_main(argc, argv);


#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif
    return 0;
}
