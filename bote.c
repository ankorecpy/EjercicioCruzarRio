#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef sem_t hilo;

void inicializarHackers(int numero);
void * cruzar(void *);

int main(int argc, char * argv[]) {
	int numeroPersonas = 0, personasPorGrupo = 0;
	if (argc != 2) {
		fprintf(stderr, "Numero de parametros invalidos\n");
		exit(EXIT_FAILURE);
	}
	numeroPersonas = atoi(argv[1]);
	if ((numeroPersonas % 4) == 0) {
		fprintf(stderr, "Numero de personas invalidas\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void inicializaHackers(int numero) {
	hilo informatico;
	int iteracion;
	for (iteracion = 0; iteracion < numero; iteracion++) {
		
		
	}
}

void * cruzar(void * tipo) {
}
