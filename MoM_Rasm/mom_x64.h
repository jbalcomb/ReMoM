// ~== handmade.h

#if !defined(MOM_X64_H)

static bool g_KbHit;

#if !defined(__DOS16__)
//#define getch() (kbhit())
#define getch() { while(!g_KbHit) {} }
#endif


void *PlatformLoadFile(char* Filename);// Not the actual API! Just an example


#define MOM_X64_H
#endif
