
#include "MGC_main.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


int main(int argc, char *argv[])
{
#ifdef STU_DEBUG
    Debug_Log_Startup();
#endif


    MGC_main(argc, argv);


#ifdef STU_DEBUG
    Debug_Log_Shutdown();
#endif
    return 0;
}
