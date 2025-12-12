"Tapiir is a simple and flexible audio effects processor, inspired on the classical magnetic tape delay systems used since the early days of electro-acoustic music composition. It provides a graphical user interface consisting of six delay lines, or "taps", which can introduce an almost arbitrarily big or small delay to their inputs and can be feed back to each other.

A wide set of effects can be easily achieved by properly configuring and connecting the delay lines: complex echo patterns, resonances, filtering, etc. Delays, interconnections and gains can all be controlled in real time."

It was originally developped by Maarten de Boer and was hosted here :

http://www.resorama.com/maarten/tapiir/tapiir.html

As of today, it compiles successfully on Debian Trixie using the following commands :

./configure CXXFLAGS="-I/usr/include/FL" LIBS="-lfltk"

make

The resulting binaries will be located in the src directory.

Note: FLTK version 1.1 is required.
