# bufdumplib

bufdumplib provides a C API for hexadecimal dumps and diffs of binary memory buffers. It supports xxd (or hexpeek :)) style output as well as output usable in a C source file. An optional flags parameter allows skipping lines, 32 bit sized groups, or individual bytes that are zero (or identical in case of diffs).

# Platforms

This software is tested on 64 bit Debian and FreeBSD.

# API Documentation & Examples

[Doxygen header documentation](https://mreillydev.github.io/bufdumplib/bufdumplib_8h.html)

# Testing

A test suite is provided in the test/ directory. Advanced tests require [hexpeek](https://github.com/mreillydev/hexpeek) installed in your PATH.

# License

Licensed under the BSD-3 License.

# Author

This software was created by Michael Reilly (mreilly@mreilly.dev)
