/*
    WIZARDS.EXE
    ovr104
*/

#include "MoX_Data.H"

#include "Help.H"
#include "LBX_Load.H"


// WZD o104p01
void Main_Screen_Help(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Screen_Help()\n", __FILE__, __LINE__);
#endif

// TODO      // MoX_Data
// TODO      // WZD dseg:BB7A  _help_entries[500]
// TODO      // WZD dseg:BB7A  struct s_HLP_ENTRY _help_entries[50];
// TODO  
// TODO      // ; no wrappers, load types, formats, or reserved EMM;
// TODO      // ; this loads records directly to the specified near
// TODO      // ; pointer address, but is limited to 60,000 bytes total
// TODO      // ; when reading from disk, or 32 Kbytes from EMS
// TODO      // ; returns -1 on success, 0 on failure
// TODO      // ; ! EMS loader may report false positive on corruption
// TODO      // WZD s10p12
// TODO      // LBXR_DirectLoader( LBX_FileName@, Entry_Index, LoadTo@, FirstRec, RecCount, RecSize )
// TODO      LBX_Load_Data_Static(hlpentry_lbx_file, 1, (SAMB_ptr)_help_entries, 0, 25, 10);
// TODO  
// TODO      Set_Help_List((struct s_HLP_ENTRY **)_help_entries, 25);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Main_Screen_Help()\n", __FILE__, __LINE__);
#endif

}