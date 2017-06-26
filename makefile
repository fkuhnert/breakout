CC=gcc

CFLAGS=-g -Wall -pedantic -D_GNU_SOURCE=1 -D_REENTRANT -std=c90
#CFLAGS=-g -Wall -D_REENTRANT
MATH_FLAGS=-lm
IMAGE_FLAGS=-lSDL2_image -lSDL2_ttf
SOUND_FLAGS=-lSDL2_mixer
SFLAGS=-lmingw32 -lSDL2main -lSDL2
SOURCES=main.c funcoes.c Globais.c
OBJECTS=main.o funcoes.o Globais.o
BINARIES=main

all: $(BINARIES)

main: main.o funcoes.o Globais.o
	$(CC) -o main main.o funcoes.o Globais.o $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(SOUND_FLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS) $(MATH_FLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(SOUND_FLAGS)

funcoes.o: funcoes.c
	$(CC) -c funcoes.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(SOUND_FLAGS)

Globais.o: Globais.c
	$(CC) -c Globais.c $(CFLAGS) $(SFLAGS) $(IMAGE_FLAGS) $(SOUND_FLAGS)

clean:
	rm -rf *.o *.exe *.bak *.c~ $(BINARIES) core a.out

limpa:
	del *.o *.exe *.bak *.c~ $(BINARIES) core
