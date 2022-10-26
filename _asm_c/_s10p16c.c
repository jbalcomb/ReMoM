// _s10p16c.c ExtractFileBase (drake178: StripFileExt)

void ExtractFileBase(char * LbxFileName)
{
    int itr_filename;

    itr_filename = 0;

    while (LbxFileName[itr_filename] != '\0')
    {

        if ( LbxFileName[itr_filename] >= 'a' )
        {
            LbxFileName[itr_filename] = LbxFileName[itr_filename] - 32;  // {'a' - 32 = 'A', ..., 'z' - 32 = 'Z'}
        }

        if ( LbxFileName[itr_filename] == '.' )
        {
            LbxFileName[itr_filename] = '\0';
        }
        
        itr_filename++;
    }
}
