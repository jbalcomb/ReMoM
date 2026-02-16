# Unit Test Generation Prompt Template

Use this template when asking an AI assistant to generate unit tests for a specific function.

---

## Template

```
Generate comprehensive unit tests for the following function:

**Function Location:** [file path and line numbers]
**Function Name:** [function_name]
**Programming Language:** [language]
**Testing Framework:** [e.g., Google Test, pytest, Jest, JUnit]

**Function Code:**
```[language]
[paste the complete function code here]
```

**Context:**
- Purpose: [brief description of what the function does]
- Dependencies: [list any external dependencies, global state, or side effects]
- Related types/structures: [paste relevant struct/class definitions if needed]

**Test Requirements:**
1. **Happy Path Cases:** Test normal, expected inputs and outputs
2. **Edge Cases:** Test boundary conditions (empty inputs, null/nullptr, max values, min values, zero)
3. **Error Cases:** Test invalid inputs, error conditions, exceptions
4. **State Verification:** Verify any side effects or state changes
5. **Coverage Goals:** Aim for [X%] branch/line coverage

**Specific Scenarios to Test:**
- [Scenario 1: description]
- [Scenario 2: description]
- [Add more as needed]

**Constraints:**
- Use descriptive test names following pattern: Test_FunctionName_Scenario_ExpectedBehavior
- Include setup and teardown if needed
- Mock/stub [specific dependencies] if applicable
- Follow existing test suite conventions in [test file path]

**Output Format:**
Provide:
1. Complete test file code ready to compile/run
2. Brief explanation of each test case
3. Any setup instructions or dependencies needed
```

---

## Usage Example

```
Generate comprehensive unit tests for the following function:

**Function Location:** src/combat/battle.cpp lines 145-178
**Function Name:** CalculateDamage
**Programming Language:** C++
**Testing Framework:** Google Test

**Function Code:**
```cpp
int CalculateDamage(const Unit* attacker, const Unit* defender, bool isMelee) {
    if (!attacker || !defender) return 0;
    int baseDamage = attacker->GetStrength();
    if (!isMelee) baseDamage = attacker->GetRangedAttack();
    int defense = defender->GetDefense();
    int damage = std::max(0, baseDamage - defense);
    return damage;
}
```

**Context:**
- Purpose: Calculate combat damage between two units
- Dependencies: Unit class with GetStrength(), GetRangedAttack(), GetDefense() methods
- Related types: Unit* (pointer to Unit object)

**Test Requirements:**
1. Happy Path Cases
2. Edge Cases (null pointers, zero values, negative values)
3. Error Cases
4. Coverage Goals: 100% branch coverage

**Specific Scenarios to Test:**
- Melee attack with positive damage result
- Ranged attack with positive damage result
- Defense exceeds attack (should return 0, not negative)
- Null attacker pointer
- Null defender pointer
- Both null pointers
- Zero strength/defense values

**Constraints:**
- Use Google Test macros (TEST, EXPECT_EQ, etc.)
- Mock Unit objects or use test fixtures
- Follow naming: TEST(CalculateDamageTest, ScenarioName)

**Output Format:**
Complete test file ready to compile
```

---

## Tips for Effective Test Generation

1. **Be Specific:** The more context you provide, the better the generated tests
2. **Include Edge Cases:** Explicitly list edge cases you want covered
3. **Provide Examples:** If you have existing test patterns, include them
4. **State Coverage Goals:** Specify if you need statement, branch, or path coverage
5. **Mention Mocking Needs:** Identify which dependencies should be mocked
6. **Review Output:** Always review generated tests for completeness and correctness
