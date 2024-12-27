CC = gcc
HEADERS = {wildcard *.h}
CFLAGS = -O3 -Wall -pedantic -fopenmp -std=c17

all: main

main: main.o ppm.o scene.o
	${CC} ${CFLAGS} -o $@ $^

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean:
	rm -f *.o
	rm -f main

