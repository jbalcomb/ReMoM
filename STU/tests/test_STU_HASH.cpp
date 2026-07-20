// SHA-256 conformance against the published FIPS 180-4 / NIST test vectors,
// plus a stream-vs-bytes cross-check.  If these pass, the implementation is
// correct; the manifest compatibility pass built on top can be trusted.

#include <gtest/gtest.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>

extern "C" {
#include "../src/STU_HASH.h"
}

namespace fs = std::filesystem;

namespace
{
    std::string sha256_of(const std::string & s)
    {
        char out[65] = {0};
        STU_SHA256_Bytes(s.data(), s.size(), out);
        return std::string(out);
    }
}

TEST(STU_HASH, EmptyStringVector)
{
    EXPECT_EQ(sha256_of(""),
              "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(STU_HASH, AbcVector)
{
    EXPECT_EQ(sha256_of("abc"),
              "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

TEST(STU_HASH, FiftySixByteMessageCrossesBlockBoundary)
{
    // Exactly 56 bytes: the 0x80 pad + length force a second compression block.
    EXPECT_EQ(sha256_of("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
              "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
}

TEST(STU_HASH, OneMillionA_MultiBlock)
{
    std::string msg(1000000, 'a');
    EXPECT_EQ(sha256_of(msg),
              "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
}

TEST(STU_HASH, StreamMatchesBytes)
{
    fs::path p = fs::temp_directory_path() / "stu_hash_stream.bin";
    const std::string content = "The quick brown fox jumps over the lazy dog";
    std::ofstream(p, std::ios::binary) << content;

    FILE * fp = fopen(p.string().c_str(), "rb");
    ASSERT_NE(fp, nullptr);
    char out[65] = {0};
    ASSERT_EQ(STU_SHA256_Stream(fp, out), 1);
    fclose(fp);

    EXPECT_EQ(std::string(out), sha256_of(content));
    // Known vector for this classic sentence.
    EXPECT_EQ(std::string(out),
              "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592");

    std::error_code ec;
    fs::remove(p, ec);
}
