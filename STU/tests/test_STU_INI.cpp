// Tests for STU_INI: keeping a player's settings file up to date (doc/@AI_Plans/PLAN-ReMoM-Ini.md).

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

extern "C" {
#include "../src/STU_INI.h"
}

namespace fs = std::filesystem;

namespace
{
    class TempDir
    {
    public:
        explicit TempDir(const std::string & tag)
        {
            root_ = fs::temp_directory_path() / ("stu_ini_" + tag);
            fs::remove_all(root_);
            fs::create_directories(root_);
        }
        ~TempDir() { std::error_code ec; fs::remove_all(root_, ec); }

        std::string path(const std::string & name) const { return (root_ / name).string(); }

        void write(const std::string & name, const std::string & content) const
        {
            std::ofstream(root_ / name, std::ios::binary) << content;
        }
        std::string read(const std::string & name) const
        {
            std::ifstream in(root_ / name, std::ios::binary);
            std::stringstream ss;
            ss << in.rdbuf();
            return ss.str();
        }
        int count_backups() const
        {
            int n = 0;
            for(const auto & entry : fs::directory_iterator(root_))
            {
                if(entry.path().filename().string().find("_backup_") != std::string::npos) { n++; }
            }
            return n;
        }

    private:
        fs::path root_;
    };

    const STU_INI_Setting kSettings[] =
    {
        { "Paths", "game_data", "D:\\Games\\Master of Magic", "Where your game files are.", 1 },
        { "Video", "startup_window", "fullscreen", "fullscreen | windowed", 0 },
        { "Video", "scale_mode", "integer", "integer | fit_nearest | fit_sharp_bilinear\nHow the picture is enlarged.", 0 },
    };
    const int kCount = (int)(sizeof(kSettings) / sizeof(kSettings[0]));
}

TEST(STU_INI, HasKeyLiveAndCommented)
{
    const char * text =
        "[Video]\n"
        "startup_window = windowed\n"
        "; scale_mode = integer\n"
        "#aspect_ratio=4:3\n"
        "[Paths]\n"
        "log_dir = .\n";
    EXPECT_EQ(STU_INI_Text_Has_Key(text, "Video", "startup_window"), 1);
    EXPECT_EQ(STU_INI_Text_Has_Key(text, "video", "SCALE_MODE"), 1);
    EXPECT_EQ(STU_INI_Text_Has_Key(text, "Video", "aspect_ratio"), 1);
    EXPECT_EQ(STU_INI_Text_Has_Key(text, "Video", "log_dir"), 0);    // wrong section
    EXPECT_EQ(STU_INI_Text_Has_Key(text, "Paths", "log_dir"), 1);
    EXPECT_EQ(STU_INI_Text_Has_Key(text, "Paths", "game_data"), 0);
}

TEST(STU_INI, AddMissingAppendsToExistingSectionAndNewSection)
{
    TempDir t("addmissing");
    t.write("ReMoM.ini", "; my notes\n[Video]\nstartup_window = windowed\n\n[Logging]\nPFL = false\n");

    char backup[1024] = { 0 };
    int added = 0;
    ASSERT_EQ(STU_INI_Add_Missing(t.path("ReMoM.ini").c_str(), kSettings, kCount, backup, sizeof(backup), &added), STU_INI_CHANGED);
    EXPECT_EQ(added, 2);

    const std::string after = t.read("ReMoM.ini");
    // The player's own lines are untouched and in order.
    EXPECT_EQ(after.rfind("; my notes\n[Video]\nstartup_window = windowed\n", 0), 0u) << after;
    // scale_mode goes into [Video], commented out, before [Logging].
    const size_t scale = after.find("; scale_mode = integer\n");
    const size_t logging = after.find("[Logging]");
    ASSERT_NE(scale, std::string::npos) << after;
    EXPECT_LT(scale, logging);
    EXPECT_NE(after.find("; How the picture is enlarged.\n"), std::string::npos);
    // [Paths] did not exist: appended at the end with game_data commented out.
    const size_t paths = after.find("[Paths]");
    ASSERT_NE(paths, std::string::npos);
    EXPECT_GT(paths, logging);
    EXPECT_NE(after.find("; game_data = D:\\Games\\Master of Magic\n", paths), std::string::npos);
    // startup_window was already there: not added a second time.
    EXPECT_EQ(after.find("startup_window", after.find("startup_window") + 1), std::string::npos) << after;

    // The backup holds the file as it was.
    ASSERT_NE(backup[0], '\0');
    EXPECT_NE(std::string(backup).find("ReMoM_backup_"), std::string::npos);
    EXPECT_EQ(std::string(backup).substr(std::string(backup).size() - 4), ".ini");
    EXPECT_EQ(t.count_backups(), 1);
}

TEST(STU_INI, NothingMissingMeansNoBackupAndNoChange)
{
    TempDir t("nochange");
    const std::string body = "[Paths]\n;game_data=x\n[Video]\nstartup_window=windowed\n# scale_mode = fit_nearest\n";
    t.write("ReMoM.ini", body);

    char backup[1024] = { 0 };
    int added = 7;
    EXPECT_EQ(STU_INI_Add_Missing(t.path("ReMoM.ini").c_str(), kSettings, kCount, backup, sizeof(backup), &added), STU_INI_UNCHANGED);
    EXPECT_EQ(added, 0);
    EXPECT_EQ(backup[0], '\0');
    EXPECT_EQ(t.read("ReMoM.ini"), body);
    EXPECT_EQ(t.count_backups(), 0);
}

TEST(STU_INI, SecondRunAddsNothing)
{
    TempDir t("secondrun");
    t.write("ReMoM.ini", "[Video]\n");
    ASSERT_EQ(STU_INI_Add_Missing(t.path("ReMoM.ini").c_str(), kSettings, kCount, nullptr, 0, nullptr), STU_INI_CHANGED);
    const std::string once = t.read("ReMoM.ini");
    EXPECT_EQ(STU_INI_Add_Missing(t.path("ReMoM.ini").c_str(), kSettings, kCount, nullptr, 0, nullptr), STU_INI_UNCHANGED);
    EXPECT_EQ(t.read("ReMoM.ini"), once);
    EXPECT_EQ(t.count_backups(), 1);
}

TEST(STU_INI, KeepsCrlfLineEndings)
{
    TempDir t("crlf");
    t.write("ReMoM.ini", "[Video]\r\nstartup_window = windowed\r\n");
    ASSERT_EQ(STU_INI_Add_Missing(t.path("ReMoM.ini").c_str(), kSettings, kCount, nullptr, 0, nullptr), STU_INI_CHANGED);
    const std::string after = t.read("ReMoM.ini");
    for(size_t i = 0; i < after.size(); i++)
    {
        if(after[i] == '\n') { ASSERT_TRUE(i > 0 && after[i - 1] == '\r') << "bare LF at " << i << " in:\n" << after; }
    }
}

TEST(STU_INI, LastLineWithoutLineEnding)
{
    TempDir t("noeol");
    t.write("ReMoM.ini", "[Video]\nstartup_window = windowed");
    ASSERT_EQ(STU_INI_Add_Missing(t.path("ReMoM.ini").c_str(), kSettings, kCount, nullptr, 0, nullptr), STU_INI_CHANGED);
    const std::string after = t.read("ReMoM.ini");
    EXPECT_NE(after.find("startup_window = windowed\n\n; integer"), std::string::npos) << after;
}

TEST(STU_INI, MissingFileIsAnError)
{
    TempDir t("missingfile");
    EXPECT_EQ(STU_INI_Add_Missing(t.path("nope.ini").c_str(), kSettings, kCount, nullptr, 0, nullptr), STU_INI_ERROR);
    EXPECT_EQ(STU_INI_File_Exists(t.path("nope.ini").c_str()), 0);
}

TEST(STU_INI, ComposeNewFileAndExample)
{
    char * player = STU_INI_Compose("ReMoM settings", kSettings, kCount, 1, "\n");
    ASSERT_NE(player, nullptr);
    const std::string p(player);
    free(player);
    EXPECT_EQ(p.rfind("; ReMoM settings\n", 0), 0u) << p;
    EXPECT_NE(p.find("[Paths]\n\n; Where your game files are.\n; game_data = D:\\Games\\Master of Magic\n"), std::string::npos) << p;
    EXPECT_NE(p.find("; startup_window = fullscreen\n"), std::string::npos);
    EXPECT_NE(p.find("; scale_mode = integer\n"), std::string::npos);
    EXPECT_EQ(p.find("[Video]"), p.rfind("[Video]"));  // one [Video] for both settings

    char * example = STU_INI_Compose(nullptr, kSettings, kCount, 0, "\n");
    ASSERT_NE(example, nullptr);
    const std::string e(example);
    free(example);
    EXPECT_NE(e.find("\nstartup_window = fullscreen\n"), std::string::npos) << e;   // defaults live
    EXPECT_NE(e.find("; game_data = D:\\Games\\Master of Magic\n"), std::string::npos);  // an example value stays commented out
    // Everything a composed player file holds counts as present.
    for(int i = 0; i < kCount; i++)
    {
        EXPECT_EQ(STU_INI_Text_Has_Key(p.c_str(), kSettings[i].section, kSettings[i].key), 1) << kSettings[i].key;
    }
}

TEST(STU_INI, WriteIfDifferent)
{
    TempDir t("writeif");
    const std::string file = t.path("ReMoM_example.ini");
    EXPECT_EQ(STU_INI_Write_If_Different(file.c_str(), "a\n"), STU_INI_CHANGED);
    EXPECT_EQ(STU_INI_Write_If_Different(file.c_str(), "a\n"), STU_INI_UNCHANGED);
    EXPECT_EQ(STU_INI_Write_If_Different(file.c_str(), "b\n"), STU_INI_CHANGED);
    EXPECT_EQ(t.read("ReMoM_example.ini"), "b\n");
}
