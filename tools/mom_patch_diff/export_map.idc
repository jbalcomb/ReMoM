/*
    export_map.idc -- writes a function and data map of the open database for tools/mom_patch_diff.

    Run in IDA 5.5 on the 1.31 WIZARDS.EXE (or MAGIC.EXE) database:  File > IDC file... > export_map.idc
    It asks for the output file (e.g. wizards_map.csv).

    One line per function and per named data item:
        kind,segment,proc,name,ea,size,bytes
    kind     F (function) or D (named data item)
    segment  IDA segment name (seg161, ovr142, dseg, ...)
    proc     1-based position of the function in its segment (the "p04" of ReMoM's "WZD s161p04"); 0 for data
    ea       start address, hex
    size     bytes
    bytes    hex, two characters per byte, ".." where a relocation fixup makes the loaded byte differ from the file

    mom_patch_diff finds each function in the .EXE files by its bytes, so the map needs no file offsets.
    Not run against a real database yet: if IDA reports an error, the line number points at what to fix.
*/

#include <idc.idc>

static fixup_length(ea)
{
    auto type;
    type = GetFixupTgtType(ea);
    if(type == -1)
    {
        return 0;
    }
    type = type & 0x0F;
    if(type == 3)                 /* FIXUP_PTR32: segment:offset */
    {
        return 4;
    }
    return 2;                     /* FIXUP_SEG16, FIXUP_OFF16 and the rest are 16-bit */
}

static write_bytes(f, start, end)
{
    auto ea, n, i;
    ea = start;
    while(ea < end)
    {
        n = fixup_length(ea);
        if(n > 0)
        {
            for(i = 0; i < n && ea < end; i++)
            {
                fprintf(f, "..");
                ea = ea + 1;
            }
        }
        else
        {
            fprintf(f, "%02X", Byte(ea));
            ea = ea + 1;
        }
    }
}

static clean_name(name)
{
    /* commas would break the CSV */
    auto out, i, c;
    out = "";
    for(i = 0; i < strlen(name); i++)
    {
        c = substr(name, i, i + 1);
        if(c == ",")
        {
            c = "_";
        }
        out = out + c;
    }
    return out;
}

static main()
{
    auto path, f, seg, seg_end, ea, end, proc, name, size, count_f, count_d;

    path = AskFile(1, "*.csv", "Write the mom_patch_diff map to");
    if(path == "")
    {
        return;
    }
    f = fopen(path, "w");
    if(f == 0)
    {
        Warning("Cannot write %s", path);
        return;
    }
    fprintf(f, "kind,segment,proc,name,ea,size,bytes\n");
    count_f = 0;
    count_d = 0;

    seg = FirstSeg();
    while(seg != BADADDR)
    {
        seg_end = SegEnd(seg);

        /* functions, numbered in address order inside the segment */
        proc = 0;
        ea = NextFunction(seg - 1);
        while(ea != BADADDR && ea < seg_end)
        {
            end = GetFunctionAttr(ea, FUNCATTR_END);
            proc = proc + 1;
            fprintf(f, "F,%s,%d,%s,%X,%d,", SegName(ea), proc, clean_name(GetFunctionName(ea)), ea, end - ea);
            write_bytes(f, ea, end);
            fprintf(f, "\n");
            count_f = count_f + 1;
            ea = NextFunction(ea);
        }

        /* named data items outside functions */
        ea = seg;
        while(ea != BADADDR && ea < seg_end)
        {
            name = Name(ea);
            if(name != "" && GetFunctionAttr(ea, FUNCATTR_START) == -1)
            {
                size = ItemSize(ea);
                fprintf(f, "D,%s,0,%s,%X,%d,", SegName(ea), clean_name(name), ea, size);
                write_bytes(f, ea, ea + size);
                fprintf(f, "\n");
                count_d = count_d + 1;
            }
            ea = NextHead(ea, seg_end);
        }

        seg = NextSeg(seg);
    }

    fclose(f);
    Message("export_map: %d functions, %d data items written to %s\n", count_f, count_d, path);
}
