CC = gcc

all: serial pthread pthread2 omp

serial: main.c
	$(CC) main.c -lm -o serial

pthread: p_main.c
	$(CC) p_main.c -lm -o pthread -lpthread

omp: omp_main.c
	$(CC) omp_main.c -lm -o omp -fopenmp

pthread2: p2_main.c
	$(CC) p2_main.c -lm -o pthread2 -lpthread

verbose: main.c p_main.c omp_main.c	p2_main.c
	$(CC) main.c -DVERBOSE -lm -o serial
	$(CC) p_main.c -DVERBOSE -lm -o pthread -lpthread
	$(CC) p2_main.c -DVERBOSE -lm -o pthread2 -lpthread
	$(CC) omp_main.c -DVERBOSE -lm -o omp -fopenmp

clean:
	rm -f omp pthread pthread2 serial
