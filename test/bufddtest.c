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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "bufdumplib.h"

int main(int argc, char **argv)
{
    int what = 0, flags = 0;

    // At least one argument required
    assert(argc >= 2);

    // Check for bitflag argument (can be -0 through -7)
    if(argv[1][0] == '-' && argv[1][1] >= '0' && argv[1][1] <= '7')
    {
        flags = atoi(argv[1]+1);
        argc--;
        argv++;
    }

    // Determine command argument
    assert(argc >= 2);
    if(strcmp(argv[1], "-dump") == 0)
        what = 1;
    else if(strcmp(argv[1], "-diff") == 0)
        what = 2;
    else if(strcmp(argv[1], "-text") == 0)
        what = 3;
    assert(what);
    assert(argc == 4 - what%2);

    // Storage for file descriptions, specified lengths, and data
    int fds[2] = { -1, -1 };
    ssize_t lns[2] = { -1, -1 };
    char bfs[2][1048576];
    memset(bfs, 0, sizeof bfs);

    for(int ix = 0; 2+ix < argc; ix++)
    {
        fds[ix] = open(argv[2+ix], O_RDONLY);
        assert(fds[ix] >= 0);
        lns[ix] = read(fds[ix], bfs[ix], sizeof bfs[ix]);
        assert(lns[ix] > 0);
        assert(lns[ix] < sizeof bfs[ix]);
    }

    // Determine action and proceed to API call. Write data to stdout - calling
    // script verifies results.
    if(what == 1)
    {
        int rc = bufdumplib_dump(stdout, bfs[0], lns[0], flags);
        if(flags == 0)
            assert(rc == lns[0]);
    }
    else if(what == 2)
    {
        assert(lns[0] == lns[1]);
        bufdumplib_diff(stdout, bfs[0], bfs[1], lns[0], flags);
    }
    else if(what == 3)
    {
        assert(flags == 0);
        assert(bufdumplib_text(stdout, bfs[0], lns[0]) == lns[0]);
    }

    exit(0);
}
