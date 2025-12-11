This software was originally developped by Maarten de Boer and was hosted here :
http://www.resorama.com/maarten/tapiir/tapiir.html

As of today, it compiles successfully on Debian Trixie using the following commands :

./configure CXXFLAGS="-I/usr/include/FL" LIBS="-lfltk"
make

The resulting binaries will be located in the src directory.

Note: FLTK version 1.1 is required.
