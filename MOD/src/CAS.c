/*
    CAS -- interpreter for Caster of Magic for Windows ".CAS" scripts.  See CAS.h for the language.

    The script is tokenized once (comments stripped, names upper-cased, labels and brace pairs indexed) and then
    interpreted straight from the token array, the way a BASIC interpreter runs its program text: FOR/NEXT pair up at
    run time, GOTO may jump to any label, CALL/RETURN keep a stack.  The same statement code runs in "check" mode
    (no evaluation, every statement visited once in order) for CAS_Check().
*/

#include "CAS.h"
#include "CAS_Game.h"

#include "../../STU/src/STU_LOG.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/random.h"

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAS_NAME_MAX     31
#define CAS_STRING_MAX   255
#define CAS_MAX_DIMS     4
#define CAS_MAX_FOR      64
#define CAS_MAX_CALL     64
#define CAS_MAX_ARGS     8
#define CAS_ERROR_MAX    512

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Numbers: exact fractions n/d, d > 0, reduced.  Values that would overflow fall back to a 1/1000000 grid.        */
/* ---------------------------------------------------------------------------------------------------------------- */

typedef struct { int64_t n; int64_t d; } cas_num;

static int64_t cas_gcd(int64_t a, int64_t b)
{
    if(a < 0) { a = -a; }
    if(b < 0) { b = -b; }
    while(b != 0) { int64_t t = a % b; a = b; b = t; }
    return (a == 0) ? 1 : a;
}

static cas_num cas_make(int64_t n, int64_t d)
{
    cas_num r;
    int64_t g;
    if(d < 0) { n = -n; d = -d; }
    g = cas_gcd(n, d);
    r.n = n / g;
    r.d = d / g;
    return r;
}

static cas_num cas_int(int64_t n) { cas_num r; r.n = n; r.d = 1; return r; }

static double cas_double(cas_num a) { return (double)a.n / (double)a.d; }

static cas_num cas_from_double(double x)
{
    return cas_make((int64_t)llround(x * 1000000.0), 1000000);
}

#define CAS_SAFE_LIMIT ((int64_t)1 << 31)

static int cas_small(cas_num a) { return (a.n > -CAS_SAFE_LIMIT) && (a.n < CAS_SAFE_LIMIT) && (a.d < CAS_SAFE_LIMIT); }

static cas_num cas_add(cas_num a, cas_num b)
{
    if(!cas_small(a) || !cas_small(b)) { return cas_from_double(cas_double(a) + cas_double(b)); }
    return cas_make(a.n * b.d + b.n * a.d, a.d * b.d);
}

static cas_num cas_sub(cas_num a, cas_num b) { b.n = -b.n; return cas_add(a, b); }

static cas_num cas_mul(cas_num a, cas_num b)
{
    if(!cas_small(a) || !cas_small(b)) { return cas_from_double(cas_double(a) * cas_double(b)); }
    return cas_make(a.n * b.n, a.d * b.d);
}

static int cas_cmp(cas_num a, cas_num b)
{
    long double l = (long double)a.n * (long double)b.d;
    long double r = (long double)b.n * (long double)a.d;
    return (l < r) ? -1 : ((l > r) ? 1 : 0);
}

static int64_t cas_trunc(cas_num a) { return a.n / a.d; }                     /* %I: integer part, toward zero */

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Tokens                                                                                                          */
/* ---------------------------------------------------------------------------------------------------------------- */

enum e_CAS_TOKEN { TK_END, TK_NUM, TK_STR, TK_NAME, TK_LABEL, TK_OP, TK_LPAREN, TK_RPAREN, TK_LBRACK, TK_RBRACK, TK_LBRACE, TK_RBRACE, TK_COMMA, TK_SEMI };

/* Binary operators (TK_OP.op) and prefix operators. */
enum e_CAS_OP { OP_ADD = 1, OP_SUB, OP_MUL, OP_DIV, OP_EQ, OP_LT, OP_GT, OP_LE, OP_GE, OP_NE, OP_AND, OP_OR, OP_XOR,
                OP_ROUND, OP_INT, OP_FRAC, OP_NOT };

enum e_CAS_BLOCK { BLOCK_THEN = 1, BLOCK_ELSE, BLOCK_LOOP };

struct s_CAS_TOKEN
{
    int type;
    int op;
    int line;
    cas_num num;
    char * text;          /* names (upper-cased), labels (upper-cased), string constants (as written) */
};

struct s_CAS_LABEL
{
    char * name;
    int target;           /* token index just after the label */
};

struct s_CAS_SCRIPT
{
    char * name;
    struct s_CAS_TOKEN * tokens;
    int count;
    int * match;          /* for each '{' / '}', the index of its partner; -1 elsewhere */
    int * block;          /* for each '{', BLOCK_THEN / BLOCK_ELSE / BLOCK_LOOP */
    struct s_CAS_LABEL * labels;
    int label_count;
};

static char cas_error[CAS_ERROR_MAX];

/* Things CoM2 accepts that the language does not allow (missing ';' or ')', stray ')', unclosed comment / string).
   ReMoM accepts them the same way and lists them, so mod authors can fix their scripts. */
#define CAS_WARNINGS_MAX 8192
static char cas_warnings[CAS_WARNINGS_MAX];
static int cas_warning_count;

const char * CAS_Last_Error(void)
{
    return cas_error;
}

const char * CAS_Warnings(int * count)
{
    if(count != NULL) { *count = cas_warning_count; }
    return cas_warnings;
}

static void cas_warn_at(const char * script, int line, const char * what)
{
    size_t used = strlen(cas_warnings);
    cas_warning_count++;
    if(used + 200 < CAS_WARNINGS_MAX)
    {
        snprintf(cas_warnings + used, CAS_WARNINGS_MAX - used, "%s:%d: %s\n", script, line, what);
    }
}

static char * cas_strdup(const char * s, size_t n)
{
    char * r = (char *)malloc(n + 1);
    memcpy(r, s, n);
    r[n] = '\0';
    return r;
}

static void cas_upper(char * s)
{
    for(; *s; s++) { *s = (char)toupper((unsigned char)*s); }
}

void CAS_Free(CAS_Script * script)
{
    int itr;
    if(script == NULL) { return; }
    for(itr = 0; itr < script->count; itr++) { free(script->tokens[itr].text); }
    for(itr = 0; itr < script->label_count; itr++) { free(script->labels[itr].name); }
    free(script->tokens);
    free(script->match);
    free(script->block);
    free(script->labels);
    free(script->name);
    free(script);
}

static int cas_push_token(CAS_Script * s, int * cap, struct s_CAS_TOKEN tok)
{
    if(s->count == *cap)
    {
        *cap = (*cap == 0) ? 1024 : (*cap * 2);
        s->tokens = (struct s_CAS_TOKEN *)realloc(s->tokens, sizeof(*s->tokens) * (size_t)*cap);
    }
    s->tokens[s->count++] = tok;
    return 0;
}

static CAS_Script * cas_lex_fail(CAS_Script * s, int line, const char * what)
{
    snprintf(cas_error, sizeof(cas_error), "%s:%d: %s", s->name, line, what);
    CAS_Free(s);
    return NULL;
}

CAS_Script * CAS_Load_String(const char * name, const char * src)
{
    CAS_Script * s = (CAS_Script *)calloc(1, sizeof(*s));
    int cap = 0;
    int line = 1;
    int label_cap = 0;
    const char * p = src;
    int * stack;
    int depth = 0;
    int pending_block = 0;
    int itr;

    cas_error[0] = '\0';
    cas_warnings[0] = '\0';
    cas_warning_count = 0;
    s->name = cas_strdup(name, strlen(name));

    while(*p)
    {
        struct s_CAS_TOKEN tok;
        memset(&tok, 0, sizeof(tok));
        tok.line = line;

        if(*p == '\n') { line++; p++; continue; }
        if(isspace((unsigned char)*p)) { p++; continue; }

        if(*p == ':')                                                        /* comment, may span lines */
        {
            p++;
            while(*p && *p != ':') { if(*p == '\n') { line++; } p++; }
            if(*p == '\0')
            {
                cas_warn_at(s->name, tok.line, "comment never closed with ':' (runs to the end of the file)");
                continue;
            }
            p++;
            continue;
        }
        if(*p == '"')
        {
            const char * start = ++p;
            while(*p && *p != '"' && *p != '\n') { p++; }
            if(p - start > CAS_STRING_MAX) { return cas_lex_fail(s, line, "string longer than 255 characters"); }
            tok.type = TK_STR;
            tok.text = cas_strdup(start, (size_t)(p - start));
            if(*p == '"') { p++; }
            else { cas_warn_at(s->name, line, "string not closed with '\"' (ends at the end of the line)"); }
        }
        else if(*p == '!')
        {
            const char * start = ++p;
            while(*p && *p != '!' && *p != '\n') { p++; }
            if(*p != '!') { return cas_lex_fail(s, line, "label not closed with '!'"); }
            tok.type = TK_LABEL;
            tok.text = cas_strdup(start, (size_t)(p - start));
            cas_upper(tok.text);
            p++;
        }
        else if(isdigit((unsigned char)*p) || (*p == '.' && isdigit((unsigned char)p[1])))
        {
            int64_t n = 0;
            int64_t d = 1;
            while(isdigit((unsigned char)*p)) { n = n * 10 + (*p - '0'); p++; }
            if(*p == '.')
            {
                p++;
                while(isdigit((unsigned char)*p)) { if(d < 1000000000) { n = n * 10 + (*p - '0'); d *= 10; } p++; }
            }
            tok.type = TK_NUM;
            tok.num = cas_make(n, d);
        }
        else if(isalpha((unsigned char)*p) || *p == '_')
        {
            const char * start = p;
            while(isalnum((unsigned char)*p) || *p == '_') { p++; }
            if(*p == '$') { p++; }
            if(p - start > CAS_NAME_MAX) { return cas_lex_fail(s, line, "name longer than 31 characters"); }
            tok.type = TK_NAME;
            tok.text = cas_strdup(start, (size_t)(p - start));
            cas_upper(tok.text);
        }
        else if(*p == '%')
        {
            const char * start = ++p;
            char word[4] = { 0 };
            while(isalpha((unsigned char)*p) && (p - start) < 3) { word[p - start] = (char)toupper((unsigned char)*p); p++; }
            tok.type = TK_OP;
            if(strcmp(word, "AND") == 0) { tok.op = OP_AND; }
            else if(strcmp(word, "XOR") == 0) { tok.op = OP_XOR; }
            else if(strncmp(word, "OR", 2) == 0) { tok.op = OP_OR; p = start + 2; }
            else if(word[0] == 'R') { tok.op = OP_ROUND; p = start + 1; }
            else if(word[0] == 'I') { tok.op = OP_INT; p = start + 1; }
            else if(word[0] == 'F') { tok.op = OP_FRAC; p = start + 1; }
            else if(word[0] == 'N') { tok.op = OP_NOT; p = start + 1; }
            else { return cas_lex_fail(s, line, "unknown % operator"); }
        }
        else
        {
            char c = *p++;
            tok.type = TK_OP;
            switch(c)
            {
                case '+': tok.op = OP_ADD; break;
                case '-': tok.op = OP_SUB; break;
                case '*': tok.op = OP_MUL; break;
                case '/': tok.op = OP_DIV; break;
                case '=': tok.op = OP_EQ; break;
                case '<':
                    if(*p == '=') { tok.op = OP_LE; p++; }
                    else if(*p == '>') { tok.op = OP_NE; p++; }
                    else { tok.op = OP_LT; }
                    break;
                case '>':
                    if(*p == '=') { tok.op = OP_GE; p++; } else { tok.op = OP_GT; }
                    break;
                case '(': tok.type = TK_LPAREN; break;
                case ')': tok.type = TK_RPAREN; break;
                case '[': tok.type = TK_LBRACK; break;
                case ']': tok.type = TK_RBRACK; break;
                case '{': tok.type = TK_LBRACE; break;
                case '}': tok.type = TK_RBRACE; break;
                case ',': tok.type = TK_COMMA; break;
                case ';': tok.type = TK_SEMI; break;
                default:
                {
                    char what[64];
                    snprintf(what, sizeof(what), "unexpected character '%c'", c);
                    return cas_lex_fail(s, line, what);
                }
            }
        }
        cas_push_token(s, &cap, tok);
    }
    {
        struct s_CAS_TOKEN end;
        memset(&end, 0, sizeof(end));
        end.type = TK_END;
        end.line = line;
        cas_push_token(s, &cap, end);
    }

    /* Pair the braces and label each block by the keyword that opened it. */
    s->match = (int *)malloc(sizeof(int) * (size_t)s->count);
    s->block = (int *)calloc((size_t)s->count, sizeof(int));
    stack = (int *)malloc(sizeof(int) * (size_t)s->count);
    pending_block = 0;
    for(itr = 0; itr < s->count; itr++)
    {
        struct s_CAS_TOKEN * t = &s->tokens[itr];
        s->match[itr] = -1;
        if(t->type == TK_NAME)
        {
            if(strcmp(t->text, "THEN") == 0) { pending_block = BLOCK_THEN; }
            else if(strcmp(t->text, "ELSE") == 0) { pending_block = BLOCK_ELSE; }
            else if(strcmp(t->text, "LOOP") == 0) { pending_block = BLOCK_LOOP; }
        }
        else if(t->type == TK_LBRACE)
        {
            if(pending_block == 0)
            {
                free(stack);
                return cas_lex_fail(s, t->line, "'{' that does not follow THEN, ELSE or LOOP");
            }
            s->block[itr] = pending_block;
            pending_block = 0;
            stack[depth++] = itr;
        }
        else if(t->type == TK_RBRACE)
        {
            if(depth == 0)
            {
                cas_warn_at(s->name, t->line, "'}' without a matching '{' (ignored)");
                continue;
            }
            depth--;
            s->match[itr] = stack[depth];
            s->match[stack[depth]] = itr;
        }
        else if(t->type == TK_LABEL)
        {
            if(s->label_count == label_cap)
            {
                label_cap = (label_cap == 0) ? 32 : label_cap * 2;
                s->labels = (struct s_CAS_LABEL *)realloc(s->labels, sizeof(*s->labels) * (size_t)label_cap);
            }
            s->labels[s->label_count].name = cas_strdup(t->text, strlen(t->text));
            s->labels[s->label_count].target = itr + 1;
            s->label_count++;
        }
    }
    while(depth > 0)                                                   /* a block never closed runs to the end of the file */
    {
        depth--;
        cas_warn_at(s->name, s->tokens[stack[depth]].line, "'{' never closed (the block runs to the end of the file; skipping it skips the rest)");
        s->match[stack[depth]] = s->count - 1;
    }
    free(stack);
    return s;
}

CAS_Script * CAS_Load_File(const char * path)
{
    FILE * fp = fopen(path, "rb");
    long length;
    char * text;
    CAS_Script * s;

    if(fp == NULL)
    {
        snprintf(cas_error, sizeof(cas_error), "cannot open script \"%s\"", path);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    text = (char *)malloc((size_t)length + 1);
    if(fread(text, 1, (size_t)length, fp) != (size_t)length)
    {
        fclose(fp);
        free(text);
        snprintf(cas_error, sizeof(cas_error), "cannot read script \"%s\"", path);
        return NULL;
    }
    fclose(fp);
    text[length] = '\0';
    s = CAS_Load_String(path, text);
    free(text);
    return s;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Values and variables                                                                                            */
/* ---------------------------------------------------------------------------------------------------------------- */

typedef struct
{
    int is_str;
    cas_num num;
    char str[CAS_STRING_MAX + 1];
} cas_value;

struct s_CAS_VAR
{
    char name[CAS_NAME_MAX + 1];
    int used;                     /* 0 empty, 1 in use, 2 deleted */
    int is_array;
    int dims[CAS_MAX_DIMS];
    int ndims;
    cas_value * data;             /* arrays: product of (dim + 1) elements, indexes 0..dim */
    cas_value v;
};

struct s_CAS_ENV
{
    struct s_CAS_VAR * slots;
    int cap;
    int count;
};

static uint32_t cas_hash(const char * s)
{
    uint32_t h = 2166136261u;
    for(; *s; s++) { h = (h ^ (uint8_t)*s) * 16777619u; }
    return h;
}

static void cas_env_free(struct s_CAS_ENV * env)
{
    int itr;
    for(itr = 0; itr < env->cap; itr++) { free(env->slots[itr].data); }
    free(env->slots);
    memset(env, 0, sizeof(*env));
}

static struct s_CAS_VAR * cas_env_find(struct s_CAS_ENV * env, const char * name)
{
    uint32_t i;
    if(env->cap == 0) { return NULL; }
    for(i = cas_hash(name) & (uint32_t)(env->cap - 1); env->slots[i].used != 0; i = (i + 1) & (uint32_t)(env->cap - 1))
    {
        if(env->slots[i].used == 1 && strcmp(env->slots[i].name, name) == 0) { return &env->slots[i]; }
    }
    return NULL;
}

static struct s_CAS_VAR * cas_env_add(struct s_CAS_ENV * env, const char * name)
{
    uint32_t i;
    struct s_CAS_VAR * found = cas_env_find(env, name);
    if(found != NULL) { return found; }
    if((env->count + 1) * 2 > env->cap)
    {
        struct s_CAS_ENV bigger;
        int itr;
        bigger.cap = (env->cap == 0) ? 64 : env->cap * 2;
        bigger.count = 0;
        bigger.slots = (struct s_CAS_VAR *)calloc((size_t)bigger.cap, sizeof(struct s_CAS_VAR));
        for(itr = 0; itr < env->cap; itr++)
        {
            if(env->slots[itr].used == 1)
            {
                for(i = cas_hash(env->slots[itr].name) & (uint32_t)(bigger.cap - 1); bigger.slots[i].used != 0; i = (i + 1) & (uint32_t)(bigger.cap - 1)) { }
                bigger.slots[i] = env->slots[itr];
                bigger.count++;
            }
        }
        free(env->slots);
        *env = bigger;
    }
    for(i = cas_hash(name) & (uint32_t)(env->cap - 1); env->slots[i].used == 1; i = (i + 1) & (uint32_t)(env->cap - 1)) { }
    memset(&env->slots[i], 0, sizeof(env->slots[i]));
    strncpy(env->slots[i].name, name, CAS_NAME_MAX);
    env->slots[i].used = 1;
    env->slots[i].v.is_str = (name[strlen(name) - 1] == '$');
    env->slots[i].v.num = cas_int(0);
    env->count++;
    return &env->slots[i];
}

static void cas_env_remove(struct s_CAS_ENV * env, const char * name)
{
    struct s_CAS_VAR * v = cas_env_find(env, name);
    if(v != NULL)
    {
        free(v->data);
        v->data = NULL;
        v->used = 2;
        env->count--;
    }
}

static struct s_CAS_ENV cas_master;                                   /* MASTER.CAS globals */

void CAS_Clear_Master(void)
{
    cas_env_free(&cas_master);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Run state                                                                                                       */
/* ---------------------------------------------------------------------------------------------------------------- */

struct s_CAS_FOR
{
    char var[CAS_NAME_MAX + 1];
    cas_num limit;
    cas_num step;
    int resume;
};

struct s_CAS_SAVED_LOCAL
{
    char name[CAS_NAME_MAX + 1];
    int existed;
    struct s_CAS_VAR saved;
};

struct s_CAS_CALL
{
    int return_pc;
    int locals_start;
};

struct s_CAS_RUN
{
    CAS_Script * s;
    int pc;
    int exec;                     /* 1 = run, 0 = check only */
    int quiet;                    /* check mode without recording warnings (CAS_Builtins_Used) */
    int halted;
    int is_master;
    long steps;
    struct s_CAS_ENV vars;
    struct s_CAS_FOR fors[CAS_MAX_FOR];
    int nfor;
    struct s_CAS_CALL calls[CAS_MAX_CALL];
    int ncall;
    struct s_CAS_SAVED_LOCAL * locals;
    int nlocal;
    int local_cap;
    cas_num res_sum;              /* ADDRESLINE total, read by SUMRESLINES() */
    int res_lines;                /* ADDRESLINE calls */
    int * builtin_uses;           /* check mode: uses per builtin */
    jmp_buf fail;
};

static struct s_CAS_TOKEN * cur(struct s_CAS_RUN * r)
{
    if(r->pc >= r->s->count) { r->pc = r->s->count - 1; }             /* skipping a block left open runs onto the end */
    return &r->s->tokens[r->pc];
}

static void cas_fail(struct s_CAS_RUN * r, const char * fmt, ...)
{
    char what[CAS_ERROR_MAX];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(what, sizeof(what), fmt, ap);
    va_end(ap);
    snprintf(cas_error, sizeof(cas_error), "%s:%d: %s", r->s->name, cur(r)->line, what);
    longjmp(r->fail, 1);
}

static int is_name(struct s_CAS_TOKEN * t, const char * word) { return t->type == TK_NAME && strcmp(t->text, word) == 0; }

/* Warnings are recorded by CAS_Check (each statement is visited once); a run tolerates the same things silently. */
static void cas_warn(struct s_CAS_RUN * r, const char * what)
{
    if(!r->exec && !r->quiet) { cas_warn_at(r->s->name, cur(r)->line, what); }
}

/* 1 if the current token starts on a later line than the one before it. */
static int new_line(struct s_CAS_RUN * r)
{
    return (r->pc > 0) && (cur(r)->line > r->s->tokens[r->pc - 1].line);
}

/* 1 if the current token ends an expression for sure: ';', THEN, braces, end of file, or a new line. */
static int expression_ends(struct s_CAS_RUN * r)
{
    int type = cur(r)->type;
    return type == TK_SEMI || type == TK_LBRACE || type == TK_RBRACE || type == TK_END || is_name(cur(r), "THEN") || new_line(r);
}

/* ')' expected: also accepted missing when the expression clearly ends. */
static void close_paren(struct s_CAS_RUN * r)
{
    if(cur(r)->type == TK_RPAREN) { r->pc++; return; }
    if(expression_ends(r)) { cas_warn(r, "missing ')'"); return; }
    cas_fail(r, "expected ')'");
}

/* Skips stray ')' left over before THEN / ';'. */
static void skip_stray_parens(struct s_CAS_RUN * r)
{
    while(cur(r)->type == TK_RPAREN) { cas_warn(r, "extra ')'"); r->pc++; }
}

static void expect(struct s_CAS_RUN * r, int type, const char * what)
{
    if(cur(r)->type != type) { cas_fail(r, "expected %s", what); }
    r->pc++;
}

static void expect_word(struct s_CAS_RUN * r, const char * word)
{
    skip_stray_parens(r);
    if(!is_name(cur(r), word)) { cas_fail(r, "expected %s", word); }
    r->pc++;
}

/* End of a simple statement: ';', or nothing before '}' / end of file. */
static void end_statement(struct s_CAS_RUN * r)
{
    skip_stray_parens(r);
    if(cur(r)->type == TK_SEMI) { r->pc++; return; }
    if(cur(r)->type == TK_RBRACE || cur(r)->type == TK_END) { return; }
    if(new_line(r)) { cas_warn(r, "missing ';' at the end of the line"); return; }
    cas_fail(r, "expected ';'");
}

static struct s_CAS_VAR * var_lookup(struct s_CAS_RUN * r, const char * name)
{
    struct s_CAS_VAR * v = cas_env_find(&r->vars, name);
    if(v == NULL && !r->is_master) { v = cas_env_find(&cas_master, name); }
    return v;
}

static double num_value(struct s_CAS_RUN * r, const cas_value * v)
{
    if(v->is_str) { cas_fail(r, "expected a number, got the string \"%s\"", v->str); }
    return cas_double(v->num);
}

static int64_t int_value(struct s_CAS_RUN * r, const cas_value * v)
{
    if(v->is_str) { cas_fail(r, "expected a number, got the string \"%s\"", v->str); }
    return cas_trunc(v->num);
}

static cas_value make_num(cas_num n) { cas_value v; v.is_str = 0; v.num = n; v.str[0] = '\0'; return v; }
static cas_value make_int(int64_t n) { return make_num(cas_int(n)); }

static cas_value make_str(const char * s)
{
    cas_value v;
    v.is_str = 1;
    v.num = cas_int(0);
    strncpy(v.str, s, CAS_STRING_MAX);
    v.str[CAS_STRING_MAX] = '\0';
    return v;
}

static int truthy(struct s_CAS_RUN * r, const cas_value * v)
{
    if(v->is_str) { cas_fail(r, "a string is not a condition"); }
    return v->num.n != 0;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Built-ins                                                                                                       */
/* ---------------------------------------------------------------------------------------------------------------- */

typedef cas_value (*cas_builtin_fn)(struct s_CAS_RUN * r, cas_value * args, int nargs);

enum e_CAS_BUILTIN_KIND { BI_FUNC = 1, BI_CMD = 2 };

struct s_CAS_BUILTIN
{
    const char * name;
    int kind;
    int min_args;
    int max_args;
    cas_builtin_fn fn;            /* language built-in */
    CAS_Game_Fn game;             /* game built-in (CAS_Game.c); both NULL: known, not implemented in ReMoM yet */
};

static cas_value bi_rnd(struct s_CAS_RUN * r, cas_value * a, int n)
{
    int64_t x = int_value(r, &a[0]);
    if(x < 1 || x > 32767) { cas_fail(r, "RND(%lld): argument must be 1..32767", (long long)x); }
    return make_int(Random((int16_t)x) - 1);                               /* 0 .. X-1, from the game's RNG */
}

static cas_value bi_sgn(struct s_CAS_RUN * r, cas_value * a, int n) { double x = num_value(r, &a[0]); return make_int((x > 0) - (x < 0)); }
static cas_value bi_abs(struct s_CAS_RUN * r, cas_value * a, int n) { cas_num x = a[0].num; num_value(r, &a[0]); if(x.n < 0) { x.n = -x.n; } return make_num(x); }

static cas_value bi_expon(struct s_CAS_RUN * r, cas_value * a, int n)
{
    cas_num x = a[0].num;
    cas_num result = cas_int(1);
    int64_t i = int_value(r, &a[1]);
    num_value(r, &a[0]);
    if(i < 0 || i > 64) { cas_fail(r, "EXPON: exponent must be 0..64"); }
    while(i-- > 0) { result = cas_mul(result, x); }
    return make_num(result);
}

static cas_value bi_strs(struct s_CAS_RUN * r, cas_value * a, int n)
{
    char text[64];
    num_value(r, &a[0]);
    if(a[0].num.d == 1) { snprintf(text, sizeof(text), "%lld", (long long)a[0].num.n); }
    else { snprintf(text, sizeof(text), "%g", cas_double(a[0].num)); }
    return make_str(text);
}

static cas_value bi_val(struct s_CAS_RUN * r, cas_value * a, int n)
{
    if(!a[0].is_str) { cas_fail(r, "VAL needs a string"); }
    return make_num(cas_from_double(atof(a[0].str)));
}

static cas_value bi_sqrt(struct s_CAS_RUN * r, cas_value * a, int n)
{
    int64_t x = int_value(r, &a[0]);
    if(x < 0) { cas_fail(r, "SQRT of a negative number"); }
    return make_int((int64_t)floor(sqrt((double)x)));
}


static cas_value bi_sumreslines(struct s_CAS_RUN * r, cas_value * a, int n) { return make_num(r->res_sum); }

static cas_value bi_addresline(struct s_CAS_RUN * r, cas_value * a, int n)
{
    if(!a[0].is_str) { cas_fail(r, "ADDRESLINE: the first argument is the line's text"); }
    num_value(r, &a[1]);
    r->res_sum = cas_add(r->res_sum, a[1].num);
    r->res_lines++;
    return make_int(0);
}

static cas_value bi_debuglog(struct s_CAS_RUN * r, cas_value * a, int n)
{
    if(a[0].is_str) { LOG_DEBUG(LOG_CAT_MOD, "[CAS] %s: %s", r->s->name, a[0].str); }
    else { LOG_DEBUG(LOG_CAT_MOD, "[CAS] %s: %g", r->s->name, cas_double(a[0].num)); }
    return make_int(0);
}

#define F(name, lo, hi, fn) { name, BI_FUNC, lo, hi, fn, NULL }
#define C(name, lo, hi, fn) { name, BI_CMD, lo, hi, fn, NULL }
#define G(name, lo, hi, fn) { name, BI_FUNC, lo, hi, NULL, fn }
#define GC(name, lo, hi, fn) { name, BI_CMD, lo, hi, NULL, fn }

/* Every built-in Scripts.TXT documents, plus the ones Warlord uses that it does not (GETCITYDATA, GETWIZARDDATA,
   PRIMARYREALM).  Checked against the keyword table in Caster.exe 1.05.11, which also spells GETNODEDATA as NODEDATA.
   fn and game both NULL: not implemented in ReMoM yet. */
static const struct s_CAS_BUILTIN cas_builtins[] =
{
    F("RND", 1, 1, bi_rnd), F("SGN", 1, 1, bi_sgn), F("ABS", 1, 1, bi_abs), F("EXPON", 2, 2, bi_expon),
    F("STRS", 1, 1, bi_strs), F("VAL", 1, 1, bi_val), F("SQRT", 1, 1, bi_sqrt),
    F("SUMRESLINES", 0, 0, bi_sumreslines),
    G("DIFFICULTY", 0, 0, CAS_Game_Difficulty), G("LANDSIZE", 0, 0, CAS_Game_Landsize), G("NOFWIZARDS", 0, 0, CAS_Game_Nofwizards),
    G("MAXCITY", 0, 0, CAS_Game_Maxcity), G("CITYOWNER", 1, 1, CAS_Game_Cityowner), G("CITYRACE", 1, 1, CAS_Game_Cityrace),
    G("ISBUILT", 2, 2, CAS_Game_Isbuilt), G("GOLD", 1, 1, CAS_Game_Gold), G("MANA", 1, 1, CAS_Game_Mana),
    C("ADDRESLINE", 2, 2, bi_addresline), C("DEBUGLOG", 1, 1, bi_debuglog),

    F("IREADS", 3, 3, NULL), F("IREADN", 3, 3, NULL), G("BOOKS", 2, 2, CAS_Game_Books), F("WASSSPICKED", 2, 2, NULL),
    F("PERSONALITY", 1, 1, NULL), F("OBJECTIVE", 1, 1, NULL), G("RETORT", 2, 2, CAS_Game_Retort), F("MINERALS", 0, 0, NULL),
    F("CLIMATE", 0, 0, NULL), F("HASSPELLCAT", 2, 2, NULL), G("HOMERACE", 1, 1, CAS_Game_Homerace), G("SPELLSTATE", 2, 2, CAS_Game_Spellstate),
    F("OVERLANDTURN", 0, 0, NULL), F("BASETURN", 0, 0, NULL), G("CONTACT", 2, 2, CAS_Game_Contact), F("HOSTILITY", 2, 2, NULL),
    G("RELATION", 2, 2, CAS_Game_Relation), F("HREL", 2, 2, NULL), G("TREATY", 2, 2, CAS_Game_Treaty), G("SCORINGOPTION", 1, 1, CAS_Game_Scoringoption),
    F("OWNEDLIGHTNINGRESIST", 1, 1, NULL), F("OWNEDFIREIMMUNITY", 1, 1, NULL), G("DEFEATED", 1, 1, CAS_Game_Defeated),
    G("BANISHED", 1, 1, CAS_Game_Banished), G("HASGLOBAL", 2, 2, CAS_Game_Hasglobal), F("CHAOSSURGEVALUE", 1, 1, NULL),
    F("AISTRATEGY", 2, 2, NULL), F("SELECTBASEDON", 2, 2, NULL), G("GETSTAT", 3, 4, CAS_Game_Getstat), F("ISHERO", 1, 1, NULL),
    F("FANTASTIC", 1, 1, NULL), F("BASEFANTASTIC", 1, 1, NULL), F("GETSP", 1, 1, NULL),
    G("BUILDINGSOWNED", 3, 3, CAS_Game_Buildingsowned), G("CITYENCHANT", 2, 2, CAS_Game_Cityenchant), G("POPULATION", 1, 1, CAS_Game_Population),
    G("POPUNITS", 1, 1, CAS_Game_Popunits), G("OREINRANGE", 2, 2, CAS_Game_Oreinrange), G("TERRAININRANGE", 2, 2, CAS_Game_Terraininrange), F("GARRISON", 1, 1, NULL),
    G("ISFORTRESS", 1, 1, CAS_Game_Isfortress), G("EVENTTURN", 1, 1, CAS_Game_Eventturn), G("EVENTVAL", 1, 1, CAS_Game_Eventval), G("RESEARCHING", 1, 1, CAS_Game_Researching),
    F("CASTING", 1, 1, NULL), F("RANDOMARCANUSRACE", 1, 1, NULL), F("RANDOMMYRRANRACE", 1, 1, NULL),
    F("GETCOMBATENCHANTMENTFLAG", 3, 3, NULL), F("GETOLENCHANTMENTFLAG", 3, 3, NULL),
    F("GETENCHANTMENTFLAG", 3, 3, NULL), F("GETITEMPOWER", 2, 2, NULL), F("HASCOMBATGLOBAL", 3, 3, NULL),
    F("ISCOMBAT", 0, 0, NULL), F("GETTERRAIN", 3, 3, NULL), G("CITYONTILE", 1, 3, CAS_Game_Cityontile), F("NODEONTILE", 1, 3, NULL),
    G("PLANESIZEX", 1, 1, CAS_Game_Planesizex), G("PLANESIZEY", 1, 1, CAS_Game_Planesizey), F("ALIVE", 1, 1, NULL), F("MAXUNIT", 0, 0, NULL),
    F("ATTACKER", 0, 0, NULL), F("DEFENDER", 0, 0, NULL), F("HASTREE", 1, 1, NULL), F("INCOMBAT", 1, 1, NULL),
    F("COMBATP", 0, 0, NULL), F("COMBATX", 0, 0, NULL), F("COMBATY", 0, 0, NULL), F("GETDAMAGE", 2, 2, NULL),
    F("GETHEAB", 2, 2, NULL), F("GETDIPVAR", 3, 3, NULL), F("PORTRAIT", 1, 1, NULL), F("WIZNAME", 1, 1, NULL),
    G("NOFUNITSONTILE", 3, 3, CAS_Game_Nofunitsontile), G("UNITONTILE", 4, 4, CAS_Game_Unitontile), G("GETCITYDATA", 2, 2, CAS_Game_Getcitydata),
    G("GETWIZARDDATA", 2, 2, CAS_Game_Getwizarddata), F("PRIMARYREALM", 1, 1, NULL),

    C("IWRITE", 4, 4, NULL), C("ADDAISPELL", 2, 2, NULL), C("SETPERSONALITY", 2, 2, NULL), C("SETOBJECTIVE", 2, 2, NULL),
    C("DEALCOMBATDAMAGE", 4, 4, NULL), C("SETCOMBATENCHANTMENTFLAG", 4, 4, NULL),
    C("SETOLENCHANTMENTFLAG", 4, 4, NULL), C("SETENCHANTMENTFLAG", 4, 4, NULL), C("SETRETORT", 3, 3, NULL),
    C("SETBOOK", 3, 3, NULL), C("SETSTAT", 4, 5, NULL), C("APPLYCHAOSCHANNEL", 1, 1, NULL),
    GC("SETCITYDATA", 3, 3, CAS_Game_Setcitydata), GC("SETWIZARDDATA", 3, 3, CAS_Game_Setwizarddata), C("ADDABILITYLINE", 4, 4, NULL),
    C("SETTERRAIN", 4, 4, NULL), C("SETHEAB", 4, 4, NULL), C("GAINEXP", 2, 2, NULL), C("SETBUILDINGSTATE", 3, 3, NULL),
    C("SETCITYENCHANTOWNER", 3, 3, NULL), C("SETCOMBATGLOBAL", 4, 4, NULL), C("SETGLOBAL", 3, 3, NULL),
    C("OVERLANDHEAL", 2, 2, NULL), C("COMBATHEAL", 4, 4, NULL), C("GETNODEDATA", 2, 5, NULL), C("NODEDATA", 2, 5, NULL), C("SETDIPVAR", 4, 4, NULL),
    C("SETSPELLDATA", 3, 3, NULL),
};

#undef F
#undef C
#undef G
#undef GC

#define CAS_BUILTIN_COUNT ((int)(sizeof(cas_builtins) / sizeof(cas_builtins[0])))

static int builtin_index(const char * name)
{
    int itr;
    for(itr = 0; itr < CAS_BUILTIN_COUNT; itr++)
    {
        if(strcmp(cas_builtins[itr].name, name) == 0) { return itr; }
    }
    return -1;
}

int CAS_Builtin_Implemented(const char * name)
{
    char upper[CAS_NAME_MAX + 1];
    int idx;
    strncpy(upper, name, CAS_NAME_MAX);
    upper[CAS_NAME_MAX] = '\0';
    cas_upper(upper);
    idx = builtin_index(upper);
    if(idx < 0) { return -1; }
    return (cas_builtins[idx].fn != NULL) || (cas_builtins[idx].game != NULL);
}

/* Calls built-in `idx` with arguments already parsed, or records the use (check mode). */
static cas_value call_builtin(struct s_CAS_RUN * r, int idx, cas_value * args, int nargs)
{
    const struct s_CAS_BUILTIN * b = &cas_builtins[idx];
    if(nargs < b->min_args || nargs > b->max_args)
    {
        cas_fail(r, "%s takes %d..%d arguments, got %d", b->name, b->min_args, b->max_args, nargs);
    }
    if(!r->exec)
    {
        if(r->builtin_uses != NULL) { r->builtin_uses[idx]++; }
        return make_int(0);
    }
    if(b->game != NULL)
    {
        int32_t ints[CAS_MAX_ARGS];
        int32_t result = 0;
        char error[200];
        int itr;
        for(itr = 0; itr < nargs; itr++)
        {
            int64_t v = int_value(r, &args[itr]);
            if(v < INT32_MIN || v > INT32_MAX) { cas_fail(r, "%s: argument %d (%lld) is outside 32-bit range", b->name, itr + 1, (long long)v); }
            ints[itr] = (int32_t)v;
        }
        error[0] = '\0';
        if(b->game(ints, nargs, &result, error, sizeof(error)) != 0) { cas_fail(r, "%s: %s", b->name, error); }
        return make_int(result);
    }
    if(b->fn == NULL) { cas_fail(r, "%s is not implemented in ReMoM yet", b->name); }
    return b->fn(r, args, nargs);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Expressions: right-recursive, no precedence                                                                     */
/* ---------------------------------------------------------------------------------------------------------------- */

static cas_value expr(struct s_CAS_RUN * r);

static int parse_args(struct s_CAS_RUN * r, cas_value * args, int close_type)
{
    int n = 0;
    if(cur(r)->type == close_type) { r->pc++; return 0; }
    for(;;)
    {
        if(n == CAS_MAX_ARGS) { cas_fail(r, "too many arguments"); }
        args[n++] = expr(r);
        if(cur(r)->type == TK_COMMA) { r->pc++; continue; }
        if(close_type == TK_RPAREN) { close_paren(r); }
        else { expect(r, close_type, "']'"); }
        return n;
    }
}

/* Element of array variable v at the given indexes (1-based in scripts; 0 is accepted). */
static cas_value * array_element(struct s_CAS_RUN * r, struct s_CAS_VAR * v, cas_value * idx, int n)
{
    int itr;
    long offset = 0;
    if(!v->is_array) { cas_fail(r, "%s is not an array", v->name); }
    if(n != v->ndims) { cas_fail(r, "%s has %d dimensions, indexed with %d", v->name, v->ndims, n); }
    for(itr = 0; itr < n; itr++)
    {
        int64_t i = int_value(r, &idx[itr]);
        if(i < 0 || i > v->dims[itr]) { cas_fail(r, "%s index %lld outside 1..%d", v->name, (long long)i, v->dims[itr]); }
        offset = offset * (v->dims[itr] + 1) + (long)i;
    }
    return &v->data[offset];
}

static cas_value primary(struct s_CAS_RUN * r)
{
    struct s_CAS_TOKEN * t = cur(r);

    if(t->type == TK_NUM) { r->pc++; return make_num(t->num); }
    if(t->type == TK_STR) { r->pc++; return make_str(t->text); }
    if(t->type == TK_LPAREN)
    {
        cas_value v;
        r->pc++;
        v = expr(r);
        close_paren(r);
        return v;
    }
    if(t->type == TK_OP && (t->op == OP_SUB || t->op == OP_ADD))       /* sign of a constant or term: -1, -X, -(..) */
    {
        cas_value v;
        int negate = (t->op == OP_SUB);
        r->pc++;
        v = primary(r);
        if(r->exec && negate) { num_value(r, &v); v.num.n = -v.num.n; }
        return v;
    }
    if(t->type == TK_OP && t->op >= OP_ROUND)                          /* prefix operators apply to the next operand */
    {
        int op = t->op;
        cas_value v;
        r->pc++;
        v = primary(r);
        if(!r->exec) { return v; }
        num_value(r, &v);
        switch(op)
        {
            case OP_ROUND: { double x = cas_double(v.num); return make_int((int64_t)((x < 0) ? -floor(-x + 0.5) : floor(x + 0.5))); }
            case OP_INT:   return make_int(cas_trunc(v.num));
            case OP_FRAC:  return make_num(cas_sub(v.num, cas_int(cas_trunc(v.num))));
            default:       return make_int(v.num.n == 0);
        }
    }
    if(t->type == TK_NAME)
    {
        int idx = builtin_index(t->text);
        r->pc++;
        if(idx >= 0 && cas_builtins[idx].kind == BI_FUNC)
        {
            cas_value args[CAS_MAX_ARGS];
            int n = 0;
            if(cur(r)->type == TK_LPAREN) { r->pc++; n = parse_args(r, args, TK_RPAREN); }
            return call_builtin(r, idx, args, n);
        }
        if(cur(r)->type == TK_LBRACK)
        {
            cas_value idxv[CAS_MAX_DIMS + CAS_MAX_ARGS];
            int n;
            struct s_CAS_VAR * v;
            r->pc++;
            n = parse_args(r, idxv, TK_RBRACK);
            if(!r->exec) { return make_int(0); }
            v = var_lookup(r, t->text);
            if(v == NULL) { cas_fail(r, "array %s was never declared with DIMNUM / DIMSTR", t->text); }
            return *array_element(r, v, idxv, n);
        }
        if(!r->exec) { return make_int(0); }
        {
            struct s_CAS_VAR * v = var_lookup(r, t->text);
            if(v == NULL)
            {
                return (t->text[strlen(t->text) - 1] == '$') ? make_str("") : make_int(0);   /* never set: 0 / "" */
            }
            if(v->is_array) { cas_fail(r, "%s is an array; index it", t->text); }
            return v->v;
        }
    }
    cas_fail(r, "expected a value");
    return make_int(0);
}

static int is_binary_op(struct s_CAS_TOKEN * t) { return t->type == TK_OP && t->op >= OP_ADD && t->op <= OP_XOR; }

static cas_value apply(struct s_CAS_RUN * r, int op, cas_value a, cas_value b)
{
    if(a.is_str || b.is_str)
    {
        if(op == OP_ADD && a.is_str && b.is_str)
        {
            cas_value v = make_str(a.str);
            strncat(v.str, b.str, CAS_STRING_MAX - strlen(v.str));
            return v;
        }
        if(a.is_str && b.is_str && (op == OP_EQ || op == OP_NE))
        {
            int same = strcmp(a.str, b.str) == 0;
            return make_int((op == OP_EQ) ? same : !same);
        }
        cas_fail(r, "this operator does not take strings");
    }
    switch(op)
    {
        case OP_ADD: return make_num(cas_add(a.num, b.num));
        case OP_SUB: return make_num(cas_sub(a.num, b.num));
        case OP_MUL: return make_num(cas_mul(a.num, b.num));
        case OP_DIV:
            if(b.num.n == 0) { cas_fail(r, "division by zero"); }
            return make_num(cas_mul(a.num, cas_make(b.num.d, b.num.n)));
        case OP_EQ:  return make_int(cas_cmp(a.num, b.num) == 0);
        case OP_NE:  return make_int(cas_cmp(a.num, b.num) != 0);
        case OP_LT:  return make_int(cas_cmp(a.num, b.num) < 0);
        case OP_GT:  return make_int(cas_cmp(a.num, b.num) > 0);
        case OP_LE:  return make_int(cas_cmp(a.num, b.num) <= 0);
        case OP_GE:  return make_int(cas_cmp(a.num, b.num) >= 0);
        case OP_AND: return make_int((a.num.n != 0) && (b.num.n != 0));
        case OP_OR:  return make_int((a.num.n != 0) || (b.num.n != 0));
        default:     return make_int((a.num.n != 0) != (b.num.n != 0));
    }
}

static cas_value expr(struct s_CAS_RUN * r)
{
    cas_value left = primary(r);
    if(is_binary_op(cur(r)))
    {
        int op = cur(r)->op;
        cas_value right;
        r->pc++;
        right = expr(r);                                               /* a op (rest): right-recursive */
        if(!r->exec) { return left; }
        return apply(r, op, left, right);
    }
    return left;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/*  Statements                                                                                                      */
/* ---------------------------------------------------------------------------------------------------------------- */

static int parens_enclose_statement(struct s_CAS_RUN * r);

static int find_label(struct s_CAS_RUN * r, const char * name)
{
    char upper[CAS_STRING_MAX + 1];
    int itr;
    strncpy(upper, name, CAS_STRING_MAX);
    upper[CAS_STRING_MAX] = '\0';
    cas_upper(upper);
    for(itr = 0; itr < r->s->label_count; itr++)
    {
        if(strcmp(r->s->labels[itr].name, upper) == 0) { return r->s->labels[itr].target; }
    }
    cas_fail(r, "no label !%s! in this script", upper);
    return -1;
}

static void assign(struct s_CAS_RUN * r, struct s_CAS_ENV * env, const char * name, cas_value * idx, int nidx, cas_value value)
{
    struct s_CAS_VAR * v;
    int wants_str = (name[strlen(name) - 1] == '$');
    if(value.is_str != wants_str)
    {
        cas_fail(r, wants_str ? "%s holds strings; got a number" : "%s holds numbers; got a string", name);
    }
    if(nidx > 0)
    {
        v = cas_env_find(env, name);
        if(v == NULL) { cas_fail(r, "array %s was never declared with DIMNUM / DIMSTR in this script", name); }
        *array_element(r, v, idx, nidx) = value;
        return;
    }
    v = cas_env_add(env, name);
    if(v->is_array) { cas_fail(r, "%s is an array; index it", name); }
    v->v = value;
}

static void declare_local(struct s_CAS_RUN * r, const char * name)
{
    struct s_CAS_VAR * existing = cas_env_find(&r->vars, name);
    struct s_CAS_SAVED_LOCAL * saved;
    if(r->ncall == 0) { return; }                                      /* outside a subroutine: an ordinary variable */
    if(r->nlocal == r->local_cap)
    {
        r->local_cap = (r->local_cap == 0) ? 16 : r->local_cap * 2;
        r->locals = (struct s_CAS_SAVED_LOCAL *)realloc(r->locals, sizeof(*r->locals) * (size_t)r->local_cap);
    }
    saved = &r->locals[r->nlocal++];
    strncpy(saved->name, name, CAS_NAME_MAX);
    saved->name[CAS_NAME_MAX] = '\0';
    saved->existed = (existing != NULL);
    if(existing != NULL)
    {
        saved->saved = *existing;
        if(existing->is_array)                                         /* keep the caller's array data */
        {
            existing->data = NULL;
            existing->is_array = 0;
        }
        cas_env_remove(&r->vars, name);
    }
    cas_env_add(&r->vars, name);
}

static void restore_locals(struct s_CAS_RUN * r, int start)
{
    while(r->nlocal > start)
    {
        struct s_CAS_SAVED_LOCAL * saved = &r->locals[--r->nlocal];
        cas_env_remove(&r->vars, saved->name);
        if(saved->existed)
        {
            struct s_CAS_VAR * v = cas_env_add(&r->vars, saved->name);
            *v = saved->saved;
            v->used = 1;
        }
    }
}

static void stmt_dim(struct s_CAS_RUN * r, int is_str)
{
    for(;;)
    {
        struct s_CAS_TOKEN * name = cur(r);
        cas_value dims[CAS_MAX_DIMS + CAS_MAX_ARGS];
        int n;
        expect(r, TK_NAME, "an array name");
        expect(r, TK_LBRACK, "'['");
        n = parse_args(r, dims, TK_RBRACK);
        if(n < 1 || n > CAS_MAX_DIMS) { cas_fail(r, "arrays have 1..%d dimensions", CAS_MAX_DIMS); }
        if(r->exec)
        {
            struct s_CAS_VAR * v;
            long total = 1;
            int itr;
            if((name->text[strlen(name->text) - 1] == '$') != is_str) { cas_fail(r, "%s: string arrays end in $, number arrays do not", name->text); }
            v = cas_env_add(&r->vars, name->text);
            free(v->data);
            v->is_array = 1;
            v->ndims = n;
            for(itr = 0; itr < n; itr++)
            {
                int64_t d = int_value(r, &dims[itr]);
                if(d < 1 || d > 100000) { cas_fail(r, "%s: dimension %lld outside 1..100000", name->text, (long long)d); }
                v->dims[itr] = (int)d;
                total *= (d + 1);
            }
            if(total > 1000000) { cas_fail(r, "%s: array too large", name->text); }
            v->data = (cas_value *)calloc((size_t)total, sizeof(cas_value));
            for(itr = 0; itr < total; itr++) { v->data[itr].is_str = is_str; v->data[itr].num = cas_int(0); }
        }
        if(cur(r)->type == TK_COMMA) { r->pc++; continue; }
        break;
    }
    end_statement(r);
}

/* Skips from just after a LOOP's '{' .. '}' ENDLP [cond] ';' (pc on the '}'). */
static void skip_endlp(struct s_CAS_RUN * r)
{
    int exec = r->exec;
    r->pc++;
    expect_word(r, "ENDLP");
    r->exec = 0;
    if(cur(r)->type != TK_SEMI && cur(r)->type != TK_RBRACE && cur(r)->type != TK_END) { expr(r); }
    r->exec = exec;
    end_statement(r);
}

static void close_block(struct s_CAS_RUN * r)
{
    int close = r->pc;
    int open = r->s->match[close];
    int kind;
    r->pc++;
    if(open < 0) { return; }                                           /* unmatched '}' (warned at load): ignored */
    kind = r->s->block[open];
    if(kind == BLOCK_THEN)
    {
        if(is_name(cur(r), "ELSE"))
        {
            r->pc++;
            if(cur(r)->type != TK_LBRACE) { cas_fail(r, "expected '{' after ELSE"); }
            if(r->exec) { r->pc = r->s->match[r->pc] + 1; }             /* the THEN branch ran: skip ELSE */
            else { r->pc++; }
        }
        else if(cur(r)->type == TK_SEMI) { r->pc++; }
        return;
    }
    if(kind == BLOCK_ELSE)
    {
        if(cur(r)->type == TK_SEMI) { r->pc++; }
        return;
    }
    /* LOOP block: '}' ENDLP [cond] ; -- leave when cond is true, else go round again */
    expect_word(r, "ENDLP");
    if(cur(r)->type != TK_SEMI && cur(r)->type != TK_RBRACE && cur(r)->type != TK_END)
    {
        cas_value c = expr(r);
        if(!r->exec) { end_statement(r); return; }
        if(truthy(r, &c)) { end_statement(r); return; }
    }
    else if(!r->exec) { end_statement(r); return; }
    r->pc = open + 1;
}

static void statement(struct s_CAS_RUN * r)
{
    struct s_CAS_TOKEN * t = cur(r);

    if(++r->steps > CAS_MAX_STEPS) { cas_fail(r, "more than %ld statements: runaway loop?", (long)CAS_MAX_STEPS); }

    switch(t->type)
    {
        case TK_SEMI:
        case TK_LABEL:
            r->pc++;
            return;
        case TK_RBRACE:
            close_block(r);
            return;
        case TK_NAME:
            break;
        case TK_END:
        case TK_LBRACE:
            cas_fail(r, "expected a statement");
        default:                                                       /* junk (e.g. the tail of a comment that ended early): skipped to ';' or the end of the line */
            cas_warn(r, "not a statement (skipped up to ';' or the end of the line)");
            r->pc++;
            while(cur(r)->type != TK_SEMI && cur(r)->type != TK_LBRACE && cur(r)->type != TK_RBRACE && cur(r)->type != TK_END && !new_line(r)) { r->pc++; }
            if(cur(r)->type == TK_SEMI) { r->pc++; }
            return;
    }

    if(is_name(t, "IF"))
    {
        cas_value c;
        int open;
        r->pc++;
        c = expr(r);
        expect_word(r, "THEN");
        if(cur(r)->type != TK_LBRACE) { cas_fail(r, "expected '{' after THEN"); }
        open = r->pc;
        if(!r->exec || truthy(r, &c)) { r->pc = open + 1; return; }
        r->pc = r->s->match[open] + 1;                                 /* condition false: skip THEN, enter ELSE */
        if(is_name(cur(r), "ELSE"))
        {
            r->pc++;
            if(cur(r)->type != TK_LBRACE) { cas_fail(r, "expected '{' after ELSE"); }
            r->pc++;
        }
        else if(cur(r)->type == TK_SEMI) { r->pc++; }
        return;
    }
    if(is_name(t, "LOOP"))
    {
        int open;
        cas_value c = make_int(1);
        r->pc++;
        if(cur(r)->type != TK_LBRACE) { c = expr(r); }
        if(cur(r)->type != TK_LBRACE) { cas_fail(r, "expected '{' after LOOP"); }
        open = r->pc;
        if(!r->exec || truthy(r, &c)) { r->pc = open + 1; return; }
        r->pc = r->s->match[open];
        skip_endlp(r);
        return;
    }
    if(is_name(t, "FOR"))
    {
        struct s_CAS_TOKEN * name;
        cas_value from, to, step = make_int(1);
        r->pc++;
        name = cur(r);
        expect(r, TK_NAME, "the FOR variable");
        if(cur(r)->type != TK_OP || cur(r)->op != OP_EQ) { cas_fail(r, "expected '='"); }
        r->pc++;
        from = expr(r);
        expect_word(r, "TO");
        to = expr(r);
        if(is_name(cur(r), "STEP")) { r->pc++; step = expr(r); }
        end_statement(r);
        if(r->exec)
        {
            int itr;
            num_value(r, &from); num_value(r, &to); num_value(r, &step);
            assign(r, &r->vars, name->text, NULL, 0, from);
            for(itr = 0; itr < r->nfor; itr++)                         /* re-entering a loop on the same variable */
            {
                if(strcmp(r->fors[itr].var, name->text) == 0) { r->nfor = itr; break; }
            }
            if(r->nfor == CAS_MAX_FOR) { cas_fail(r, "FOR loops nested more than %d deep", CAS_MAX_FOR); }
            strncpy(r->fors[r->nfor].var, name->text, CAS_NAME_MAX);
            r->fors[r->nfor].var[CAS_NAME_MAX] = '\0';
            r->fors[r->nfor].limit = to.num;
            r->fors[r->nfor].step = step.num;
            r->fors[r->nfor].resume = r->pc;
            r->nfor++;
        }
        return;
    }
    if(is_name(t, "NEXT"))
    {
        const char * name = NULL;
        r->pc++;
        if(cur(r)->type == TK_NAME) { name = cur(r)->text; r->pc++; }
        end_statement(r);
        if(r->exec)
        {
            int at = r->nfor - 1;
            struct s_CAS_VAR * v;
            cas_num value;
            if(name != NULL)
            {
                while(at >= 0 && strcmp(r->fors[at].var, name) != 0) { at--; }
                if(at < 0) { cas_fail(r, "NEXT %s without a FOR %s", name, name); }
            }
            if(at < 0) { cas_fail(r, "NEXT without a FOR"); }
            r->nfor = at + 1;                                          /* cancels loops started after it */
            v = cas_env_add(&r->vars, r->fors[at].var);
            value = cas_add(v->v.num, r->fors[at].step);
            v->v = make_num(value);
            if((r->fors[at].step.n >= 0) ? (cas_cmp(value, r->fors[at].limit) <= 0) : (cas_cmp(value, r->fors[at].limit) >= 0))
            {
                r->pc = r->fors[at].resume;
            }
            else
            {
                r->nfor = at;
            }
        }
        return;
    }
    if(is_name(t, "GOTO") || is_name(t, "CALL"))
    {
        int is_call = is_name(t, "CALL");
        int start;
        cas_value label;
        r->pc++;
        start = r->pc;
        label = expr(r);
        if(!r->exec && r->pc == start + 1 && r->s->tokens[start].type == TK_STR)
        {
            find_label(r, r->s->tokens[start].text);                   /* a constant label must exist */
        }
        end_statement(r);
        if(!r->exec) { return; }
        if(!label.is_str) { cas_fail(r, "%s needs a label name string", is_call ? "CALL" : "GOTO"); }
        if(is_call)
        {
            if(r->ncall == CAS_MAX_CALL) { cas_fail(r, "CALL nested more than %d deep", CAS_MAX_CALL); }
            r->calls[r->ncall].return_pc = r->pc;
            r->calls[r->ncall].locals_start = r->nlocal;
            r->ncall++;
        }
        r->pc = find_label(r, label.str);
        return;
    }
    if(is_name(t, "RETURN"))
    {
        r->pc++;
        end_statement(r);
        if(r->exec)
        {
            if(r->ncall == 0) { cas_fail(r, "RETURN without a CALL"); }
            r->ncall--;
            restore_locals(r, r->calls[r->ncall].locals_start);
            r->pc = r->calls[r->ncall].return_pc;
        }
        return;
    }
    if(is_name(t, "HALT"))
    {
        r->pc++;
        end_statement(r);
        if(r->exec) { r->halted = 1; }
        return;
    }
    if(is_name(t, "LOCAL"))
    {
        r->pc++;
        for(;;)
        {
            struct s_CAS_TOKEN * name = cur(r);
            expect(r, TK_NAME, "a variable name");
            if(r->exec) { declare_local(r, name->text); }
            if(cur(r)->type == TK_COMMA) { r->pc++; continue; }
            break;
        }
        end_statement(r);
        return;
    }
    if(is_name(t, "DIMNUM") || is_name(t, "DIMSTR"))
    {
        int is_str = is_name(t, "DIMSTR");
        r->pc++;
        stmt_dim(r, is_str);
        return;
    }
    if(is_name(t, "MASTERVARSET"))
    {
        struct s_CAS_TOKEN * name;
        cas_value value;
        r->pc++;
        name = cur(r);
        expect(r, TK_NAME, "a master variable");
        if(cur(r)->type != TK_OP || cur(r)->op != OP_EQ) { cas_fail(r, "expected '='"); }
        r->pc++;
        value = expr(r);
        end_statement(r);
        if(r->exec)
        {
            if(cas_env_find(&cas_master, name->text) == NULL) { cas_fail(r, "MASTERVARSET %s: MASTER.CAS never created it", name->text); }
            assign(r, &cas_master, name->text, NULL, 0, value);
        }
        return;
    }
    if(is_name(t, "ELSE") && r->s->tokens[r->pc + 1].type == TK_LBRACE)
    {
        /* a second ELSE after an IF's ELSE block: CoM2 reaches it only by running on, so it is skipped like any ELSE after a block that ran */
        cas_warn(r, "ELSE without an IF (its block is skipped)");
        if(r->exec) { r->pc = r->s->match[r->pc + 1] + 1; }
        else { r->pc += 2; }
        return;
    }
    if(is_name(t, "ELSE") || is_name(t, "THEN") || is_name(t, "ENDLP") || is_name(t, "TO") || is_name(t, "STEP"))
    {
        cas_fail(r, "%s out of place", t->text);
    }

    /* Assignment: NAME = expr;  NAME[i,j] = expr; */
    {
        struct s_CAS_TOKEN * next = &r->s->tokens[r->pc + 1];
        if((next->type == TK_OP && next->op == OP_EQ) || next->type == TK_LBRACK)
        {
            cas_value idx[CAS_MAX_DIMS + CAS_MAX_ARGS];
            int nidx = 0;
            cas_value value;
            r->pc++;
            if(cur(r)->type == TK_LBRACK) { r->pc++; nidx = parse_args(r, idx, TK_RBRACK); }
            if(cur(r)->type != TK_OP || cur(r)->op != OP_EQ) { cas_fail(r, "expected '='"); }
            r->pc++;
            value = expr(r);
            end_statement(r);
            if(r->exec) { assign(r, &r->vars, t->text, idx, nidx, value); }
            return;
        }
    }

    /* Command (NAME a, b, ...;) or a function called for its effect (NAME(a, b);) */
    {
        int idx = builtin_index(t->text);
        cas_value args[CAS_MAX_ARGS];
        int n = 0;
        if(idx < 0) { cas_fail(r, "unknown command or missing '=': %s", t->text); }
        r->pc++;
        if(cur(r)->type == TK_LPAREN && (cas_builtins[idx].kind == BI_FUNC || parens_enclose_statement(r)))
        {
            r->pc++;
            n = parse_args(r, args, TK_RPAREN);
        }
        else if(cur(r)->type != TK_SEMI && cur(r)->type != TK_RBRACE && cur(r)->type != TK_END)
        {
            for(;;)
            {
                if(n == CAS_MAX_ARGS) { cas_fail(r, "too many arguments"); }
                args[n++] = expr(r);
                if(cur(r)->type == TK_COMMA) { r->pc++; continue; }
                break;
            }
        }
        end_statement(r);
        call_builtin(r, idx, args, n);
    }
}

/* 1 if the '(' at pc closes just before the end of the statement: `ADDAISPELL(W,S);` rather than `ADDRESLINE (A), B;` */
static int parens_enclose_statement(struct s_CAS_RUN * r)
{
    int depth = 0;
    int i;
    for(i = r->pc; r->s->tokens[i].type != TK_END; i++)
    {
        int type = r->s->tokens[i].type;
        if((type == TK_SEMI || type == TK_RBRACE) && depth > 0) { return 1; }   /* `CMD(a, (b);`: the closing ')' is missing */
        if(type == TK_LPAREN) { depth++; }
        else if(type == TK_RPAREN && --depth == 0)
        {
            type = r->s->tokens[i + 1].type;
            return type == TK_SEMI || type == TK_RBRACE || type == TK_END || r->s->tokens[i + 1].line > r->s->tokens[i].line;
        }
    }
    return 0;
}

static int cas_execute(struct s_CAS_RUN * r)
{
    if(setjmp(r->fail) != 0)
    {
        return -1;
    }
    while(!r->halted && cur(r)->type != TK_END)
    {
        statement(r);
    }
    return 0;
}

static void run_init(struct s_CAS_RUN * r, CAS_Script * s, int exec)
{
    memset(r, 0, sizeof(*r));
    r->s = s;
    r->exec = exec;
    r->res_sum = cas_int(0);
    r->res_lines = 0;
}

static void run_free(struct s_CAS_RUN * r)
{
    cas_env_free(&r->vars);
    free(r->locals);
}

int CAS_Check(CAS_Script * script)
{
    struct s_CAS_RUN r;
    int result;
    run_init(&r, script, 0);
    cas_error[0] = '\0';
    result = cas_execute(&r);
    run_free(&r);
    return result;
}

void CAS_Builtins_Used(CAS_Script * script, CAS_Builtin_Use_Fn callback, void * user_data)
{
    struct s_CAS_RUN r;
    int uses[CAS_BUILTIN_COUNT];
    int itr;
    memset(uses, 0, sizeof(uses));
    run_init(&r, script, 0);
    r.quiet = 1;
    r.builtin_uses = uses;
    cas_execute(&r);
    run_free(&r);
    for(itr = 0; itr < CAS_BUILTIN_COUNT; itr++)
    {
        if(uses[itr] > 0) { callback(cas_builtins[itr].name, uses[itr], (cas_builtins[itr].fn != NULL) || (cas_builtins[itr].game != NULL), user_data); }
    }
}

int CAS_Run_Master(CAS_Script * script)
{
    struct s_CAS_RUN r;
    int result;
    run_init(&r, script, 1);
    r.is_master = 1;
    cas_error[0] = '\0';
    r.vars = cas_master;                                               /* the master script writes the globals */
    result = cas_execute(&r);
    cas_master = r.vars;
    memset(&r.vars, 0, sizeof(r.vars));
    run_free(&r);
    if(result != 0) { LOG_ERROR(LOG_CAT_MOD, "[CAS] %s", cas_error); }
    return result;
}

static int cas_last_res_lines;
static int32_t cas_last_res_sum;

int CAS_Last_Result_Lines(int32_t * sum)
{
    if(sum != NULL) { *sum = cas_last_res_sum; }
    return cas_last_res_lines;
}

int CAS_Run(CAS_Script * script, const struct s_CAS_INPUT * inputs, int input_count, const char * result_name, int32_t * result)
{
    struct s_CAS_RUN r;
    char upper[CAS_NAME_MAX + 1];
    int itr;
    int status;

    run_init(&r, script, 1);
    cas_error[0] = '\0';
    for(itr = 0; itr < input_count; itr++)
    {
        strncpy(upper, inputs[itr].name, CAS_NAME_MAX);
        upper[CAS_NAME_MAX] = '\0';
        cas_upper(upper);
        cas_env_add(&r.vars, upper)->v = make_int(inputs[itr].value);
    }
    strncpy(upper, result_name, CAS_NAME_MAX);
    upper[CAS_NAME_MAX] = '\0';
    cas_upper(upper);
    cas_env_add(&r.vars, upper)->v = make_int(*result);

    cas_last_res_lines = 0;
    cas_last_res_sum = 0;
    status = cas_execute(&r);
    if(status == 0)
    {
        struct s_CAS_VAR * v = cas_env_find(&r.vars, upper);
        double sum = cas_double(r.res_sum);
        cas_last_res_lines = r.res_lines;
        cas_last_res_sum = (sum > 2147483647.0) ? INT32_MAX : (sum < -2147483648.0) ? INT32_MIN : (int32_t)cas_trunc(r.res_sum);
        if(v == NULL || v->is_array || v->v.is_str)
        {
            snprintf(cas_error, sizeof(cas_error), "%s: %s is not a number at the end of the script", script->name, upper);
            status = -1;
        }
        else if(cas_double(v->v.num) > 2147483647.0 || cas_double(v->v.num) < -2147483648.0)
        {
            snprintf(cas_error, sizeof(cas_error), "%s: %s = %g is outside 32-bit range", script->name, upper, cas_double(v->v.num));
            status = -1;
        }
        else
        {
            *result = (int32_t)cas_trunc(v->v.num);
        }
    }
    if(status != 0) { LOG_ERROR(LOG_CAT_MOD, "[CAS] %s", cas_error); }
    run_free(&r);
    return status;
}
