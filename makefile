ifeq ($(OS),Windows_NT)
	CC=x86_64-w64-mingw32-gcc
	CFLAGS= -Wall -I/usr/local/cross-tools//x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main
	LDFLAGS=-lm -lopengl32 -lglu32 -L/usr/local/cross-tools//x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc
else
	CC=gcc
	CFLAGS=-c -g -I"/usr/include/SDL2"
	LDFLAGS=-lgcc -lGL -lGLU -lX11 -lXrandr -lpthread -lXi -lm -lXinerama -lXcursor -lSDL2
endif
SOURCEDIR=src
OBJDIR=obj
HEADERDIR=include
EXECUTABLE=back_into_orbit
SOURCES=$(wildcard src/*.c)
OBJECTS=$(subst src/, obj/, $(SOURCES:.c=.o))

all: $(SOURCEDIR) $(OBJDIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo Successfully compiled sources!
	@echo Linking objects \"$^\" into "$@"...
	@$(CC) $(LDFLAGS) $^ -o $@
	@echo Done!

obj/%.o: src/%.c
	@echo Compiling $< into $@...
	@$(CC) -I./$(HEADERDIR) $(CFLAGS) $< -o $@

$(SOURCEDIR):
	@echo Checking for $(SOURCEDIR)/ directory...
	@if [ ! -d "$(SOURCEDIR)" ];then\
		echo $(SOURCEDIR)/ directory not found. Creating one...;\
		mkdir -p $(SOURCEDIR);\
	else\
		echo Directory $(SOURCEDIR)/ found;\
	fi
$(OBJDIR):
	@echo Checking for required build directories...
	@echo Checking for $(OBJDIR)/ directory...
	@if [ ! -d "$(OBJDIR)" ];then\
		echo $(OBJDIR)/ directory not found. Creating one...;\
		mkdir -p $(OBJDIR);\
	else\
		echo Directory $(OBJDIR)/ found;\
	fi

.PHONY: clean
clean:
	@if [ -e $(EXECUTABLE) ];\
	then\
		echo Deleting $(EXECUTABLE);\
		rm -f $(EXECUTABLE);\
	fi;
	@if [ -e $(OBJDIR) ];\
	then \
		echo Deleting $(OBJDIR)/;\
		rm -fr $(OBJDIR);\
	fi;

.PHONY: help
help:
	@echo
	@echo "*** This makefile is designed to allow full automation of source tabulation, compilation, linking, and directory cleaning for committing to a git repo."
	@echo "*** There are currently 3 targets:"
	@echo
	@echo "    help      -- Print this help message"
	@echo "    all       -- Default target. Build the source into an executable target. Requires tabulated '$(SOURCELIST)' file in root directory."
	@echo "    clean     -- Remove '$(OBJDIR)/' directory and executable file '$(EXECUTABLE)' to allow clean commit to git repo."
	@echo

