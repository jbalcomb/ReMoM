/*
    SimTex Universe (STU)
    Crash reporting  -- see STU_BRAK.h
*/

#include "STU_BRAK.h"
#include "STU_LOG.h"

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <dbghelp.h>
#if defined(_MSC_VER)
#pragma comment(lib, "dbghelp.lib")
#endif
#else
#include <stdlib.h>
#if defined(__GLIBC__) || defined(__APPLE__)
#define STU_BRAK_HAVE_BACKTRACE 1
#include <execinfo.h>
#include <unistd.h>
#endif
#endif



static int m_installed = 0;

/* Re-entrancy guard: a fault inside the handler must not recurse forever. */
static int m_in_handler = 0;



static const char * STU_BRAK_Signal_Name(int sig)
{
    switch(sig)
    {
        case SIGABRT: return "SIGABRT (abort / failed assertion)";
        case SIGSEGV: return "SIGSEGV (invalid memory access)";
        case SIGILL:  return "SIGILL (illegal instruction)";
        case SIGFPE:  return "SIGFPE (arithmetic error)";
        default:      return "unknown signal";
    }
}


/*
    Flush the log BEFORE anything else.  STU_LOG buffers, so a crash otherwise discards the whole
    run's log -- including the lines that would explain the crash.
*/
static void STU_BRAK_Flush_Log(void)
{
    STU_Log_Flush_All();
    fflush(stdout);
    fflush(stderr);
}


/*
    Emit one report line to BOTH stderr and the log.

    The log copy is not redundant: ReMoMber calls freopen("CON", "w", stderr) during startup, which
    reattaches stderr to the console device, so `2> file` captures nothing from that point on and a
    crash report written only to stderr is invisible to any scripted or unattended run.  The log file
    is the only destination that survives.

    (Neither fprintf nor the logger is async-signal-safe.  In a process that is already going down
    with a corrupt-or-worse state, a readable stack trace is worth more than strict signal-safety;
    this is the usual trade-off for an in-process crash reporter.)
*/
static void STU_BRAK_Emit(const char * format, ...)
{
    char    line[1024];
    va_list args;

    va_start(args, format);
    vsnprintf(line, sizeof(line), format, args);
    va_end(args);

    fprintf(stderr, "%s\n", line);
    fflush(stderr);

    LOG_FATAL(LOG_CAT_GENERAL, "%s", line);
}



/* ========================================================================= */
/*  Windows                                                                  */
/* ========================================================================= */
#if defined(_WIN32)

static const char * STU_BRAK_Exception_Name(DWORD code)
{
    switch(code)
    {
        case EXCEPTION_ACCESS_VIOLATION:      return "ACCESS_VIOLATION";
        case EXCEPTION_STACK_OVERFLOW:        return "STACK_OVERFLOW";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:    return "INT_DIVIDE_BY_ZERO";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:    return "FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_ILLEGAL_INSTRUCTION:   return "ILLEGAL_INSTRUCTION";
        case EXCEPTION_PRIV_INSTRUCTION:      return "PRIV_INSTRUCTION";
        case EXCEPTION_IN_PAGE_ERROR:         return "IN_PAGE_ERROR";
        case EXCEPTION_BREAKPOINT:            return "BREAKPOINT";
        default:                              return "UNKNOWN";
    }
}


static void STU_BRAK_Print_Stack(CONTEXT * context, int frames_to_skip)
{
    HANDLE       process = GetCurrentProcess();
    HANDLE       thread = GetCurrentThread();
    STACKFRAME64 frame;
    CONTEXT      walk_context = *context;
    DWORD        machine;
    int          depth;
    int          printed = 0;

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
    if(!SymInitialize(process, NULL, TRUE))
    {
        STU_BRAK_Emit("[stu-brak]   (SymInitialize failed, %lu -- no symbols)\n", (unsigned long)GetLastError());
        return;
    }

    memset(&frame, 0, sizeof(frame));
#if defined(_M_X64) || defined(__x86_64__)
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

    for(depth = 0; depth < 64; depth++)
    {
        unsigned char   symbol_storage[sizeof(SYMBOL_INFO) + 512];
        SYMBOL_INFO *   symbol = (SYMBOL_INFO *)symbol_storage;
        IMAGEHLP_LINE64 line;
        DWORD64         displacement = 0;
        DWORD           line_displacement = 0;

        if(!StackWalk64(machine, process, thread, &frame, &walk_context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        {
            break;
        }
        if(frame.AddrPC.Offset == 0)
        {
            break;
        }
        if(depth < frames_to_skip)
        {
            continue;   /* hide the handler's own frames */
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
                STU_BRAK_Emit("[stu-brak]   #%02d %s  (%s:%lu)\n", printed, symbol->Name, line.FileName, (unsigned long)line.LineNumber);
            }
            else
            {
                STU_BRAK_Emit("[stu-brak]   #%02d %s + 0x%llx\n", printed, symbol->Name, (unsigned long long)displacement);
            }
        }
        else
        {
            STU_BRAK_Emit("[stu-brak]   #%02d 0x%016llx\n", printed, (unsigned long long)frame.AddrPC.Offset);
        }
        printed++;
    }

    SymCleanup(process);
}


static LONG WINAPI STU_BRAK_Exception_Filter(EXCEPTION_POINTERS * info)
{
    DWORD code;

    if(m_in_handler != 0)
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }
    m_in_handler = 1;

    STU_BRAK_Flush_Log();

    code = info->ExceptionRecord->ExceptionCode;
    STU_BRAK_Emit("\n[stu-brak] %s (0x%08lX) at 0x%016llx\n", STU_BRAK_Exception_Name(code), (unsigned long)code, (unsigned long long)(size_t)info->ExceptionRecord->ExceptionAddress);
    if((code == EXCEPTION_ACCESS_VIOLATION) && (info->ExceptionRecord->NumberParameters >= 2))
    {
        STU_BRAK_Emit("[stu-brak] %s address 0x%016llx\n", (info->ExceptionRecord->ExceptionInformation[0] != 0) ? "writing" : "reading", (unsigned long long)info->ExceptionRecord->ExceptionInformation[1]);
    }
    fflush(stderr);

    STU_BRAK_Print_Stack(info->ContextRecord, 0);
    fflush(stderr);

    return EXCEPTION_EXECUTE_HANDLER;
}


/*
    Signal path.  This is what catches abort() -- including a failed assert(), which on Windows
    otherwise exits with code 3 and prints nothing useful.  The unhandled-exception filter above
    never sees abort(), so without this the most common fatal case is invisible.
*/
static void STU_BRAK_Signal_Handler(int sig)
{
    CONTEXT context;

    if(m_in_handler != 0)
    {
        _exit(3);
    }
    m_in_handler = 1;

    STU_BRAK_Flush_Log();

    STU_BRAK_Emit("\n[stu-brak] %s\n", STU_BRAK_Signal_Name(sig));
    fflush(stderr);

    /* No CONTEXT is delivered with a signal, so capture the current one and walk from here.  The
       top frames are this handler and the CRT's raise/abort plumbing; skip a few of them. */
    memset(&context, 0, sizeof(context));
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);
    STU_BRAK_Print_Stack(&context, 3);
    fflush(stderr);

    _exit(3);
}


void STU_BRAK_Install(void)
{
    if(m_installed != 0)
    {
        return;
    }
    m_installed = 1;

    SetUnhandledExceptionFilter(STU_BRAK_Exception_Filter);

    signal(SIGABRT, STU_BRAK_Signal_Handler);
    signal(SIGSEGV, STU_BRAK_Signal_Handler);
    signal(SIGILL,  STU_BRAK_Signal_Handler);
    signal(SIGFPE,  STU_BRAK_Signal_Handler);

#if defined(_MSC_VER)
    /* Do not pop the "abort has been called" dialog -- it blocks unattended/scripted runs forever
       instead of failing.  We report the abort ourselves above. */
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
}


void STU_BRAK_Report(const char * reason)
{
    CONTEXT context;

    STU_BRAK_Emit("\n[stu-brak] report: %s\n", (reason != NULL) ? reason : "(no reason given)");
    memset(&context, 0, sizeof(context));
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);
    STU_BRAK_Print_Stack(&context, 1);
    fflush(stderr);
}



/* ========================================================================= */
/*  POSIX                                                                    */
/* ========================================================================= */
#else

static void STU_BRAK_Print_Stack_Posix(int frames_to_skip)
{
#if defined(STU_BRAK_HAVE_BACKTRACE)
    void * addresses[64];
    int    count;

    count = backtrace(addresses, (int)(sizeof(addresses) / sizeof(addresses[0])));
    if(count > frames_to_skip)
    {
        backtrace_symbols_fd(addresses + frames_to_skip, count - frames_to_skip, fileno(stderr));
    }
#else
    (void)frames_to_skip;
    STU_BRAK_Emit("[stu-brak]   (no backtrace support in this build)\n");
#endif
}


static void STU_BRAK_Signal_Handler(int sig)
{
    if(m_in_handler != 0)
    {
        _exit(3);
    }
    m_in_handler = 1;

    STU_BRAK_Flush_Log();

    STU_BRAK_Emit("\n[stu-brak] %s\n", STU_BRAK_Signal_Name(sig));
    fflush(stderr);

    STU_BRAK_Print_Stack_Posix(2);
    fflush(stderr);

    /* Restore the default and re-raise, so the shell sees the real signal and any core dump is
       still produced. */
    signal(sig, SIG_DFL);
    raise(sig);
}


void STU_BRAK_Install(void)
{
    if(m_installed != 0)
    {
        return;
    }
    m_installed = 1;

    signal(SIGABRT, STU_BRAK_Signal_Handler);
    signal(SIGSEGV, STU_BRAK_Signal_Handler);
    signal(SIGILL,  STU_BRAK_Signal_Handler);
    signal(SIGFPE,  STU_BRAK_Signal_Handler);
    signal(SIGBUS,  STU_BRAK_Signal_Handler);
}


void STU_BRAK_Report(const char * reason)
{
    STU_BRAK_Emit("\n[stu-brak] report: %s\n", (reason != NULL) ? reason : "(no reason given)");
    STU_BRAK_Print_Stack_Posix(1);
    fflush(stderr);
}

#endif /* _WIN32 */
