/*
    STU File I/O
    This module contains functions for working with files, such as opening, reading, writing, and
    closing files. It may also include functions for working with specific file formats used by the game,
    such as LBX archives or save game files. The functions in this module are intended to be used by
    other parts of the game code that need to read or write data to files, such as loading game resources
    or saving game progress. For example, STU_File_Open() can be used to open a file and obtain a file
    pointer that can be used for subsequent read/write operations on that file.

*/

#include "STU_FIO.h"

#include <stdio.h>



void STU_File_Open(void * file_struct, const char * file_name, const char * mode)
{
    FILE * file = fopen(file_name, mode);
    if(file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", file_name);
        // Handle error as needed (e.g., exit, return an error code, etc.)
    }
    else
    {
        // Store the file pointer in the provided structure as needed
        // For example, if file_struct is a struct that contains a FILE* member:
        // ((MyFileStruct*)file_struct)->file_pointer = file;
    }
}
