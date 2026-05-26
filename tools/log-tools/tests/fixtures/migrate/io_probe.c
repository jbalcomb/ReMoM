/* io_probe.c -- exercises log_migrate_io.py's printf/fprintf detection and skip rules.
 * Not real compilable C; the script only reads text. */

void io_probe(void)
{
    /* printf detection */
    printf("plain printf line\n");
    printf("error: printf with error keyword\n");

    /* fprintf(stderr,...) detection */
    fprintf(stderr, "stderr message\n");
    fprintf(stderr, "FATAL: cannot proceed\n");

    /* fprintf(stdout,...) detection */
    fprintf(stdout, "stdout message\n");

    /* SHOULD BE SKIPPED -- non-logging streams */
    fprintf(fp,             "writing to caller's file pointer\n");
    fprintf(out,            "writing to output file\n");
    fprintf(fout,           "writing to fout\n");
    fprintf(PLAY_LOG,       "writing to specialized stream\n");
    fprintf(replay_file,    "recording replay\n");
    fprintf(logfile,        "writing to caller's logfile\n");
    fprintf(Unit_Outcomes_File, "unit outcome data\n");

    /* SHOULD NOT MATCH printf detector -- these contain 'printf' as a substring */
    sprintf(buf, "format %d", value);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "fprintf line containing the word printf in its message");
}
