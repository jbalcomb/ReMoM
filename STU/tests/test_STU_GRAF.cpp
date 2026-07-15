// Tests for STU_GRAF asset-resolution (Phase 1: read-only search path).
//
// Boundary under test: "given a search path and a file laid out in some dir,
// STU_GRAF_Open_Asset opens the first match; not-found returns NULL." We drive
// the low-level Reset/Add_Search_Dir API so the test is deterministic and does
// not depend on the executable location or environment.

#include <gtest/gtest.h>

#include <cstdio>
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
