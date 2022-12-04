
#include "WZD_main.H"


int main(int argc, char *argv[])
{
#if defined(__VSCode__) || defined(DEBUG) || defined(TEST) || defined(STU_DEBUG) || defined(STU_TEST)
    abort();
#endif

    WZD_main(argc, argv);
    

    return 0;
}
