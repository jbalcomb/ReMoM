/*
    demo_crash.c — crash reporter for the stand-alone demo harness.

    The demos drive real game code from real saves, so they do occasionally fault.  Without a
    debugger attached all you get is "Segmentation fault" and a wall-clock guess at where it
    happened.  Demo_Install_Crash_Handler() installs an unhandled-exception filter that prints the
    exception code, the faulting address and a symbolized call stack (DbgHelp reads the PDBs the
    MSVC-debug preset already produces), so a fault names the function that raised it.

    Windows-only; on other platforms the installer is a no-op.
*/

#include "demo_harness.h"

#include <stdio.h>

#if defined(_WIN32)

#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

static const char * Demo_Exception_Name(DWORD code)
{
    switch(code)
    {
        case EXCEPTION_ACCESS_VIOLATION:      return "ACCESS_VIOLATION";
        case EXCEPTION_STACK_OVERFLOW:        return "STACK_OVERFLOW";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:    return "INT_DIVIDE_BY_ZERO";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_ILLEGAL_INSTRUCTION:   return "ILLEGAL_INSTRUCTION";
        case EXCEPTION_PRIV_INSTRUCTION:      return "PRIV_INSTRUCTION";
        case EXCEPTION_IN_PAGE_ERROR:         return "IN_PAGE_ERROR";
        case EXCEPTION_BREAKPOINT:            return "BREAKPOINT";
        default:                              return "UNKNOWN";
    }
}

static void Demo_Print_Stack(CONTEXT * context)
{
    HANDLE       process = GetCurrentProcess();
    HANDLE       thread = GetCurrentThread();
    STACKFRAME64 frame;
    CONTEXT      walk_context = *context;
    DWORD        machine;
    int          depth;

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
    if(!SymInitialize(process, NULL, TRUE))
    {
        printf("[crash] SymInitialize failed (%lu); no symbolized stack.\n", (unsigned long)GetLastError());
        return;
    }

    memset(&frame, 0, sizeof(frame));
#if defined(_M_X64)
    machine = IMAGE_FILE_MACHINE_AMD64;
    frame.AddrPC.Offset = walk_context.Rip;
    frame.AddrFrame.Offset = walk_context.Rbp;
    frame.AddrStack.Offset = walk_context.Rsp;
#else
    machine = IMAGE_FILE_MACHINE_I386;
    frame.AddrPC.Offset = walk_context.Eip;
    frame.AddrFrame.Offset = walk_context.Ebp;
    frame.AddrStack.Offset = walk_context.Esp;
#endif
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;

    for(depth = 0; depth < 40; depth++)
    {
        unsigned char       symbol_storage[sizeof(SYMBOL_INFO) + 512];
        SYMBOL_INFO *       symbol = (SYMBOL_INFO *)symbol_storage;
        IMAGEHLP_LINE64     line;
        DWORD64             displacement = 0;
        DWORD               line_displacement = 0;

        if(!StackWalk64(machine, process, thread, &frame, &walk_context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            break;
        }
        if(frame.AddrPC.Offset == 0)
        {
            break;
        }

        memset(symbol_storage, 0, sizeof(symbol_storage));
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = 512 - 1;

        memset(&line, 0, sizeof(line));
        line.SizeOfStruct = sizeof(line);

        if(SymFromAddr(process, frame.AddrPC.Offset, &displacement, symbol))
        {
            if(SymGetLineFromAddr64(process, frame.AddrPC.Offset, &line_displacement, &line))
            {
                printf("[crash]  #%02d %s  (%s:%lu)\n", depth, symbol->Name, line.FileName, (unsigned long)line.LineNumber);
            }
            else
            {
                printf("[crash]  #%02d %s + 0x%llx\n", depth, symbol->Name, (unsigned long long)displacement);
            }
        }
        else
        {
            printf("[crash]  #%02d 0x%016llx\n", depth, (unsigned long long)frame.AddrPC.Offset);
        }
    }

    SymCleanup(process);
}

static LONG WINAPI Demo_Crash_Filter(EXCEPTION_POINTERS * info)
{
    DWORD code = info->ExceptionRecord->ExceptionCode;

    printf("\n[crash] %s (0x%08lX) at 0x%016llx\n", Demo_Exception_Name(code), (unsigned long)code, (unsigned long long)(uintptr_t)info->ExceptionRecord->ExceptionAddress);
    if((code == EXCEPTION_ACCESS_VIOLATION) && (info->ExceptionRecord->NumberParameters >= 2))
    {
        printf("[crash] %s address 0x%016llx\n", (info->ExceptionRecord->ExceptionInformation[0] != 0) ? "writing" : "reading", (unsigned long long)info->ExceptionRecord->ExceptionInformation[1]);
    }
    fflush(stdout);

    Demo_Print_Stack(info->ContextRecord);
    fflush(stdout);

    return EXCEPTION_EXECUTE_HANDLER;   /* terminate; we've reported */
}

void Demo_Install_Crash_Handler(void)
{
    SetUnhandledExceptionFilter(Demo_Crash_Filter);
}

#else /* !_WIN32 */

void Demo_Install_Crash_Handler(void)
{
}

#endif /* _WIN32 */
