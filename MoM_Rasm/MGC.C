

int main(int argc, char *argv[])
{
#ifdef DEBUG
    Debug_Log_Startup();
#endif

#if defined(__VSCode__) || defined(__DOS16__) || defined(DEBUG) || defined(TEST)
    abort();
#endif

    MGC_Main(argc, argv);  // s01p01  seg001.C/H

#ifdef DEBUG
    Debug_Log_Shutdown();
#endif

    return 0;
}
