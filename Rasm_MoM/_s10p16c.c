// _s10p16c.c StripFileExt
// STR_StripFileExt

/*
Proficient C (1987)
pg476
void getpname(path, pname)
    / * find the end of the pathname string * /
    cp = path;      / * start of pathname * /
    while (*cp != '\0')
        ++cp;
    --cp;           / * went one too far * /
    ...
    / * copy the filename part only * /
    while ((*pname = tolower(*cp)) != '.' && *pname != '\0') {
        ++cp;
        ++pname;
    }
    *pname = '\0';
*/

/*
J:\STU\DBWD\developc\DOOM-master\linuxdoom-1.10\w_wad.c

void
ExtractFileBase
( char*		path,
  char*		dest )
{
    char*	src;
    int		length;

    src = path + strlen(path) - 1;
    
    // back up until a \ or the start
    while (src != path
	   && *(src-1) != '\\'
	   && *(src-1) != '/')
    {
	src--;
    }
    
    // copy up to eight characters
    memset (dest,0,8);
    length = 0;
    
    while (*src && *src != '.')
    {
	if (++length == 9)
	    I_Error ("Filename base of %s >8 chars",path);

	*dest++ = toupper((int)*src++);
    }
}

*/
