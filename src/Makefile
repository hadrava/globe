CC=gcc
LDFLAGS=
CFLAGS=-c -Wall -std=gnu99 -ggdb


all: globe

globe: main.o params.o log.o catalogue.o render.o transform.o drawings.o image.o interactive.o render_image.o stereographical.o fit.o
	$(CC) $(LDFLAGS) `pkg-config --libs opencv` -lnlopt -lm -lpthread main.o params.o log.o catalogue.o render.o transform.o drawings.o image.o interactive.o render_image.o fit.o stereographical.o -o globe

main.o: main.c
	$(CC) $(CFLAGS) main.c

params.o: params.c
	$(CC) $(CFLAGS) params.c

log.o: log.c
	$(CC) $(CFLAGS) log.c
	
catalogue.o: catalogue.c
	$(CC) $(CFLAGS) catalogue.c

render.o: render.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` render.c

transform.o: transform.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` transform.c

drawings.o: drawings.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` drawings.c

image.o: image.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` image.c

interactive.o: interactive.c
	$(CC) $(CFLAGS) interactive.c

render_image.o: render_image.c
	$(CC) $(CFLAGS) -pthread render_image.c

fit.o: fit.c
	$(CC) $(CFLAGS) fit.c

stereographical.o: stereographical.c
	$(CC) $(CFLAGS) `pkg-config --cflags opencv` stereographical.c

clean:
	rm -rf *o globe

remake: clean all
