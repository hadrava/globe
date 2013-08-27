CC=gcc
LDFLAGS=
CFLAGS=-c -Wall -std=c99


all: globe

globe: main.o params.o help.o log.o
	$(CC) $(LDFLAGS) main.o params.o help.o log.o -o globe

main.o: main.c
	$(CC) $(CFLAGS) main.c

params.o: params.c
	$(CC) $(CFLAGS) params.c

help.o: help.c
	$(CC) $(CFLAGS) help.c

log.o: log.c
	$(CC) $(CFLAGS) log.c
clean:
	rm -rf *o globe

remake: clean all
