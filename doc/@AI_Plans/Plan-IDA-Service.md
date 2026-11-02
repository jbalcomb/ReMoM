# Plan: IDA Pro 5.5 as a REST service on the Coder Workstation (Windows 10), run by NSSM

Status: plan, nothing built. Branch `remom-mod`.

## Goal

Other machines (the Ubuntu AI Workstation, a cloud session attached through Remote Control) ask the Coder
Workstation to run an IDC script against one of its IDA 5.5 databases and get the result back as TSV, instead of
running IDA under Wine themselves. The first job is `export_map.idc`; any other script added to the scripts folder
later works the same way.

## Design

The service builds on what [tools/mom_patch_diff/export_map.py](../../tools/mom_patch_diff/export_map.py) already
does: it copies the database, runs `idag.exe`/`idaw.exe -A -S<script> -L<log>` on the copy, and checks the output.

- **Server:** `tools/ida_service/ida_service.py`, standard library only (`http.server.ThreadingHTTPServer`, a
  `queue.Queue`, one worker thread). IDA 5.5 runs one job per process; jobs run one at a time.
- **No paths or code from the client.** A request names a database and a script by *name* only:
  - databases from a configured folder (`WIZARDS`, `MAGIC`, later one per version);
  - scripts from `tools/ida_service/scripts/` in the repo.

  IDC can run programs (`Exec`), so accepting uploaded IDC would mean accepting arbitrary code. To add a script,
  commit it.
- **Never touch the originals:** every job runs on its own copy of the `.idb` and its companions, as
  `export_map.py` does now.
- **Job folder:** `jobs/<id>/` under the service's data folder holds `job.json` (state, times, exit code), the
  output TSV and `ida.log`. Jobs older than a configured number of days are deleted. When the service restarts,
  jobs it left `running` are marked `failed`.
- **Timeout:** each job has a timeout (default 1800 s). After it, the IDA process is killed and the job fails with
  the last lines of the log.

### Endpoints

| Method | Path | Returns |
|---|---|---|
| `GET` | `/health` | service version, IDA path, databases and scripts available, queue length |
| `POST` | `/jobs` | body `{"idb": "WIZARDS", "script": "export_map"}`; returns `{"id": ...}` |
| `GET` | `/jobs/<id>` | `job.json` |
| `GET` | `/jobs/<id>/output` | the TSV |
| `GET` | `/jobs/<id>/log` | IDA's log |

### Access

- The server binds to `127.0.0.1` by default.
- To serve the LAN: bind to the workstation's LAN address and require a bearer token (stored in the config file,
  never in the repo). Then add one Windows Firewall rule that allows the port **only from the AI Workstation's
  address**.
- An SSH tunnel through Windows 10's OpenSSH server is the alternative that needs neither the open port nor the
  token.

### Client

- `tools/ida_service/ida_client.py`, standard library only: `submit`, `wait`, `fetch`.
- `export_map.py --service URL` uses the client, so the address table and patch index steps in
  [Handoff-remom-mod.md](Handoff-remom-mod.md) work the same whether IDA is local, under Wine, or on the Coder
  Workstation.

### Code changes

- `export_map.py`: move the run into a function, `run_ida(ida, idb, script_text, out, timeout, wine)`. Its
  `main()` and the service both call it.
- The script-file handling (the `@@OUTPUT_PATH@@` placeholder) stays as it is.

## Running it as a Windows service with NSSM

NSSM (the Non-Sucking Service Manager) runs any program as a Windows service. It restarts the program when it
exits, rotates its stdout/stderr logs, and stops it with Ctrl+C and then by killing its process tree.

### Vetting (do this first, on the workstation)

- **Tier C:** dev-only, never ships, never builds the product. Note that it is *not* "offline scratch". It starts
  a program at boot, under a service account, that listens on the network. The blast radius is that account and
  whatever that account can reach. The measures below keep it small.
- **Facts recalled from memory, not verified.** This cloud session could not reach `nssm.cc`, and the GitHub API
  lookups failed. Verify each of these on the workstation:
  - author Iain Patterson; public domain;
  - the last release is 2.24 (2014);
  - the pre-release 2.24-101-g897c7ad (2017) is the one recommended for Windows 10 (Creators Update and later),
    where 2.24 services can fail to start;
  - source at `git.nssm.cc`;
  - the binaries are not code-signed;
  - some antivirus products flag it, because malware uses it for persistence.
- **Gaps that stay open:**
  - The project is unmaintained since 2017.
  - Nobody publishes reproducible builds.
  - No signature ties the binary to its author.

  Record these with the decision, as the [vetting policy](../@Devel/Dependency-Vetting.md) requires.
- If NSSM is rejected, WinSW (MIT-licensed, maintained on GitHub, builds in public CI) is the replacement. It
  fills the same role with an XML config, and nothing else in this plan changes.

### Containing it

- **Location:** not installed system-wide and not on `PATH`. `nssm.exe` lives at
  `tools/ida_service/bin/nssm.exe` (git-ignored). Its SHA-256 is recorded in the tracked
  `tools/ida_service/nssm_manifest.tsv`. `install_service.sh` refuses to run if the hash differs.
- **Account:** the service runs as a dedicated local account (`idasvc`), not LocalSystem. That account can read
  only the IDA install, the database folder and the repo checkout, and write only the service's data folder.
  - Log in as `idasvc` once, interactively, and start IDA by hand. IDA 5.5 asks its first-run questions (licence
    agreement) per user. In a service those dialogs would hang where nobody can see them.
- **Use `idaw.exe` (text mode), not `idag.exe`.** Services run in session 0 and have no visible desktop, so any
  dialog IDA opens there waits until the job's timeout. The text-mode IDA has less to open.
- **Interpreter:** use the absolute path of the **Python 3** interpreter. On this project's Windows boxes `python`
  is 2.7 (see [AGENTS.md](../../AGENTS.md)). The service also checks `sys.version_info` and exits with an error
  under Python 2.

### Install (elevated Git Bash; `install_service.sh` will do these steps after the hash check)

```bash
NSSM=/c/STU/devel/ReMoM/tools/ida_service/bin/nssm.exe
PY3='C:/Users/<you>/AppData/Local/Programs/Python/Python312/python.exe'   # the Python 3 interpreter, absolute
SVC=ReMoM-IDA
"$NSSM" install $SVC "$PY3"
"$NSSM" set $SVC AppParameters 'C:/STU/devel/ReMoM/tools/ida_service/ida_service.py --config C:/STU/ida_service/config.json'
"$NSSM" set $SVC AppDirectory 'C:/STU/ida_service'
"$NSSM" set $SVC ObjectName '.\idasvc' '<password>'
"$NSSM" set $SVC Start SERVICE_DELAYED_AUTO_START
"$NSSM" set $SVC AppStdout 'C:/STU/ida_service/logs/service_stdout.log'
"$NSSM" set $SVC AppStderr 'C:/STU/ida_service/logs/service_stderr.log'
"$NSSM" set $SVC AppRotateFiles 1
"$NSSM" set $SVC AppRotateBytes 10485760
"$NSSM" set $SVC AppExit Default Restart
"$NSSM" set $SVC AppThrottle 5000
"$NSSM" set $SVC AppKillProcessTree 1
"$NSSM" start $SVC
curl -s http://127.0.0.1:8765/health
```

- Arguments that hold Windows paths use the `C:/...` form in single quotes. Git Bash rewrites `/c/...` only when
  the whole argument is one path, and `AppParameters` holds several.
- On stop, NSSM sends Ctrl+C. The service catches it, kills the running IDA, marks that job `failed` and exits.
  `AppKillProcessTree` covers the case where the service does not exit in time.

## Phases

1. **Service on Linux, tested with the fake IDA.**
   - Write `run_ida()`, `ida_service.py` and `ida_client.py`.
   - Unit tests start the server on an ephemeral port with the fake `idag` from
     `tools/mom_patch_diff/tests/test_address_table.py`.
   - Cover: submit, poll, fetch, unknown database or script names (rejected), timeout, restart marking jobs failed,
     token check.
   - This phase can be done in a cloud session.
2. **By hand on the Coder Workstation.** Run `python3 tools/ida_service/ida_service.py` in a console with the real
   IDA 5.5 and `WIZARDS.idb`.
   - This is also the first real run of `export_map.idc`, which has only ever run against the fake.
3. **NSSM.**
   - Vet it and record the decision.
   - Create `idasvc` and do its first IDA run.
   - Run `install_service.sh`, reboot, and check that `/health` answers.
4. **From the AI Workstation.**
   - Add the token and the firewall rule (or the SSH tunnel).
   - Run `export_map.py --service` for both databases, then the address table.
   - Update [Handoff-remom-mod.md](Handoff-remom-mod.md).

## Open questions

- **Port:** 8765 is a placeholder.
- **Several IDA versions:** one database folder per game version means one service with a `version` field in the
  request, rather than one service per version.
