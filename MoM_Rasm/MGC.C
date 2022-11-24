
#include "MGC_main.H"


int main(int argc, char *argv[])
{
#ifdef STU_DEBUG
    abort();
#endif

    MGC_main(argc, argv);
    

    return 0;
}
