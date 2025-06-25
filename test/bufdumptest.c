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
#include <assert.h>

#include "bufdumplib.h"

int main(int argc, char **argv)
{
    char const str[] =
"v@aa#OCW]  F#lZet2QI0a hCv$:gn}sY-BGwAe}6WP 0=:83@uQXPwo\n"
"Roundhea dhawkbillquot afootboar dbullfightan tiarlexicostatistics\n"
"jD)41R1 Efb>y=mUu)FPh{g tlN'W^,QvK@ZaF\"07g 7(]J&gZY9dQV1i\\]Zwe0<na\n"
"(|=l1)+'Ry\\@v5r,-:hyY3/   yXXU&i9@6$?y ?S`Q-xfObR>mBb&we\";!0|\n"
"mulley MachmetersalarynunatakHakluyt\n"
"V|VO7 dtU+ADit!p^IO87Ub;taa ];)p?vhM$9`[pYL7Z5saR\n"
"tp8*T/lxoL8Hz'),i)hR-3l<q*sU,h@&Mq{p\n"
"winklesyllabik insfolkseti formalgorism\n"
"}RFG9G4JYLC4` 6MUxb=W$qtyJdlldvV i)x5bqwLA\n"
"alcahestafar opportunisthogansc hizopodsorcererphosphorescent\n"
"zA4\"p:<r*=B 5ZtSn-/+QkL[7F7mfuo}80\n";

    // Write dump of fixed data to stdout - calling script verifies results.
    assert(bufdumplib_dump(stdout, str, sizeof str - 1, 0) == 557);
}
