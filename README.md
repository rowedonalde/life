life
====

This is Conway's Game of Life (or at least an approximation of
it--the field is finite, and all virtual cells right off the
edge are considered dead).

Build it with ```make life```. 
It takes the original generation in via
standard input, one line at a time, with 0s representing dead
cells and 1s representing living cells. End your input with a
newline and Ctrl-D.

Actually, it's probably easier and cleaner to say something like:

    ./life < pattern.txt

See the examples folder for examples.

##glife


glife is a version of life that takes the same input but instead
features windowed graphic output. It requires OpenGL and SDL2.
Build it with:

    make glife

You can build both life and glife with:

    make
