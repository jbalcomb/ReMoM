#ifndef WIN32_LBX_H
#define WIN32_LBX_H

unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex);
unsigned int LBX_Load_Entry(char *LbxName, int LbxEntry, unsigned int SAMB_head, int LoadType, int LbxHdrFmt);

#endif /* WIN32_LBX_H */
