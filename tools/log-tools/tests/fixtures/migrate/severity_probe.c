/* severity_probe.c -- exercises every branch of log_migrate.py's severity heuristic.
 * File is intentionally NOT a real compilable C file; the migration script only reads text. */

void severity_probe(void)
{
    dbg_prn("FATAL: cannot recover, aborting\n");
    dbg_prn("error: write failed for slot 3\n");
    dbg_prn("warn: unexpected state at turn end\n");
    dbg_prn("turn 1 begin\n");
    trc_prn("TRACE stack %d picked %s\n", id, name);
}
