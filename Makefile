OBJECTS = tetris.o tetrominoes.o main.o game.o room.o
TARGET_BIN = tetris
CC = gcc
PARAMS = -lncursesw -lm
.PHONY = clean

build: ${OBJECTS}
	$(CC) -o ${TARGET_BIN} $^ ${PARAMS}

main.o:
	$(CC) -c main.c

tetris.o: tetris.h
	$(CC) -c tetris.c

tetrominoes.o: tetrominoes.h
	$(CC) -c tetrominoes.c

game.o: game.h
	$(CC) -c game.c

room.o: room.h
	$(CC) -c room.c

rebuild: clean build

clean:
	rm -rf ${OBJECTS}