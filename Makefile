## -*- Makefile del progetto -*-

all: build
	mpicc *.o -o main -lm

## Target: build
build:
	mpicc *.c -c

## Target: clean
clean:
	rm main
	rm *.o
