CFLAGS=-fbuiltin -Wall -Wextra -g
LIBS= -lgcc -lGL -lGLU -lglfw3 -lX11 -lXrandr -lpthread -lXi -lm -lXinerama -lXcursor -I../common -I"/usr/include/SDL2" -lSDL2
CC=gcc

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
