ifeq ($(OS),Windows_NT)
	CC=x86_64-w64-mingw32-gcc
	CFLAGS= -Wall -I/usr/local/cross-tools//x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main
	LDFLAGS=-lm -lopengl32 -lglu32 -L/usr/local/cross-tools//x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc
else
	CC=gcc
	CFLAGS=-c -g -I"/usr/include/SDL2" -I"/usr/include/libxml2" -Wall
	LDFLAGS=-lgcc -lGL -lGLU -lX11 -lXrandr -lpthread -lXi -lm -lXinerama -lXcursor -lSDL2 $(xml2-config --libs)
endif
SOURCEDIR=src
OBJDIR=obj
HEADERDIR=include
EXECUTABLE=back_into_orbit
SOURCES=$(wildcard src/*.c)
OBJECTS=$(subst src/, obj/, $(SOURCES:.c=.o))

all: $(SOURCEDIR) $(OBJDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

obj/%.o: src/%.c
	$(CC) -I./$(HEADERDIR) $(CFLAGS) $< -o $@

$(SOURCEDIR):
	@if [ ! -d "$(SOURCEDIR)" ];then\
		mkdir -p $(SOURCEDIR);\
	fi
$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ];then\
		mkdir -p $(OBJDIR);\
	fi

.PHONY: clean
clean:
	@if [ -e $(EXECUTABLE) ];\
	then\
		rm -f $(EXECUTABLE);\
	fi;
	@if [ -e $(OBJDIR) ];\
	then \
		rm -fr $(OBJDIR);\
	fi;

