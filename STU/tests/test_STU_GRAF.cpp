// Tests for STU_GRAF asset-resolution (Phase 1: read-only search path).
//
// Boundary under test: "given a search path and a file laid out in some dir,
// STU_GRAF_Open_Asset opens the first match; not-found returns NULL." We drive
// the low-level Reset/Add_Search_Dir API so the test is deterministic and does
// not depend on the executable location or environment.

#include <gtest/gtest.h>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

extern "C" {
#include "../src/STU_GRAF.h"
#include "../src/STU_HASH.h"
#include "../src/STU_LOG.h"
}

namespace fs = std::filesystem;

namespace
{
    // A temp directory unique to each test, cleaned up on destruction.
    class TempTree
    {
    public:
        explicit TempTree(const std::string & tag)
        {
            root_ = fs::temp_directory_path() / ("stu_graf_" + tag);
            fs::remove_all(root_);
            fs::create_directories(root_);
        }
        ~TempTree() { std::error_code ec; fs::remove_all(root_, ec); }

        std::string dir() const { return root_.string(); }

        void write(const std::string & name, const std::string & content) const
        {
            std::ofstream(root_ / name, std::ios::binary) << content;
        }
        std::string sub(const std::string & name) const
        {
            fs::path d = root_ / name;
            fs::create_directories(d);
            return d.string();
        }

    private:
        fs::path root_;
    };

    std::string read_all(FILE * fp)
    {
        std::string out;
        char buf[256];
        size_t n;
        while((n = fread(buf, 1, sizeof(buf), fp)) > 0)
        {
            out.append(buf, n);
        }
        return out;
    }

    void set_env(const char * k, const char * v)
    {
#ifdef _WIN32
        _putenv_s(k, v);
#else
        setenv(k, v, 1);
#endif
    }
    void unset_env(const char * k)
    {
#ifdef _WIN32
        _putenv_s(k, "");
#else
        unsetenv(k);
#endif
    }

    // Read STU_GRAF_Read_Game_Data_From_Ini for a given ini body; returns "" on miss.
    std::string parse_game_data(const std::string & dir, const std::string & body)
    {
        std::string ini = (fs::path(dir) / "ReMoM.ini").string();
        std::ofstream(ini, std::ios::binary) << body;
        char out[1024] = {0};
        int ok = STU_GRAF_Read_Game_Data_From_Ini(ini.c_str(), out, sizeof(out));
        return ok ? std::string(out) : std::string();
    }
}

TEST(STU_GRAF, ResolvesViaSearchDir)
{
    TempTree t("resolve");
    t.write("GRAFTST.DAT", "hello-graf");

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    FILE * fp = STU_GRAF_Open_Asset("GRAFTST.DAT", "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "hello-graf");
    fclose(fp);
}

TEST(STU_GRAF, NotFoundReturnsNull)
{
    TempTree t("missing");

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    FILE * fp = STU_GRAF_Open_Asset("NOPE.DAT", "rb");
    EXPECT_EQ(fp, nullptr);
    if(fp != nullptr) { fclose(fp); }
}

TEST(STU_GRAF, FirstMatchWins)
{
    TempTree a("first_a");
    TempTree b("first_b");
    a.write("DUP.DAT", "from-A");
    b.write("DUP.DAT", "from-B");

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(a.dir().c_str());  // higher priority
    STU_GRAF_Add_Search_Dir(b.dir().c_str());

    FILE * fp = STU_GRAF_Open_Asset("DUP.DAT", "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "from-A");
    fclose(fp);
}

TEST(STU_GRAF, FallsThroughToLaterDir)
{
    TempTree a("ft_a");
    TempTree b("ft_b");
    b.write("ONLY_B.DAT", "in-B");

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(a.dir().c_str());  // no match here
    STU_GRAF_Add_Search_Dir(b.dir().c_str());

    FILE * fp = STU_GRAF_Open_Asset("ONLY_B.DAT", "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "in-B");
    fclose(fp);
}

TEST(STU_GRAF, PathNameOpenedDirectly)
{
    TempTree data("direct_data");
    TempTree other("direct_other");
    data.write("ABS.DAT", "abs-content");
    std::string full = (fs::path(data.dir()) / "ABS.DAT").string();

    // A name that already carries a path is opened directly, ignoring the
    // (unrelated) search dir.
    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(other.dir().c_str());

    FILE * fp = STU_GRAF_Open_Asset(full.c_str(), "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "abs-content");
    fclose(fp);
}

// ---- Phase 3: config [Paths] game_data parsing ----

TEST(STU_GRAF, IniGameDataBasic)
{
    TempTree t("ini_basic");
    EXPECT_EQ(parse_game_data(t.dir(), "[Paths]\ngame_data=/opt/mom\n"), "/opt/mom");
}

TEST(STU_GRAF, IniGameDataSpacesAndQuotes)
{
    TempTree t("ini_sp");
    // Value keeps embedded spaces; surrounding quotes are stripped.
    EXPECT_EQ(parse_game_data(t.dir(),
                              "[Paths]\ngame_data = \"C:\\GOG Games\\Master of Magic\"\n"),
              "C:\\GOG Games\\Master of Magic");
}

TEST(STU_GRAF, IniGameDataCaseInsensitive)
{
    TempTree t("ini_ci");
    EXPECT_EQ(parse_game_data(t.dir(), "[PATHS]\n; a comment\nGAME_DATA=/x/y\n"), "/x/y");
}

TEST(STU_GRAF, IniGameDataMissing)
{
    TempTree t("ini_miss");
    EXPECT_EQ(parse_game_data(t.dir(), "[Other]\ngame_data=/nope\n"), "");  // wrong section
    EXPECT_EQ(parse_game_data(t.dir(), "[Paths]\nfoo=bar\n"), "");          // key absent
    char out[64] = {0};
    std::string nofile = (fs::path(t.dir()) / "does_not_exist.ini").string();
    EXPECT_EQ(STU_GRAF_Read_Game_Data_From_Ini(nofile.c_str(), out, sizeof(out)), 0);
}

TEST(STU_GRAF, UserDirsResolve)
{
    char cfg[1024] = {0};
    char cache[1024] = {0};
    ASSERT_EQ(STU_GRAF_User_Config_Dir(cfg, sizeof(cfg)), 1);
    ASSERT_EQ(STU_GRAF_User_Cache_Dir(cache, sizeof(cache)), 1);
    EXPECT_NE(std::string(cfg).find("ReMoM"), std::string::npos);
    EXPECT_NE(std::string(cache).find("ReMoM"), std::string::npos);
}

// ---- Phase 4: writable per-user family (data dir / open / DIR / LOF / seed) ----

TEST(STU_GRAF, UserDataDirResolves)
{
    char d[1024] = {0};
    ASSERT_EQ(STU_GRAF_User_Data_Dir(d, sizeof(d)), 1);
    EXPECT_NE(std::string(d).find("ReMoM"), std::string::npos);
}

// ---- Phase 3: end-to-end search-path order (XDG is honored only on Linux) ----
#if !defined(_WIN32) && !defined(__APPLE__)

TEST(STU_GRAF, InitUsesConfigGameData)
{
    TempTree cfg("initcfg");
    TempTree cache("initcache");
    TempTree data("initdata");
    data.write("CFGUNIQ.DAT", "via-config");
    fs::create_directories(fs::path(cfg.dir()) / "ReMoM");
    std::ofstream(fs::path(cfg.dir()) / "ReMoM" / "ReMoM.ini", std::ios::binary)
        << "[Paths]\ngame_data=" << data.dir() << "\n";

    unset_env("REMOM_DATA_DIR");
    set_env("XDG_CONFIG_HOME", cfg.dir().c_str());
    set_env("XDG_CACHE_HOME", cache.dir().c_str());

    STU_GRAF_Init(STU_GRAF_PLAYER);
    FILE * fp = STU_GRAF_Open_Asset("CFGUNIQ.DAT", "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "via-config");
    fclose(fp);

    unset_env("XDG_CONFIG_HOME");
    unset_env("XDG_CACHE_HOME");
    STU_GRAF_Reset();
}

TEST(STU_GRAF, InitCacheShadowsConfigGameData)
{
    TempTree cfg("shadowcfg");
    TempTree cache("shadowcache");
    TempTree data("shadowdata");
    // Same-named asset in both the cache dir and the configured game-data dir:
    // the cache copy must win.
    fs::create_directories(fs::path(cache.dir()) / "ReMoM");
    std::ofstream(fs::path(cache.dir()) / "ReMoM" / "DUP.DAT", std::ios::binary) << "from-cache";
    data.write("DUP.DAT", "from-data");
    fs::create_directories(fs::path(cfg.dir()) / "ReMoM");
    std::ofstream(fs::path(cfg.dir()) / "ReMoM" / "ReMoM.ini", std::ios::binary)
        << "[Paths]\ngame_data=" << data.dir() << "\n";

    unset_env("REMOM_DATA_DIR");
    set_env("XDG_CONFIG_HOME", cfg.dir().c_str());
    set_env("XDG_CACHE_HOME", cache.dir().c_str());

    STU_GRAF_Init(STU_GRAF_PLAYER);
    FILE * fp = STU_GRAF_Open_Asset("DUP.DAT", "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "from-cache");
    fclose(fp);

    unset_env("XDG_CONFIG_HOME");
    unset_env("XDG_CACHE_HOME");
    STU_GRAF_Reset();
}

TEST(STU_GRAF, OpenUserWritesUnderDataDir)
{
    TempTree data("userwrite");
    STU_GRAF_Init(STU_GRAF_PLAYER);  // opt into the user-data family
    set_env("XDG_DATA_HOME", data.dir().c_str());

    FILE * fp = STU_GRAF_Open_User("T.DAT", "wb");
    ASSERT_NE(fp, nullptr);
    fputs("hi", fp);
    fclose(fp);

    // The file lands under <XDG_DATA_HOME>/ReMoM/, never in the CWD.
    EXPECT_TRUE(fs::exists(fs::path(data.dir()) / "ReMoM" / "T.DAT"));

    // DIR/LOF resolve to the same place.
    char found[64] = {0};
    EXPECT_EQ(STU_GRAF_User_DIR("T.DAT", found), STU_GRAF_DIR_FOUND);
    EXPECT_STREQ(found, "T.DAT");
    EXPECT_EQ(STU_GRAF_User_LOF("T.DAT"), 2L);

    FILE * rp = STU_GRAF_Open_User("T.DAT", "rb");
    ASSERT_NE(rp, nullptr);
    EXPECT_EQ(read_all(rp), "hi");
    fclose(rp);

    unset_env("XDG_DATA_HOME");
}

TEST(STU_GRAF, UserDirAndLofAbsentAreZero)
{
    TempTree data("userabsent");
    STU_GRAF_Init(STU_GRAF_PLAYER);  // opt into the user-data family
    set_env("XDG_DATA_HOME", data.dir().c_str());

    char found[64];
    found[0] = 'X';
    EXPECT_EQ(STU_GRAF_User_DIR("NOPE.DAT", found), STU_GRAF_DIR_ABSENT);
    EXPECT_EQ(found[0], '\0');
    EXPECT_EQ(STU_GRAF_User_LOF("NOPE.DAT"), 0L);

    unset_env("XDG_DATA_HOME");
}

TEST(STU_GRAF, SeedCopiesOriginalAndLeavesSourceUntouched)
{
    TempTree data("seeduser");  // XDG_DATA_HOME (writable destination)
    TempTree game("seedgame");  // read-only original game-data dir
    game.write("CONFIG.MOM", "orig-bytes");

    set_env("XDG_DATA_HOME", data.dir().c_str());
    STU_GRAF_Init(STU_GRAF_PLAYER);  // PLAYER profile...
    STU_GRAF_Reset();                // ...then isolate the search path to our fixture
    STU_GRAF_Add_Search_Dir(game.dir().c_str());  // Open_Asset finds the original

    ASSERT_EQ(STU_GRAF_Seed_User_File("CONFIG.MOM"), 1);

    // User copy exists with the original's bytes...
    FILE * fp = STU_GRAF_Open_User("CONFIG.MOM", "rb");
    ASSERT_NE(fp, nullptr);
    EXPECT_EQ(read_all(fp), "orig-bytes");
    fclose(fp);

    // ...and the original was never modified.
    FILE * op = fopen((fs::path(game.dir()) / "CONFIG.MOM").string().c_str(), "rb");
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(read_all(op), "orig-bytes");
    fclose(op);

    // Idempotent: a second seed must not clobber the user's working copy.
    FILE * w = STU_GRAF_Open_User("CONFIG.MOM", "wb");
    ASSERT_NE(w, nullptr);
    fputs("user-edited", w);
    fclose(w);
    EXPECT_EQ(STU_GRAF_Seed_User_File("CONFIG.MOM"), 1);
    FILE * fp2 = STU_GRAF_Open_User("CONFIG.MOM", "rb");
    ASSERT_NE(fp2, nullptr);
    EXPECT_EQ(read_all(fp2), "user-edited");
    fclose(fp2);

    STU_GRAF_Reset();
    unset_env("XDG_DATA_HOME");
}

TEST(STU_GRAF, SeedMissingOriginalReturnsZero)
{
    TempTree data("seednone");
    set_env("XDG_DATA_HOME", data.dir().c_str());
    STU_GRAF_Init(STU_GRAF_PLAYER);  // PLAYER profile...
    STU_GRAF_Reset();                // ...with an empty search path -> no original to copy

    EXPECT_EQ(STU_GRAF_Seed_User_File("ABSENT.MOM"), 0);

    unset_env("XDG_DATA_HOME");
}

// HEADLESS profile must keep the user-family CWD-relative (HeMoM / matchup
// determinism): a swapped open/DIR under HEADLESS behaves like the original
// stu_fopen_ci, regardless of XDG_DATA_HOME.
TEST(STU_GRAF, HeadlessUserFamilyUsesCwd)
{
    TempTree data("headlessxdg");
    set_env("XDG_DATA_HOME", data.dir().c_str());
    STU_GRAF_Init(STU_GRAF_HEADLESS);

    FILE * fp = STU_GRAF_Open_User("GRAFHDL.DAT", "wb");
    ASSERT_NE(fp, nullptr);
    fputs("x", fp);
    fclose(fp);

    // Landed in the CWD, NOT under XDG_DATA_HOME/ReMoM.
    EXPECT_TRUE(fs::exists("GRAFHDL.DAT"));
    EXPECT_FALSE(fs::exists(fs::path(data.dir()) / "ReMoM" / "GRAFHDL.DAT"));

    char found[64] = {0};
    EXPECT_EQ(STU_GRAF_User_DIR("GRAFHDL.DAT", found), STU_GRAF_DIR_FOUND);
    EXPECT_EQ(STU_GRAF_User_LOF("GRAFHDL.DAT"), 1L);

    std::remove("GRAFHDL.DAT");
    unset_env("XDG_DATA_HOME");
    STU_GRAF_Reset();  // restore PLAYER default for later tests
}

TEST(STU_GRAF, UserStateDirResolvesAndCreates)
{
    // The log-state resolver both resolves AND creates the dir (STU_LOG opens
    // files there but does not itself mkdir).
    TempTree t("statehome");
    set_env("XDG_STATE_HOME", t.dir().c_str());

    char d[1024] = {0};
    ASSERT_EQ(STU_GRAF_User_State_Dir(d, sizeof(d)), 1);
    std::string s(d);
    EXPECT_NE(s.find("ReMoM"), std::string::npos);
    EXPECT_NE(s.find(t.dir()), std::string::npos);          // under our XDG_STATE_HOME
    EXPECT_TRUE(fs::exists(fs::path(t.dir()) / "ReMoM"));    // created on resolution

    unset_env("XDG_STATE_HOME");
}

TEST(STU_GRAF, BackupAndReseedRestoresOriginalAndBacksUp)
{
    TempTree data("reseeddata");
    TempTree game("reseedgame");
    game.write("TST.DAT", "orig-content");
    set_env("XDG_DATA_HOME", data.dir().c_str());
    STU_GRAF_Init(STU_GRAF_PLAYER);
    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(game.dir().c_str());

    // The user has an edited working copy.
    FILE * w = STU_GRAF_Open_User("TST.DAT", "wb");
    ASSERT_NE(w, nullptr);
    fputs("edited", w);
    fclose(w);

    ASSERT_EQ(STU_GRAF_Backup_And_Reseed_User_File("TST.DAT", "bak"), 1);

    // Working copy now matches the original again...
    FILE * r = STU_GRAF_Open_User("TST.DAT", "rb");
    ASSERT_NE(r, nullptr);
    EXPECT_EQ(read_all(r), "orig-content");
    fclose(r);

    // ...and the edited copy was preserved under <user-data>/bak/.
    fs::path bak = fs::path(data.dir()) / "ReMoM" / "bak" / "TST.DAT";
    ASSERT_TRUE(fs::exists(bak));
    FILE * bf = fopen(bak.string().c_str(), "rb");
    ASSERT_NE(bf, nullptr);
    EXPECT_EQ(read_all(bf), "edited");
    fclose(bf);

    // No original -> no-op, returns 0.
    EXPECT_EQ(STU_GRAF_Backup_And_Reseed_User_File("NOPE.DAT", "bak"), 0);

    STU_GRAF_Reset();
    unset_env("XDG_DATA_HOME");
}

// ---- App-directory name (REMOM_APP_DIR) --------------------------------------
//
// The per-user app directory is normally "ReMoM".  Portable layouts (the AppImage)
// put that directory in the SAME folder as the executable -- which is also named
// ReMoM -- so they override the name via REMOM_APP_DIR.  A filesystem cannot hold a
// file and a directory of the same name, hence the escape hatch.
// See doc/#Devel/Devel-Linux-AppImage.md.

TEST(STU_GRAF, AppDirDefaultsToReMoM)
{
    TempTree data("appdir_default");
    STU_GRAF_Init(STU_GRAF_PLAYER);
    unset_env("REMOM_APP_DIR");
    set_env("XDG_DATA_HOME", data.dir().c_str());

    FILE * fp = STU_GRAF_Open_User("T.DAT", "wb");
    ASSERT_NE(fp, nullptr);
    fclose(fp);
    EXPECT_TRUE(fs::exists(fs::path(data.dir()) / "ReMoM" / "T.DAT"));

    STU_GRAF_Reset();
    unset_env("XDG_DATA_HOME");
}

TEST(STU_GRAF, AppDirHonoursRemomAppDirOverride)
{
    TempTree data("appdir_override");
    STU_GRAF_Init(STU_GRAF_PLAYER);
    set_env("XDG_DATA_HOME", data.dir().c_str());
    set_env("REMOM_APP_DIR", "remom_app_dir");

    FILE * fp = STU_GRAF_Open_User("T.DAT", "wb");
    ASSERT_NE(fp, nullptr);
    fclose(fp);

    // Lands under the overridden name, and the default name is never created --
    // so a file called "ReMoM" could sit safely beside it.
    EXPECT_TRUE(fs::exists(fs::path(data.dir()) / "remom_app_dir" / "T.DAT"));
    EXPECT_FALSE(fs::exists(fs::path(data.dir()) / "ReMoM"));

    STU_GRAF_Reset();
    unset_env("REMOM_APP_DIR");
    unset_env("XDG_DATA_HOME");
}

TEST(STU_GRAF, AppDirOverrideRejectsPathsAndFallsBack)
{
    // The override is a NAME, not a path: anything that could redirect writes out
    // of the resolved base is refused and the default is used instead.
    const char * bad[] = { "../escape", "sub/dir", "back\\slash", "..", ".", "" };

    for(size_t i = 0; i < sizeof(bad) / sizeof(bad[0]); i++)
    {
        TempTree data("appdir_bad");
        STU_GRAF_Init(STU_GRAF_PLAYER);
        set_env("XDG_DATA_HOME", data.dir().c_str());
        set_env("REMOM_APP_DIR", bad[i]);

        char d[1024] = {0};
        ASSERT_EQ(STU_GRAF_User_Data_Dir(d, sizeof(d)), 1) << "bad value: " << bad[i];
        EXPECT_NE(std::string(d).find("ReMoM"), std::string::npos) << "bad value: " << bad[i];
        EXPECT_EQ(std::string(d).find(".."), std::string::npos) << "bad value: " << bad[i];

        STU_GRAF_Reset();
        unset_env("REMOM_APP_DIR");
        unset_env("XDG_DATA_HOME");
    }
}

TEST(STU_GRAF, AppDirBlockedByFileFailsCleanly)
{
    // Reproduces the AppImage portable-mode collision: a regular FILE occupies the
    // name the app directory needs.  Expected: no crash, no write, a clean NULL.
    TempTree data("appdir_blocked");
    STU_GRAF_Init(STU_GRAF_PLAYER);
    set_env("XDG_DATA_HOME", data.dir().c_str());
    set_env("REMOM_APP_DIR", "remom_app_dir");

    fs::path blocker = fs::path(data.dir()) / "remom_app_dir";
    std::ofstream(blocker.string(), std::ios::binary) << "not a directory";
    ASSERT_TRUE(fs::is_regular_file(blocker));

    EXPECT_EQ(STU_GRAF_Open_User("T.DAT", "wb"), nullptr);
    EXPECT_TRUE(fs::is_regular_file(blocker));  // untouched

    STU_GRAF_Reset();
    unset_env("REMOM_APP_DIR");
    unset_env("XDG_DATA_HOME");
}

TEST(STU_GRAF, AppDirBlockedByFileIsDiagnosed)
{
    // Regression test for graf_mkdir_one treating EEXIST as success.  Note that the
    // test above passes EITHER WAY -- STU_GRAF_Open_User returns NULL regardless,
    // because the doomed fopen() just fails with ENOTDIR instead.  The whole point
    // of the fix is that the cause stops being silent, so the LOG LINE is the only
    // thing that actually distinguishes fixed from broken.  Assert on it.
    TempTree data("appdir_blocked_log");
    STU_GRAF_Init(STU_GRAF_PLAYER);
    set_env("XDG_DATA_HOME", data.dir().c_str());
    set_env("REMOM_APP_DIR", "remom_app_dir");

    std::ofstream((fs::path(data.dir()) / "remom_app_dir").string(), std::ios::binary) << "x";

    std::remove("remom_log_new.txt");
    STU_Log_Startup(NULL);
    EXPECT_EQ(STU_GRAF_Open_User("T.DAT", "wb"), nullptr);
    STU_Log_Shutdown();

    std::ifstream  lf("remom_log_new.txt", std::ios::in | std::ios::binary);
    std::ostringstream ss;
    ss << lf.rdbuf();
    EXPECT_NE(ss.str().find("a non-directory of that name already exists"), std::string::npos)
        << "graf_mkdir_one must report EEXIST-on-a-regular-file as failure, not as a created directory";

    std::remove("remom_log_new.txt");
    STU_GRAF_Reset();
    unset_env("REMOM_APP_DIR");
    unset_env("XDG_DATA_HOME");
}

#endif

// ---- Phase 5: data-compatibility pass (embedded manifest table) --------------
// Search-path based (not user-data), so these are platform-independent.  The
// runtime uses the compiled-in g_lbx_manifest; tests drive the core
// (STU_GRAF_Check_Compat_Against) with a synthetic in-memory manifest, while
// still writing the fixture .LBX files a search dir so they get hashed.

namespace
{
    std::string hex_of(const std::string & content)
    {
        char out[65] = {0};
        STU_SHA256_Bytes(content.data(), content.size(), out);
        return std::string(out);
    }
}

TEST(STU_GRAF, CompatSupportedMatchIsSilent)
{
    TempTree t("compat_ok");
    t.write("FONTS.LBX", "font-bytes-v131");
    std::string h = hex_of("font-bytes-v131");
    STU_LBX_Manifest_Entry m[] = {
        { "FONTS.LBX", h.c_str(), "v1.31-floppy-1995" },
        { nullptr, nullptr, nullptr }
    };

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    STU_GRAF_Compat_Report r;
    EXPECT_EQ(STU_GRAF_Check_Compat_Against(m, &r), 0);
    EXPECT_EQ(r.manifest_found, 1);
    EXPECT_EQ(r.files_checked, 1);
    EXPECT_EQ(r.ok_count, 1);
    EXPECT_EQ(r.problem_count, 0);
    STU_GRAF_Reset();
}

TEST(STU_GRAF, CompatUnrecognizedHashWarns)
{
    TempTree t("compat_unk");
    t.write("MYSTERY.LBX", "modified-or-unknown");
    // Manifest lists a different (all-zero) hash -> no match -> unrecognized.
    STU_LBX_Manifest_Entry m[] = {
        { "MYSTERY.LBX",
          "0000000000000000000000000000000000000000000000000000000000000000",
          "v1.31-floppy-1995" },
        { nullptr, nullptr, nullptr }
    };

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    STU_GRAF_Compat_Report r;
    EXPECT_EQ(STU_GRAF_Check_Compat_Against(m, &r), 1);
    EXPECT_EQ(r.ok_count, 0);
    EXPECT_EQ(r.problem_count, 1);
    EXPECT_NE(std::string(r.summary).find("MYSTERY.LBX"), std::string::npos);
    EXPECT_NE(std::string(r.summary).find("unrecognized"), std::string::npos);
    STU_GRAF_Reset();
}

TEST(STU_GRAF, CompatUnsupportedVersionWarns)
{
    TempTree t("compat_old");
    t.write("OLD.LBX", "v1.0-content");
    std::string h = hex_of("v1.0-content");
    // Actual hash IS in the manifest, but tagged as an unsupported version.
    STU_LBX_Manifest_Entry m[] = {
        { "OLD.LBX", h.c_str(), "v1.0-floppy" },
        { nullptr, nullptr, nullptr }
    };

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    STU_GRAF_Compat_Report r;
    EXPECT_EQ(STU_GRAF_Check_Compat_Against(m, &r), 1);
    EXPECT_EQ(r.ok_count, 0);
    EXPECT_EQ(r.problem_count, 1);
    EXPECT_NE(std::string(r.summary).find("OLD.LBX"), std::string::npos);
    EXPECT_NE(std::string(r.summary).find("version"), std::string::npos);
    STU_GRAF_Reset();
}

TEST(STU_GRAF, CompatMultiDistroPicksSupported)
{
    TempTree t("compat_multi");
    t.write("FONTS.LBX", "canonical-v131");
    std::string h = hex_of("canonical-v131");
    // Two rows for one file: a non-matching cdrom row + the matching floppy row.
    STU_LBX_Manifest_Entry m[] = {
        { "FONTS.LBX",
          "1111111111111111111111111111111111111111111111111111111111111111",
          "v1.31-cdrom" },
        { "FONTS.LBX", h.c_str(), "v1.31-floppy-1995" },
        { nullptr, nullptr, nullptr }
    };

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    STU_GRAF_Compat_Report r;
    EXPECT_EQ(STU_GRAF_Check_Compat_Against(m, &r), 0);
    EXPECT_EQ(r.ok_count, 1);
    STU_GRAF_Reset();
}

TEST(STU_GRAF, CompatEmptyManifestIsSilentNoop)
{
    TempTree t("compat_none");
    t.write("FONTS.LBX", "whatever");
    // Empty manifest (just the terminator) -- as shipped before authoring.
    STU_LBX_Manifest_Entry m[] = {
        { nullptr, nullptr, nullptr }
    };

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    STU_GRAF_Compat_Report r;
    EXPECT_EQ(STU_GRAF_Check_Compat_Against(m, &r), 0);
    EXPECT_EQ(r.manifest_found, 0);
    EXPECT_EQ(r.files_checked, 0);
    STU_GRAF_Reset();
}

TEST(STU_GRAF, CompatUninstalledManifestFileNotCounted)
{
    TempTree t("compat_ghost");
    t.write("FONTS.LBX", "present-ok");
    std::string h1 = hex_of("present-ok");
    std::string h2 = hex_of("does-not-exist");
    // Manifest references a present file (ok) and a GHOST that isn't on disk.
    STU_LBX_Manifest_Entry m[] = {
        { "FONTS.LBX", h1.c_str(), "v1.31-floppy-1995" },
        { "GHOST.LBX", h2.c_str(), "v1.31-floppy-1995" },
        { nullptr, nullptr, nullptr }
    };

    STU_GRAF_Reset();
    STU_GRAF_Add_Search_Dir(t.dir().c_str());

    STU_GRAF_Compat_Report r;
    EXPECT_EQ(STU_GRAF_Check_Compat_Against(m, &r), 0);
    EXPECT_EQ(r.files_checked, 1);  // GHOST.LBX absent -> not counted
    EXPECT_EQ(r.ok_count, 1);
    STU_GRAF_Reset();
}

TEST(STU_GRAF, ShippedManifestIsWellFormed)
{
    // Guard the committed g_lbx_manifest against a botched paste/generation:
    // every row must carry a name, a 64-hex SHA-256, and a version tag.
    int n = 0;
    for(const STU_LBX_Manifest_Entry * e = g_lbx_manifest; e->name != nullptr; ++e)
    {
        ++n;
        ASSERT_NE(e->sha256, nullptr) << e->name << ": null hash";
        std::string h(e->sha256);
        EXPECT_EQ(h.size(), 64u) << e->name << ": sha256 not 64 chars";
        EXPECT_EQ(h.find_first_not_of("0123456789abcdefABCDEF"), std::string::npos)
            << e->name << ": sha256 has a non-hex char";
        EXPECT_NE(e->version, nullptr) << e->name << ": null version tag";
    }
    EXPECT_GT(n, 0) << "shipped manifest is empty";
}

// ---- Auto-detect: first candidate dir that looks like a MoM install ----------
// (a "MoM install" = a dir containing the signature FONTS.LBX)

TEST(STU_GRAF, FirstGameDataDirPicksSignatureDir)
{
    TempTree a("autodet_a");
    TempTree b("autodet_b");
    b.write("FONTS.LBX", "x");                    // only b looks like an install
    std::string da = a.dir(), db = b.dir();       // keep the strings alive
    const char * cands[] = { da.c_str(), db.c_str(), nullptr };
    EXPECT_STREQ(STU_GRAF_First_Game_Data_Dir(cands), db.c_str());
}

TEST(STU_GRAF, FirstGameDataDirFirstMatchWins)
{
    TempTree a("autodet_fa");
    TempTree b("autodet_fb");
    a.write("FONTS.LBX", "x");
    b.write("FONTS.LBX", "y");
    std::string da = a.dir(), db = b.dir();
    const char * cands[] = { da.c_str(), db.c_str(), nullptr };
    EXPECT_STREQ(STU_GRAF_First_Game_Data_Dir(cands), da.c_str());
}

TEST(STU_GRAF, FirstGameDataDirNoneOrEmptyReturnsNull)
{
    TempTree a("autodet_none");                   // no FONTS.LBX
    std::string da = a.dir();
    const char * cands[] = { da.c_str(), nullptr };
    EXPECT_EQ(STU_GRAF_First_Game_Data_Dir(cands), nullptr);

    const char * empty[] = { nullptr };
    EXPECT_EQ(STU_GRAF_First_Game_Data_Dir(empty), nullptr);
    EXPECT_EQ(STU_GRAF_First_Game_Data_Dir(nullptr), nullptr);
}
