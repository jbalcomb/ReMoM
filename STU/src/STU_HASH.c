/*
    STU_HASH -- SHA-256 (FIPS 180-4) implementation.

    A textbook, portable, dependency-free implementation.  Not constant-time and
    not meant for adversarial/secret data -- it exists to fingerprint game asset
    files, where correctness against the published test vectors is all that matters.
    Validated in STU/tests/test_STU_HASH.cpp against the standard "abc" / empty-string / long-message vectors.
*/

#include "STU_HASH.h"

#include <string.h>
#include <stdint.h>

typedef struct
{
    uint32_t state[8];
    uint64_t total_len;   /* total message bytes fed so far */
    uint8_t  buffer[64];
    size_t   buflen;      /* bytes currently buffered (0..63) */
} sha256_ctx;

static const uint32_t SHA256_K[64] =
{
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
    0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
    0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
    0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
    0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
    0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
    0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
    0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u
};

#define ROTR(x, n)  (((x) >> (n)) | ((x) << (32 - (n))))
#define CH(x, y, z)  (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BSIG0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define BSIG1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SSIG0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define SSIG1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

static void sha256_init(sha256_ctx * ctx)
{
    ctx->state[0] = 0x6a09e667u;
    ctx->state[1] = 0xbb67ae85u;
    ctx->state[2] = 0x3c6ef372u;
    ctx->state[3] = 0xa54ff53au;
    ctx->state[4] = 0x510e527fu;
    ctx->state[5] = 0x9b05688cu;
    ctx->state[6] = 0x1f83d9abu;
    ctx->state[7] = 0x5be0cd19u;
    ctx->total_len = 0;
    ctx->buflen = 0;
}

static void sha256_transform(sha256_ctx * ctx, const uint8_t * p)
{
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t1, t2;
    int i;

    for(i = 0; i < 16; i++)
    {
        w[i] = ((uint32_t)p[i * 4] << 24) | ((uint32_t)p[i * 4 + 1] << 16) |
               ((uint32_t)p[i * 4 + 2] << 8) | ((uint32_t)p[i * 4 + 3]);
    }
    for(i = 16; i < 64; i++)
    {
        w[i] = SSIG1(w[i - 2]) + w[i - 7] + SSIG0(w[i - 15]) + w[i - 16];
    }

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for(i = 0; i < 64; i++)
    {
        t1 = h + BSIG1(e) + CH(e, f, g) + SHA256_K[i] + w[i];
        t2 = BSIG0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

static void sha256_update(sha256_ctx * ctx, const uint8_t * data, size_t len)
{
    size_t i;
    for(i = 0; i < len; i++)
    {
        ctx->buffer[ctx->buflen++] = data[i];
        ctx->total_len++;
        if(ctx->buflen == 64)
        {
            sha256_transform(ctx, ctx->buffer);
            ctx->buflen = 0;
        }
    }
}

static void sha256_final(sha256_ctx * ctx, uint8_t out[32])
{
    uint64_t bitlen = ctx->total_len * 8u;
    size_t i = ctx->buflen;
    int j;

    /* Append the 0x80 terminator, then pad with zeros, leaving room for the 64-bit big-endian length in the final block. */
    ctx->buffer[i++] = 0x80;
    if(i > 56)
    {
        while(i < 64)
        {
            ctx->buffer[i++] = 0x00;
        }
        sha256_transform(ctx, ctx->buffer);
        i = 0;
    }
    while(i < 56)
    {
        ctx->buffer[i++] = 0x00;
    }
    for(j = 7; j >= 0; j--)
    {
        ctx->buffer[i++] = (uint8_t)(bitlen >> (j * 8));
    }
    sha256_transform(ctx, ctx->buffer);

    for(i = 0; i < 8; i++)
    {
        out[i * 4]     = (uint8_t)(ctx->state[i] >> 24);
        out[i * 4 + 1] = (uint8_t)(ctx->state[i] >> 16);
        out[i * 4 + 2] = (uint8_t)(ctx->state[i] >> 8);
        out[i * 4 + 3] = (uint8_t)(ctx->state[i]);
    }
}

static void sha256_hex(const uint8_t digest[32], char out_hex[65])
{
    static const char hexd[] = "0123456789abcdef";
    int i;
    for(i = 0; i < 32; i++)
    {
        out_hex[i * 2]     = hexd[(digest[i] >> 4) & 0x0f];
        out_hex[i * 2 + 1] = hexd[digest[i] & 0x0f];
    }
    out_hex[64] = '\0';
}

void STU_SHA256_Bytes(const void * data, size_t len, char out_hex[65])
{
    sha256_ctx ctx;
    uint8_t digest[32];

    sha256_init(&ctx);
    sha256_update(&ctx, (const uint8_t *)data, len);
    sha256_final(&ctx, digest);
    sha256_hex(digest, out_hex);
}

int STU_SHA256_Stream(FILE * fp, char out_hex[65])
{
    sha256_ctx ctx;
    uint8_t digest[32];
    uint8_t buf[4096];
    size_t n;

    if(fp == NULL)
    {
        return 0;
    }

    sha256_init(&ctx);
    while((n = fread(buf, 1, sizeof(buf), fp)) > 0)
    {
        sha256_update(&ctx, buf, n);
    }
    if(ferror(fp))
    {
        return 0;
    }
    sha256_final(&ctx, digest);
    sha256_hex(digest, out_hex);
    return 1;
}
