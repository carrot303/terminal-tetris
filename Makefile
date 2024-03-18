OBJECTS = tetris.o main.o
TARGET_BIN = tetris
CC = gcc
.PHONY = clean

build: ${OBJECTS}
	$(CC) -o ${TARGET_BIN} $^

main.o:
	$(CC) -c main.c

tetris.o: tetris.h
	$(CC) -c tetris.c

rebuild: clean build

clean:
	rm -rf ${TARGET_BIN} ${OBJECTS}