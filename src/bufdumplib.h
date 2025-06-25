// Copyright 2025 Michael Reilly <mreilly@mreilly.dev>
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the names of the copyright holders nor the names of the
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef _BUFDUMPLIB_H_
#define _BUFDUMPLIB_H_

/**
 * @file bufdumplib.h
 *
 * @brief This library provides a C API for hexadecimal dumps and diffs of
 *        binary memory buffers.
 */

/**
 * @brief Write a hex dump of a binary memory buffer over the specified length
 *        into a file pointer.
 *
 * @param fp FILE pointer to which to write output
 * @param buf Buffer data to dump
 * @param len Length of buffer
 * @param flags
 *        - 1 skips all-zero lines (of 16 bytes)
 *        - 2 skips all-zero groups (of 4 bytes)
 *        - 4 skips individual zero bytes
 *
 * @return On success, count of non-skipped bytes printed. On error, a negative
 *         value.
 */
int bufdumplib_dump(FILE *fp, const void *buf, size_t len, int flags);

/**
 * @brief Write a hex diff of two binary memory buffers over the specified
 *        length into a file pointer.
 *
 * @param fp FILE pointer to which to write output
 * @param b0 First buffer to compare (output on left)
 * @param b1 Second buffer to compare (output on right)
 * @param len Length of buffer
 * @param flags
 *        - 1 skips identical lines (of 16 bytes)
 *        - 2 skips identical groups (of 4 bytes)
 *        - 4 skips identical bytes
 *
 * @return On success, count of non-skipped bytes printed. On error, a negative
 *         value.
 */
int bufdumplib_diff(FILE *fp, const void *b0, const void *b1, size_t len,
                    int flags);

/**
 * @brief Write a comma-delimited hex byte array (e.g. for use in C source)
 *        into a file pointer.
 *
 * @param fp FILE pointer to which to write output
 * @param buf Buffer data to dump
 * @param len Length of buffer
 *
 * @return On success, count of byte array. On error, a negative value.
 */
int bufdumplib_text(FILE *fp, const void *buf, size_t len);

#endif
