
/doxyfunc

Auto Model: GPT-5.3-Codex

>>> Follow instructions in doxyfunc.prompt.md

>>> Used 4 references
>>> CLAUDE.md
>>> copilot-instruction.md
>>> doxyfunc.prompt.md
>>> AGENTS.md



Yes, you can document an entire .c or .h file in Doxygen using a special comment block with the @file (or \file) command. Placing this comment block at the top of your file ensures the documentation is associated with the file itself in the generated output. 
Stack Overflow
Stack Overflow
 +2
How to use the @file command
The special comment block must be a Doxygen-recognized format, such as /** ... */ or /*! ... */. The block should be placed at the very beginning of the file, typically after any boilerplate or copyright notices but before other code. 
Doxygen
Doxygen
 +4
Here is an example structure for a .h file:

/**
 * @file myheader.h
 * @brief Provides an overview of the functionality in this file.
 * 
 * This section can contain a more detailed description of the file's purpose, 
 * usage instructions, internal workings, or any other relevant information.
 * It can span multiple lines.
 * 
 * @author Your Name
 * @date 2026-02-18
 */

Key Points
@file command: This command is essential for Doxygen to recognize that the comment block documents the file as a whole. If the filename is omitted after @file (as in @file), Doxygen automatically associates the documentation with the file it is located in.
@brief command: Use @brief for a concise, one-line summary of the file's content.
Location: The documentation block should be placed at the beginning of the file to ensure it is processed correctly as file documentation.
Other tags: You can use other standard Doxygen tags within the block, such as @author, @date, @version, @bug, etc., to provide more information.
Visibility: If the EXTRACT_ALL option is set to NO in your Doxyfile (the default), Doxygen will only generate documentation for files that have at least one special documentation block, like the one using @file.
