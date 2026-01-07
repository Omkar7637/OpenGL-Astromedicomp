gcc -c -o OGL.o OGL.c
gcc -o OGL OGL.o -lX11 -lGL -lGLU
./OGL
