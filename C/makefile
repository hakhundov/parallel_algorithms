CC = gcc

all: serial pthread omp

serial: main.c
	$(CC) main.c -lm -o serial

pthread: p_main.c
	$(CC) p_main.c -lm -o pthread -lpthread

omp: omp_main.c
	$(CC) omp_main.c -lm -o omp -fopenmp
