# AED 2020/2021, Projeto AirRoutes
#
# To compile program:
# make
#
# Miguel Ribeiro, Filipe Oliveira
#--------------------------------------------------

# Compiler Flags

CC= gcc
CFLAGS= -Wall -std=c99 -O3

backbone: main.o grafo.o backbones.o
	gcc $(CFLAGS) -o backbone main.o grafo.o backbones.o

main.o: main.c grafo.h backbones.h
	gcc $(CFLAGS) -c main.c

grafo.o: grafo.c grafo.h
	gcc $(CFLAGS) -c grafo.c

backbones.o: backbones.c backbones.h
	gcc $(CFLAGS) -c backbones.c

clean:
	rm -f *.o backbone *.~
