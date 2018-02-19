CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: movement_test

movement_test: log.cpp jacobA.cpp 
	g++ $(CFLAGS) jacobA.cpp log.cpp libggfonts.a \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so \
	-Wall -Wextra $(LFLAGS) -o movement_test

clean:
	rm -f movement_test 
	rm -f *.o

