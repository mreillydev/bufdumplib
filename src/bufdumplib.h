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
 *
 * **Examples**\n 
Given the following C data buffer:\n 
\code{.c}
    uint8_t const buf[] = {
0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,0xde,0xad,0xfa,0xce,   0,   0,   0,   0,   0,   0,   0,0xde,0xad,0xfa,0xce,
    };
\endcode

bufdumplib_dump(stdout, buf, sizeof buf, **0**) with **flags=0** outputs:\n 
\code{.c}
000000: feedface deadbeef 00000000 00000000
000010: 00000000 00000000 00000000 00000000
000020: 00000000 00000000 00000000 00000000
000030: 00deadfa ce000000 00000000 deadface
\endcode

bufdumplib_dump(stdout, buf, sizeof buf, **1**) with **flags=1** skips the first
and second lines since they contain only zeros:\n 
\code{.c}
000000: feedface deadbeef 00000000 00000000
000030: 00deadfa ce000000 00000000 deadface
\endcode

bufdumplib_dump(stdout, buf, sizeof buf, **2**) with **flags=2** blanks the
4 byte (32 bit) zero groups:\n 
\code{.c}
000000: feedface deadbeef ........ ........
000010: ........ ........ ........ ........
000020: ........ ........ ........ ........
000030: 00deadfa ce000000 ........ deadface
\endcode

bufdumplib_dump(stdout, buf, sizeof buf, **4**) with **flags=4** blanks the
remaining individual zero bytes in line 4 that the previous example left in
numeric form:\n 
\code{.c}
000000: feedface deadbeef ........ ........
000010: ........ ........ ........ ........
000020: ........ ........ ........ ........
000030: ..deadfa ce...... ........ deadface
\endcode

bufdumplib_dump(stdout, buf, sizeof buf, **3**) with **flags=3** blanks lines
and groups that are zero, but preserves the individual zero bytes on line 4:\n 
\code{.c}
000000: feedface deadbeef ........ ........
000030: 00deadfa ce000000 ........ deadface
\endcode

This example can be found in examples/dumpexample.c
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
 *
 * **Examples**\n 
Now consider a new buffer with a few changed bytes:\n 
\code{.c}
    uint8_t const chg[] = {
0xfe,0xed,0xfa,0xce,0xbe,0xef,0xbe,0xef,   0,   0,   0,   0,   0,   0,   0,   0,
//                    ^^   ^^
   0,   0,0x03,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//          ^^
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,0xde,0xad,0xfa,0xce,   0,   0,   0,   0,   0,   0,   0,0xde,0xad,0xfa,0xce,
    };
\endcode

bufdumplib_diff(stdout, buf, chg, sizeof buf, **0**) with **flags=0** outputs a
straight comparison with everything intact:\n 
\code{.c}
000000: feedface deadbeef 00000000 00000000|feedface beefbeef 00000000 00000000
000010: 00000000 00000000 00000000 00000000|00000300 00000000 00000000 00000000
000020: 00000000 00000000 00000000 00000000|00000000 00000000 00000000 00000000
000030: 00deadfa ce000000 00000000 deadface|00deadfa ce000000 00000000 deadface
\endcode

bufdumplib_diff(stdout, buf, chg, sizeof buf, **1**) with **flags=1** skips the
second and third lines since they are the same:\n
\code{.c}
000000: feedface deadbeef 00000000 00000000|feedface beefbeef 00000000 00000000
000010: 00000000 00000000 00000000 00000000|00000300 00000000 00000000 00000000
\endcode

bufdumplib_diff(stdout, buf, chg, sizeof buf, **2**) with **flags=2** blanks the
4 byte (32 bit) groups that are the same in each buffer:\n 
\code{.c}
000000: ........ deadbeef ........ ........|........ beefbeef ........ ........
000010: 00000000 ........ ........ ........|00000300 ........ ........ ........
000020: ........ ........ ........ ........|........ ........ ........ ........
000030: ........ ........ ........ ........|........ ........ ........ ........
\endcode

bufdumplib_diff(stdout, buf, chg, sizeof buf, **4**) with **flags=4** blanks the
remaining individual zero bytes in lines 0 and 1 that the previous example left
in numeric form:\n 
\code{.c}
000000: ........ dead.... ........ ........|........ beef.... ........ ........
000010: ....00.. ........ ........ ........|....03.. ........ ........ ........
000020: ........ ........ ........ ........|........ ........ ........ ........
000030: ........ ........ ........ ........|........ ........ ........ ........
\endcode

bufdumplib_diff(stdout, buf, chg, sizeof buf, **7**) with **flags=7** (or **5**)
blanks lines and bytes (and groups) that are the same:\n 
\code{.c}
000000: ........ dead.... ........ ........|........ beef.... ........ ........
000010: ....00.. ........ ........ ........|....03.. ........ ........ ........
\endcode

This example can be found in examples/diffexample.c

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
 *
 * **Example**\n 
Given the same C data buffer as above:\n 
\code{.c}
    uint8_t const buf[] = {
0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,0xde,0xad,0xfa,0xce,   0,   0,   0,   0,   0,   0,   0,0xde,0xad,0xfa,0xce,
    };
\endcode
bufdumplib_text(stdout, buf, sizeof buf) outputs:\n 
\code{.c}
0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xde,0xad,0xfa,0xce,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xde,0xad,0xfa,0xce
\endcode

This example can be found in examples/textexample.c

 */
int bufdumplib_text(FILE *fp, const void *buf, size_t len);

#endif
