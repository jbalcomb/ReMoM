
/*
    API ~== ST_SA.C/.H
*/


/*
    Simtex SPACE ALLOC
        - SAMB (Space Alloc Memory Block)
MGC seg007
MGC seg008
SEE: ST_SAMB.md
*/


/*
    Types
*/
/*
    SAMB Head
    SAMB Data
    'Load Type' {0,1,2}
    sah0, sah1, sah2
    sad0, sad1, sad2
*/
/*
    SA_Allocate_MemBlk() returns a SAMB Data Type 0
    SA_Allocate_Space()  returns a SAMB Data Type 1
    SA_Alloc_First() returns a SAMB Data Type 2
    SA_Alloc_Next() returns a SAMB Data Type 2
    
    SA_Alloc_Validate() takes a SAMB Head Type 1
    SA_Alloc_First() takes a SAMB Head Type 1
    SA_Alloc_Next() takes a SAMB Head Type 1

    LBX_Load_Entry() takes no SAMB Head or, depending on 'Load Type', a SAMB Head Type 1 or a SAMB Head Type 2

*/
typedef unsigned int        SAMB_addr;
typedef unsigned char  *    SAMB_ptr;
typedef SAMB_addr SAMB_head;  // 
typedef SAMB_addr SAMB_data;  // 


/*
    Manifest Constants
*/
#define SA_MEMSIG1      0x12FA
#define SA_MEMSIG2      0x4ECF


int SA_Alloc_Validate(SAMB_ptr pSAMB_head);
