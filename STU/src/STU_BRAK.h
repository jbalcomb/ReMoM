/*
    SimTex Universe (STU)
    Crash reporting

    Turns a bare "exit code 3" or "Segmentation fault" into a named function and line number.

    Install once at startup.  On a fatal fault the handler:
      1. flushes STU_LOG  -- the log is buffered, so without this an abort discards everything the
         run had logged, which is exactly the failure that motivated this module,
      2. writes the fault reason and a symbolized call stack to stderr (unbuffered),
      3. terminates.

    Catches BOTH families of fatal condition, which is the point:
      - CPU faults: access violation / SIGSEGV, illegal instruction, divide by zero, stack overflow.
      - abort(): a failed assert(), or a bare abort().  On Windows an unhandled-exception filter does
        NOT see abort() -- it exits with code 3 and no output at all.  Catching SIGABRT is what makes
        assertion failures reportable.

    Windows uses DbgHelp against the PDBs the MSVC presets already emit.  POSIX uses
    backtrace()/backtrace_symbols_fd(); build with -g (and -rdynamic for names in non-static builds).
*/

#ifndef STU_BRAK_H
#define STU_BRAK_H



#ifdef __cplusplus
extern "C" {
#endif



/*
    Install the crash handlers.  Safe to call more than once (subsequent calls are ignored).
    Call as early in main() as possible -- ideally right after logging is initialized, so the
    log-flush on crash has somewhere to flush to.
*/
void STU_BRAK_Install(void);

/*
    Dump the current call stack to stderr without crashing.  Useful for "how did we get here?"
    questions and for confirming the symbolization actually works on this build.
    'reason' is printed as the header; NULL is allowed.
*/
void STU_BRAK_Report(const char * reason);



#ifdef __cplusplus
}
#endif

#endif /* STU_BRAK_H */
