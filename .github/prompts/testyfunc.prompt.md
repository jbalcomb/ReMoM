---
name: 'testyfunc'
description: 'Generate a unit-test for selected function'
agent: 'agent'
tools: ['read', 'edit', 'search', 'ms-vscode.cpp-devtools/RunCtest_CMakeTools']
---
Generate a unit-test for selected function, using GoogleTest.
Run the test using ctest to be sure it works.
