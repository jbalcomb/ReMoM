---
name: 'doxyfile'
description: 'Generate Doxygen comment for the current file'
agent: 'agent'
tools: ['search', 'read', 'edit']
---
Generate a summary for [${fileBasename}](${file}), as a Doxygen style file commment.
Do not replace existing comments.