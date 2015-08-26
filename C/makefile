CC = gcc

all: main.c
	$(CC) main.c -o C -g 

pthread: p_main.c
	$(CC) p_main.c -o p_C -lpthread

omp: omp_main.c
	$(CC) omp_main.c -lm -g -o omp_C -fopenmp
