CC=gcc
CFLAGS=-std=c99
OFLAGS = -O0 -g -lglut -lGL
demo:
	$(CC) -o demo demo.c pong.c $(CFLAGS) $(OFLAGS)


clean:
	rm demo
