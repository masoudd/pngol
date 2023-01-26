PNGoL: PNG + Game of Life
============
![Example](https://github.com/masoudd/pngol/blob/master/example.gif)

A command line program implementing [Game of Life][1].

This program takes a starting generation from the input and
outputs PNG file images of each successive generations.
These images can be used as frames of a video to show the progress
of an initial state.


How to compile
--------------
You need libpng for compiling
simply run `make`
it will compile into `pngol`


How to use
-----------
Make a directory for pictures:

    mkdir output
    cd output

Generate pictures here from random initial state with default width and height number of cells:

    ../pngol 0 100

This will make `life_00000.png` to `life_00100.png`.

To make an mp4 file from the pictures in current directory (uses ffmpeg):

    ../scripts/png_to_mp4.sh

or see `png_to_gif.sh` to make a gif.

See [ffmpeg documentation][2] for more.


Use an initial state file
-------------------------
A state file is an ascii text file containing `height` number of lines of `width` length made up of only `0` and `1` characters.

A 10x5 example:

    0010000000
    1010000000
    0110000000
    0000000000
    0000000000


Configure
---------
Edit `config.c`. To change the default number of cells, change `WIDTH` and `HEIGHT`, set `SCALE` to desired width of each cell in pixels.

You can also change the RGB values of background (dead cells) and foreground (alive cells).

compile again for changes to take effect.


[1]: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
[2]: https://trac.ffmpeg.org/wiki/Slideshow
