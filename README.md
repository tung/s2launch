s2launch - Simple game launcher GUI for Sphere 2
================================================

s2launch remembers and lets you choose games to play with the [Sphere 2](https://github.com/kyuu/sphere) game engine.

1. Tell s2launch the Sphere 2 engine location.
2. Click 'Add' to add a game to the games list.
3. Choose a game from the list and click 'Play'.

Visit the [project page](https://github.com/tung/s2launch) and [forum thread](http://spheredev.org/smforums/index.php?topic=8004.0) for updates.

Building s2launch - Windows
---------------------------

1. Download and install MinGW and MSys.
2. Download [Phoenix 2011-12-13](http://byuu.org/phoenix/).
3. Copy `phoenix` to `C:\MinGW\include`.
4. Copy `phoenix\nall` to `C:\MinGW\include`.
5. Build Phoenix using its [tutorial](http://byuu.org/phoenix/tutorial-compilation).
6. Ensure/move `phoenix-resource.o` to `C:\MinGW\include\phoenix\windows`.
7. Ensure/move `phoenix.o` to `C:\MinGW\include\phoenix`.
8. Build `s2launch.exe` by entering `make -f Makefile.win`.

Building s2launch - Linux
-------------------------

1. Install a C++ compiler and `libgtk2.0-dev` (GTK+ 3.0 won't work).
2. Download [Phoenix 2011-12-13](http://byuu.org/phoenix/).
3. Copy `phoenix` to `/usr/local/include`.
4. Symlink `/usr/local/nall` to `/usr/local/phoenix/nall`.
5. Build Phoenix using its [tutorial](http://byuu.org/phoenix/tutorial-compilation). Do this in a copy outside of `/usr/local/include` to avoid permission issues.
6. Copy `phoenix.o` to `/usr/local/include/phoenix`.
7. Build `s2launch` by entering `make -f Makefile.linux`.

License
-------

Copyright (c) 2011, Tung Nguyen

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
