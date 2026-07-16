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
#include <string>

extern "C" {
#include "../src/STU_GRAF.h"
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

#endif
