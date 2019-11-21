# Defines
CC = gcc
CFLAGS = -g
INCLUDE =
LIBS =
OBJECTS = main.o
OUTPUT_FILE = ../bin/gas68

# Targets
all: main.o
	$(CC) -o $(OUTPUT_FILE) $(OBJECTS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

# Clean up files
clean:
	rm $(OUTPUT_FILE).exe *.o
