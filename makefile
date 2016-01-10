CFLAGS=-fbuiltin -Wall -Wextra -g
LIBS= -lgcc -lGL -lGLU -lX11 -lXrandr -lpthread -lXi -lm -lXinerama -lXcursor -I../common -I"/usr/include/SDL2" -lSDL2

# uncomment for linux compilation
#CC=gcc
# uncomment for windows compilation
CC=x86_64-w64-mingw32-gcc

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
