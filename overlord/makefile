CFLAGS=-fbuiltin -Wall -Wextra -g
LIBS= -lgcc -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lm -lXinerama -lXft -lXcursor -I../common -lreadline
CC=gcc

SRCFILES:= $(shell find -type f -name "*.c")

OBJFILES:= $(patsubst %.c,%.o,$(SRCFILES)) 

PROJ:= 0verlord
	
.PHONY:all clean run

all: $(PROJ)

$(PROJ): $(OBJFILES)
	$(CC) -o $(PROJ) $(CFLAGS) $(OBJFILES) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm $(OBJFILES)
	rm $(PROJ)
