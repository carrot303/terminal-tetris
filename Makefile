OBJECTS = tetris.o main.o
TARGET_BIN = tetris
CC = gcc
PARAMS = -lncursesw
.PHONY = clean

build: ${OBJECTS}
	$(CC) -o ${TARGET_BIN} $^ ${PARAMS}

main.o:
	$(CC) -c main.c

tetris.o: tetris.h
	$(CC) -c tetris.c

rebuild: clean build

clean:
	rm -rf ${OBJECTS}