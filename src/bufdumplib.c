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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "bufdumplib.h"

#define Width 6
#define Cols 16
#define Group 4

#ifndef MIN
    #define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

// Return if ptr is fully zero over the specific length
static int iszero(const uint8_t *ptr, size_t len)
{
    for(int i = 0; i < len; i++)
        if(ptr[i])
            return 0;
    return 1;
}

// Skip zero or identical (depending on mode) memory segments
static int skip(int laps, int index, uint8_t *b0, uint8_t *b1, size_t len)
{
    if(laps == 1 && iszero(b0+index, len))
        return 1;
    if(laps == 2 && ! memcmp(b0+index, b1+index, len))
        return 1;
    return 0;
}

// Conditionally call fprintf() unless previous call failed
static void fpf(int *ec, FILE *fp, char const *fmt, ...)
{
    if(*ec < 0)
        return; // don't try fprintf again if failed
    va_list vl;
    va_start(vl, fmt);
    int rc = vfprintf(fp, fmt, vl);
    va_end(vl);
    if(rc < 0)
        *ec = rc;
}

// See header for documentation
// If fprintf() fails, stop printing but continue reading through the buffers
// for operational consistency.
static int bufshow(FILE *fp, const void *_b0, const void *_b1, size_t len,
                   int flags)
{
    if( ! fp)
        return -1000;
    if( ! _b0)
        return -1001;

    uint8_t *b0 = (uint8_t*)_b0;
    uint8_t *b1 = (uint8_t*)_b1;
    int ec = 0, count = 0, laps = (b1 ? 2 : 1);

    for(int i = 0; i < len; i += Cols)
    {
        // Check for line skip
        if((flags&1) && skip(laps, i, b0, b1, MIN(Cols, len-i)))
            continue;

        // Print relative offset
        fpf(&ec, fp, "%0*x:", Width, i);

        // Iterate over both buffers if provided
        for(int b = 0; b < laps; b++)
        {
            uint8_t *buf = (b ? b1 : b0);
            int j = 0;
            for(int k = 0, blank = 0; j < Cols && (k=i+j) < len; j++)
            {
                if(j % Group == 0)
                {
                    // Check for group skip; if so, set blank flag for below
                    blank = 0;
                    if((flags&2) && skip(laps, k, b0, b1, MIN(Group, len-k)))
                        blank = 1;
                    // Print separator for diff
                    if(j == 0 && b == 1)
                        fpf(&ec, fp, "|");
                    else
                        fpf(&ec, fp, " ");
                }
                if(blank || ((flags&4) && skip(laps, k, b0, b1, 1)))
                {
                    // Print blank bytes for group or byte skip
                    fpf(&ec, fp, "..");
                }
                else
                {
                    // Print actual non-skipped bytes
                    fpf(&ec, fp, "%02x", buf[k]);
                    if(b == 0)
                        count++;
                }
            }
            // Pad with spaces for diff alignment of last line
            if(laps == 2 && b == 0)
            {   
                for( ; j < Cols; j++)
                    fpf(&ec, fp, "%s  ", (j % Group == 0 ? " " : ""));
            }
        }
 
        // One newline per line
        fpf(&ec, fp, "\n");
    }

    return (ec < 0 ? ec : count);
}

// Hex print one buffer
int bufdumplib_dump(FILE *fp, const void *buf, size_t len, int flags)
{
    return bufshow(fp, buf, NULL, len, flags);
}

// Hex diff two buffers
int bufdumplib_diff(FILE *fp, const void *b0, const void *b1, size_t len,
                       int flags)
{
    return bufshow(fp, b0, b1, len, flags);
}

// Print a buffer into a comma-delimited hex byte array
// If fprintf() fails, stop printing but continue reading through the buffers
// for operational consistency.
int bufdumplib_text(FILE *fp, const void *buf, size_t len)
{
    if( ! fp)
        return -1000;

    int ec = 0, count = 0;
    while(count < len)
    {
        fpf(&ec, fp, "0x%02x", ((uint8_t*)buf)[count]);
        count++;
        if(count != len)
            fpf(&ec, fp, ",");
        if(count % Cols == 0 || count == len)
            fpf(&ec, fp, "\n");
    }

    return (ec < 0 ? ec : count);
}
