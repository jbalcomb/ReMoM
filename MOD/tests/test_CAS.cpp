#include <gtest/gtest.h>

#include <cstdint>
#include <cstring>
#include <string>

extern "C" {
#include "../../MoX/src/random.h"
#include "../src/CAS.h"
}

namespace
{

/* Loads `source`, runs it with RESULT preset to `initial` and returns RESULT.  Fails the test on a load or run error. */
int32_t Run_Result(const char * source, int32_t initial = 0, const struct s_CAS_INPUT * inputs = nullptr, int input_count = 0)
{
    CAS_Script * script = CAS_Load_String("test.CAS", source);
    EXPECT_NE(script, nullptr) << CAS_Last_Error();
    if(script == nullptr) { return INT32_MIN; }
    struct s_CAS_INPUT all[16];
    int n = 0;
    all[n++] = { "RESULT", initial };
    for(int i = 0; i < input_count; i++) { all[n++] = inputs[i]; }
    int32_t result = initial;
    int rc = CAS_Run(script, all, n, "RESULT", &result);
    EXPECT_EQ(rc, 0) << CAS_Last_Error();
    CAS_Free(script);
    return result;
}

/* Loads and runs `source`; returns CAS_Last_Error() if anything failed, "" otherwise. */
std::string Run_Error(const char * source)
{
    CAS_Script * script = CAS_Load_String("test.CAS", source);
    if(script == nullptr) { return CAS_Last_Error(); }
    int32_t result = 0;
    int rc = CAS_Run(script, nullptr, 0, "RESULT", &result);
    std::string error = (rc == 0) ? "" : CAS_Last_Error();
    CAS_Free(script);
    return error;
}

/* Loads and checks `source`; returns the warning count (-1 if it does not load or check). */
int Check_Warnings(const char * source)
{
    CAS_Script * script = CAS_Load_String("test.CAS", source);
    if(script == nullptr) { return -1; }
    int rc = CAS_Check(script);
    int count = 0;
    CAS_Warnings(&count);
    CAS_Free(script);
    return (rc == 0) ? count : -1;
}

class CAS_test : public ::testing::Test
{
protected:
    void SetUp() override { CAS_Clear_Master(); }
    void TearDown() override { CAS_Clear_Master(); }
};

TEST_F(CAS_test, Expressions_Are_Right_Recursive)
{
    EXPECT_EQ(Run_Result("RESULT = 2*3+4;"), 14);                        /* 2*(3+4) */
    EXPECT_EQ(Run_Result("RESULT = 10-4-3;"), 9);                        /* 10-(4-3) */
    EXPECT_EQ(Run_Result("RESULT = (10-4)-3;"), 3);
    EXPECT_EQ(Run_Result("B = 5; RESULT = B+3*2;"), 11);                 /* Warlord's BASEGOLD=BASEGOLD+3*BREWER */
}

TEST_F(CAS_test, Numbers_Are_Exact_Fractions)
{
    EXPECT_EQ(Run_Result("RESULT = (1/3)*3;"), 1);
    EXPECT_EQ(Run_Result("RESULT = (7/2)*10;"), 35);
    EXPECT_EQ(Run_Result("RESULT = 7/2;"), 3);                           /* the result variable is truncated */
    EXPECT_EQ(Run_Result("RESULT = -7/2;"), -3);
}

TEST_F(CAS_test, Prefix_Operators)
{
    EXPECT_EQ(Run_Result("RESULT = %R (5/2);"), 3);
    EXPECT_EQ(Run_Result("RESULT = %R (-5/2);"), -3);
    EXPECT_EQ(Run_Result("RESULT = %I (9/4);"), 2);
    EXPECT_EQ(Run_Result("RESULT = (%F (9/4))*4;"), 1);
    EXPECT_EQ(Run_Result("RESULT = %N 0;"), 1);
    EXPECT_EQ(Run_Result("RESULT = %N 7;"), 0);
}

TEST_F(CAS_test, Comparisons_And_Logic)
{
    EXPECT_EQ(Run_Result("RESULT = (3<4) %AND (4<=4);"), 1);
    EXPECT_EQ(Run_Result("RESULT = (3>4) %OR (2<>2);"), 0);
    EXPECT_EQ(Run_Result("RESULT = (1=1) %XOR (1=1);"), 0);
    EXPECT_EQ(Run_Result("A$ = \"Orc\"; RESULT = (A$ = \"Orc\");"), 1);
}

TEST_F(CAS_test, If_Then_Else)
{
    EXPECT_EQ(Run_Result("IF 1 THEN { RESULT = 1; } ELSE { RESULT = 2; }"), 1);
    EXPECT_EQ(Run_Result("IF 0 THEN { RESULT = 1; } ELSE { RESULT = 2; }"), 2);
    EXPECT_EQ(Run_Result("IF 0 THEN { RESULT = 1; } RESULT = RESULT + 5;", 10), 15);
    EXPECT_EQ(Run_Result("if 1 then { if 0 then { result = 1; } else { result = 3; } }"), 3);   /* case-insensitive */
}

TEST_F(CAS_test, For_Next_Is_Inclusive_And_Runs_Once)
{
    EXPECT_EQ(Run_Result("FOR I=1 TO 10; RESULT = RESULT + I; NEXT I;"), 55);
    EXPECT_EQ(Run_Result("FOR I=10 TO 1 STEP -3; RESULT = RESULT + I; NEXT;"), 22);   /* 10 7 4 1 */
    EXPECT_EQ(Run_Result("FOR I=5 TO 1; RESULT = RESULT + 1; NEXT I;"), 1);
}

TEST_F(CAS_test, Loop_Endlp)
{
    EXPECT_EQ(Run_Result("LOOP { RESULT = RESULT + 1; } ENDLP RESULT >= 4;"), 4);
    EXPECT_EQ(Run_Result("LOOP 0 { RESULT = 99; } ENDLP;", 7), 7);
}

TEST_F(CAS_test, Goto_Call_Return_Local)
{
    EXPECT_EQ(Run_Result("GOTO \"SKIP\"; RESULT = 1; !SKIP! RESULT = RESULT + 2;"), 2);
    EXPECT_EQ(Run_Result("X = 1; CALL \"SUB\"; RESULT = X; HALT; !SUB! LOCAL X; X = 50; RETURN;"), 1);
    EXPECT_EQ(Run_Result("CALL \"SUB\"; RESULT = RESULT + 1; HALT; !SUB! RESULT = 10; RETURN;"), 11);
}

TEST_F(CAS_test, Arrays_Are_One_Based)
{
    EXPECT_EQ(Run_Result("DIMNUM A[3,2]; A[3,2] = 7; A[1,1] = 2; RESULT = A[3,2]*A[1,1];"), 14);
    EXPECT_EQ(Run_Error("DIMNUM A[3]; A[0] = 1;"), "");                    /* 0 is accepted too */
    EXPECT_NE(Run_Error("DIMNUM A[3]; A[-1] = 1;"), "");
    EXPECT_NE(Run_Error("DIMNUM A[3]; A[4] = 1;"), "");
}

TEST_F(CAS_test, Strings)
{
    EXPECT_EQ(Run_Result("A$ = \"1\" + STRS(23); RESULT = VAL(A$);"), 123);
    EXPECT_EQ(Run_Result("DIMSTR N$[2]; N$[2] = \"x\"; RESULT = (N$[2] = \"x\");"), 1);
}

TEST_F(CAS_test, Comments_And_Labels)
{
    EXPECT_EQ(Run_Result(":set\nthe result: RESULT = 4; :done:"), 4);
}

TEST_F(CAS_test, Inputs_And_Result_Kept_When_Unset)
{
    struct s_CAS_INPUT inputs[] = { { "CITYID", 3 }, { "FOOD", 20 } };
    EXPECT_EQ(Run_Result("RESULT = (CityID*100) + food;", 0, inputs, 2), 320);
    EXPECT_EQ(Run_Result("RESULT = CityID*100 + food;", 0, inputs, 2), 360);   /* right-recursive: 3*(100+20) */
    EXPECT_EQ(Run_Result("X = 1;", 42), 42);
}

TEST_F(CAS_test, Master_Variables)
{
    CAS_Script * master = CAS_Load_String("MASTER.CAS", "BONUS = 5;");
    ASSERT_NE(master, nullptr) << CAS_Last_Error();
    ASSERT_EQ(CAS_Run_Master(master), 0) << CAS_Last_Error();
    EXPECT_EQ(Run_Result("RESULT = BONUS*2;"), 10);
    EXPECT_EQ(Run_Result("MASTERVARSET BONUS = 9; RESULT = BONUS;"), 9);
    EXPECT_EQ(Run_Result("RESULT = BONUS;"), 9);
    EXPECT_NE(Run_Error("MASTERVARSET NOSUCH = 1;"), "");
    CAS_Free(master);
}

TEST_F(CAS_test, Rnd_Uses_The_Game_Rng)
{
    Set_Random_Seed(12345);
    int32_t a = Run_Result("RESULT = RND(1000);");
    int32_t b = Run_Result("RESULT = RND(1000);");
    Set_Random_Seed(12345);
    EXPECT_EQ(Run_Result("RESULT = RND(1000);"), a);
    EXPECT_EQ(Run_Result("RESULT = RND(1000);"), b);
    EXPECT_GE(a, 0);
    EXPECT_LT(a, 1000);
}

TEST_F(CAS_test, Result_Lines)
{
    EXPECT_EQ(Run_Result("ADDRESLINE \"Base\", 3; ADDRESLINE \"Brewer\", 2; RESULT = SUMRESLINES;"), 5);
}

TEST_F(CAS_test, Errors)
{
    EXPECT_NE(Run_Error("RESULT = 1/0;"), "");
    EXPECT_NE(Run_Error("GOTO \"NOWHERE\";"), "");
    EXPECT_NE(Run_Error("LOOP { X = 1; } ENDLP 0;").find("runaway"), std::string::npos);
    EXPECT_NE(Run_Error("SETSTAT 1, 2, 3, 4;").find("not implemented in ReMoM yet"), std::string::npos);
    EXPECT_EQ(CAS_Builtin_Implemented("RND"), 1);
    EXPECT_EQ(CAS_Builtin_Implemented("SETSTAT"), 0);
    EXPECT_EQ(CAS_Builtin_Implemented("NOSUCHTHING"), -1);
}

/* Things CoM2 accepts that the language does not: accepted, each with a warning. */
TEST_F(CAS_test, Leniency_Warns)
{
    EXPECT_EQ(Check_Warnings("RESULT = 1;"), 0);
    EXPECT_EQ(Check_Warnings("RESULT = 1\nRESULT = 2;"), 1);                    /* missing ';' */
    EXPECT_EQ(Check_Warnings("RESULT = ABS(1;"), 1);                            /* missing ')' */
    EXPECT_EQ(Check_Warnings("RESULT = ABS(1));"), 1);                          /* extra ')' */
    EXPECT_EQ(Check_Warnings("RESULT = 1; :never closed"), 1);
    EXPECT_EQ(Check_Warnings("RESULT = 1; }"), 1);                              /* '}' without '{' */
    EXPECT_EQ(Check_Warnings("IF 1 THEN { RESULT = 1;"), 1);                    /* '{' never closed */
    EXPECT_EQ(Check_Warnings("IF 1 THEN { X=1; } ELSE { X=2; } ELSE { X=3; }"), 1);
    EXPECT_EQ(Run_Result("IF 1 THEN { RESULT=1; } ELSE { RESULT=2; } ELSE { RESULT=3; }"), 1);
    EXPECT_EQ(Run_Result("IF 0 THEN { RESULT=1; } ELSE { RESULT=2; } ELSE { RESULT=3; }"), 2);
    EXPECT_EQ(Run_Result("RESULT = 1\nRESULT = RESULT + 1"), 2);
}

}  /* namespace */
