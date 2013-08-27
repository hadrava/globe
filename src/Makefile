CC=gcc
LDFLAGS=
CFLAGS=-c -Wall -std=c99


all: globe

globe: main.o params.o help.o log.o catalogue.o render.o transform.o
	$(CC) $(LDFLAGS) `pkg-config --libs opencv` main.o params.o help.o log.o catalogue.o render.o transform.o -o globe

main.o: main.c
	$(CC) $(CFLAGS) main.c

params.o: params.c
	$(CC) $(CFLAGS) params.c

help.o: help.c
	$(CC) $(CFLAGS) help.c

log.o: log.c
	$(CC) $(CFLAGS) log.c
	
catalogue.o: catalogue.c
	$(CC) $(CFLAGS) catalogue.c

render.o: render.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` render.c

transform.o: transform.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` transform.c

clean:
	rm -rf *o globe

remake: clean all
