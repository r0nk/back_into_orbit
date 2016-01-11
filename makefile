
# uncomment for linux compilation
CC=gcc
CFLAGS=-fbuiltin -Wall -Wextra -g
LIBS= -lgcc -lGL -lGLU -lX11 -lXrandr -lpthread -lXi -lm -lXinerama -lXcursor  -I"/usr/include/SDL2" -lSDL2

# uncomment for windows compilation
# CC=x86_64-w64-mingw32-gcc 
# CFLAGS= -Wall -I/usr/local/cross-tools//x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main
# LIBS= -lm -lopengl32 -lglu32 -L/usr/local/cross-tools//x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc



SRCFILES:= $(shell find -type f -name "*.c")

OBJFILES:= $(patsubst %.c,%.o,$(SRCFILES)) 

PROJ:= back_into_orbit
	
.PHONY:all clean run

all: $(PROJ)

$(PROJ): $(OBJFILES)
	$(CC) -o $(PROJ) $(CFLAGS) $(OBJFILES) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm $(OBJFILES)
	rm $(PROJ)
