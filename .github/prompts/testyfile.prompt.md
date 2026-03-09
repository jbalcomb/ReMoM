---
name: 'testyfile'
description: 'Generate unit-tests for each function in the current file'
agent: 'agent'
tools: ['read', 'edit', 'search', 'ms-vscode.cpp-devtools/RunCtest_CMakeTools']
---
Generate unit-tests for each function in the current file, using GoogleTest.
Run the tests using ctest to be sure they work.
