


int16_t Get_List_Field(int16_t title_x1, int16_t title_y1, int16_t fill_width, char * title_string, char * text_string[], int16_t string_width, int16_t * variable, int16_t * active_flag, int16_t select_fill_type, int16_t sfill1, int16_t sfill2, int16_t sfill3, int16_t sfill4, int16_t help_entry)

char * text_string[]
arrray of memory addresses of character arrays

int64_t string_address = 0;
string_address = (int64_t)&text_string[0];
convert pointer to number
string_address holds the memory address of the position of the first element in the text_string array



printf("(char *)string_address: %s", (char *)string_address);
...
appears to print the address as the string, rather that printing the string at the address
