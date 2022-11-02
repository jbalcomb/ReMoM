/*
    struct (2048 bytes) lbx_header
        Address: 02:001B3A6C
    signed integer (2 bytes) entries
    unsigned integer (2 bytes) magic_number
    signed integer (2 bytes) file_type
    signed integer (2 bytes) dummy
    array (2040 bytes) offsets
        Num elements: 510, Type: unsigned integer (4 bytes)
*/

struct MoX__lbx_header
{
    signed short int    entries;
    unsigned short int  magic_number;
    signed short int    file_type;
    signed short int    dummy;
    unsigned long int   offsets[510];
}
