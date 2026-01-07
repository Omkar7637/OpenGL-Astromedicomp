g++ -c -o OGL.o OGL.cpp -I/usr/include/SOIL && g++ -o OGL OGL.o -lSOIL -lX11 -lGL -lGLU -lGLEW && ./OGL && rm OGL.o
