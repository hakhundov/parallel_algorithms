CC = gcc

all: main.c
	$(CC) main.c -lm -o A 

pthread: p_main.c
	$(CC) p_main.c -lm -o p_A -lpthread

omp: omp_main.c
	$(CC) omp_main.c -lm -o omp_A -fopenmp
