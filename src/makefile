# Defines
CC = gcc
CFLAGS = -g
INCLUDE =
LIBS =
OBJECTS = main.o

# Targets
all: main.o
	$(CC) -o gas68 $(OBJECTS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Clean up files
clean:
	rm gas68.exe *.o
