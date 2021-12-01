CC=gcc
OFLAGS = -O0 -g -lglut -lGL
demo:
	$(CC) -o demo demo.c $(OFLAGS)


clean:
	rm demo
