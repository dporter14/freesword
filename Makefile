CFLAGS = -I ./include
LFLAGS = -lX11 -lGLU -lGL -lm -lglut

ifeq ($(OS),Windows_NT)
    CFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CFLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAGS += -D LINUX
        LFLAGS += -lrt /usr/lib/x86_64-linux-gnu/libopenal.so \
        	/usr/lib/x86_64-linux-gnu/libalut.so libggfonts.a
    endif
    ifeq ($(UNAME_S),Darwin)
        CFLAGS += -D OSX -I/opt/X11/include
        LFLAGS +=  -L/opt/X11/lib -lalut -framework OpenAL m_libggfonts.a
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CFLAGS += -D ARM
    endif
endif

all: freesword snake

freesword: freesword.cpp log.cpp jacobA.cpp taylorR.cpp davidP.cpp masonP.cpp
	g++ $(CFLAGS) freesword.cpp log.cpp jacobA.cpp taylorR.cpp davidP.cpp masonP.cpp  \
	-Wall -Wextra $(LFLAGS) -o freesword


snake: snake.cpp log.cpp
	g++ $(CFLAGS) snake.cpp log.cpp  \
	-Wall -Wextra $(LFLAGS) -o snake

clean:

	rm -f snake
	rm -f freesword
	rm -f *.o

