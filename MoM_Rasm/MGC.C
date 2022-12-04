
#include "MGC_main.H"


int main(int argc, char *argv[])
{
#if defined(__VSCode__) || defined(DEBUG) || defined(TEST) || defined(STU_DEBUG) || defined(STU_TEST)
    abort();
#endif

    MGC_main(argc, argv);
    

    return 0;
}
