/*
    STU_HASH -- small, self-contained cryptographic-hash helpers.

    Currently just SHA-256 (FIPS 180-4).  No external dependency -- the STU
    layer stays libc-only -- so it links into every target without pulling in
    OpenSSL/libcrypto.  Used by STU_GRAF's data-compatibility pass to fingerprint
    the original game's .LBX files against a known-good manifest.
*/

#ifndef STU_HASH_H
#define STU_HASH_H

#include <stddef.h>  /* size_t */
#include <stdio.h>   /* FILE   */

#ifdef __cplusplus
extern "C" {
#endif

/* Lower-case hex SHA-256 digest (64 chars + NUL) of an in-memory buffer. */
void STU_SHA256_Bytes(const void * data, size_t len, char out_hex[65]);

/* Lower-case hex SHA-256 digest of a stream, read to EOF from the current
   position.  Returns 1 on success, 0 on a read error.  Does not close fp. */
int STU_SHA256_Stream(FILE * fp, char out_hex[65]);

#ifdef __cplusplus
}
#endif

#endif /* STU_HASH_H */
