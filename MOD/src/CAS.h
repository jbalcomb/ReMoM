/*
    CAS -- interpreter for Caster of Magic for Windows ".CAS" scripts.

    Language (Caster of Magic's Scripts.TXT, with the details Warlord's scripts rely on):
      - statements end with ';'; `:text:` is a comment (may span lines); `!NAME!` is a label; keywords and names are
        case-insensitive, string constants keep their case and spaces
      - numbers are exact fractions ("A/B"); strings ("...", names ending in $) are at most 255 characters
      - expressions have no operator precedence and are evaluated right-recursively: `A+B*C` is `A+(B*C)`,
        `A-B-C` is `A-(B-C)` (the scripts bracket anything else); prefix %R %I %F %N, infix + - * / = < > <= >= <>
        %AND %OR %XOR, comparisons give 1 or 0
      - IF c THEN { } [ELSE { }]; FOR v=a TO b [STEP s] ... NEXT [v] (runs at least once, inclusive); LOOP [c] { }
        ENDLP [c]; GOTO "L"; CALL "L" / RETURN; LOCAL a,b; DIMNUM / DIMSTR a[n,m] (1-based); HALT;
        MASTERVARSET v = e; commands `NAME arg, arg, ...;`; functions `NAME(arg, ...)`
      - a script's own variables live for one run; the master script's variables (MASTER.CAS) are readable from every
        script and written only through MASTERVARSET

    Built-ins: every documented name is known.  Those not wired to ReMoM yet fail the script with
    "not implemented in ReMoM yet" (the hook keeps its value) -- see CAS_Builtin_Implemented().
*/

#ifndef CAS_H
#define CAS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct s_CAS_SCRIPT CAS_Script;

/* Tokenizes a script.  Returns NULL on a lexical error (CAS_Last_Error() says where). */
CAS_Script * CAS_Load_String(const char * name, const char * source);
CAS_Script * CAS_Load_File(const char * path);
void CAS_Free(CAS_Script * script);

/* Message of the most recent load / check / run error ("" if none), with file and line. */
const char * CAS_Last_Error(void);

/* Things CoM2 accepts that the language does not (missing ';' or ')', a stray ')', an unclosed comment or string),
   recorded by the last CAS_Load_* / CAS_Check, one "file:line: what" per line.  ReMoM accepts them the same way. */
const char * CAS_Warnings(int * count);

/* Parses every statement without running anything.  Returns 0 if the script is well-formed, -1 otherwise. */
int CAS_Check(CAS_Script * script);

/* For each built-in the script names: callback(name, uses, implemented). */
typedef void (*CAS_Builtin_Use_Fn)(const char * name, int uses, int implemented, void * user_data);
void CAS_Builtins_Used(CAS_Script * script, CAS_Builtin_Use_Fn callback, void * user_data);

/* 1 if `name` is a known built-in that ReMoM implements, 0 if known but not implemented, -1 if unknown. */
int CAS_Builtin_Implemented(const char * name);

/* Runs the master script; its variables become readable from every later run.  Returns 0 on success. */
int CAS_Run_Master(CAS_Script * script);
void CAS_Clear_Master(void);

/* A numeric input variable for a run. */
struct s_CAS_INPUT
{
    const char * name;
    int32_t value;
};

/* Runs a script with numeric input variables preset, then reads the numeric variable `result_name` (truncated to an
   integer) into *result.  *result keeps its value if the script never sets it.  Returns 0 on success, -1 on a
   script error (CAS_Last_Error()). */
int CAS_Run(CAS_Script * script, const struct s_CAS_INPUT * inputs, int input_count, const char * result_name, int32_t * result);

/* ADDRESLINE calls made by the last successful CAS_Run, and their total (truncated) in *sum.  CoM2's economy scripts
   report their result this way instead of through a variable. */
int CAS_Last_Result_Lines(int32_t * sum);

/* Hard limit on executed statements per run, against runaway loops. */
#define CAS_MAX_STEPS 20000000L

#ifdef __cplusplus
}
#endif

#endif  /* CAS_H */
