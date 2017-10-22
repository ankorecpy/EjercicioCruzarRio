#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NumPasajeros 4

typedef sem_t semaforo;
typedef pthread_t hilo;

void inicializar(int numero, void *(*funcion)(void *));
void * Hacker(void *);
void * Serf(void *);
void * Capitan(void *);
void abordar();
void abandonar();
void navegarBote();
void bajarPasajeros(); 
void incrementarSemaforo(semaforo * semaforoControl, int numeroIteraciones); 
void bajarSemaforo(semaforo * semaforoControl); 
void subirSemaforo(semaforo * semaforoControl); 

semaforo hackers;
semaforo serfs;
semaforo mutex;
semaforo abordo;
semaforo capitan;

int pasajerosPorGrupo = NumPasajeros / 2;
int pasajerosABordo = 0;

int main(int argc, char * argv[]) {
	int numeroPersonas = 0, personasPorGrupo = 0;
	hilo hiloCapitan;
	if (argc != 2) {
		fprintf(stderr, "Numero de parametros invalidos\n");
		exit(EXIT_FAILURE);
	}
	numeroPersonas = atoi(argv[1]);
	if ((numeroPersonas % NumPasajeros) != 0) {
		fprintf(stderr, "Numero de personas invalidas\n");
		exit(EXIT_FAILURE);
	}
	personasPorGrupo = numeroPersonas / 2;
	sem_init(&hackers, 0, pasajerosPorGrupo);
	sem_init(&serfs, 0, pasajerosPorGrupo);
	sem_init(&mutex, 0, 1);
	sem_init(&abordo, 0, 0);
	sem_init(&capitan, 0, 0);
	pthread_create(&hiloCapitan, 0, Capitan, 0);
	inicializar(personasPorGrupo, Hacker);
	inicializar(personasPorGrupo, Serf);
	sleep(3);
	exit(EXIT_SUCCESS);
}

void inicializar(int numero, void *(*funcion)(void *)) {
	hilo nuevoHilo;
	int iteracion;
	for (iteracion = 0; iteracion < numero; iteracion++) {
		pthread_create(&nuevoHilo, 0, funcion, (void *)iteracion + 1);
	}
}

void * Hacker(void * tipo) {
	bajarSemaforo(&hackers);
	printf("\n Hacker %d abordó el bote\n", tipo);
	abordar();
	printf("\n Hacker %d abandonó el bote\n", tipo);
	abandonar();
}

void * Serf(void * tipo) {
	bajarSemaforo(&serfs);
	printf("\n Serf %d abordó el bote\n", tipo);
	abordar();
	printf("\n Serf %d abandonó el bote\n", tipo);
	abandonar();
}

void * Capitan(void * argumento) {
	while (1) {
		bajarSemaforo(&capitan);
		printf("\n--------PASAJEROS BAJANDO-----------\n");
		bajarPasajeros();
	}
}

void abordar() {
	bajarSemaforo(&mutex);
	pasajerosABordo++;
	if (pasajerosABordo == NumPasajeros) {
		navegarBote();
	}
	subirSemaforo(&mutex);
	bajarSemaforo(&abordo);
}

void abandonar() {
	bajarSemaforo(&mutex);
	pasajerosABordo--;
	if (pasajerosABordo == 0) {
		incrementarSemaforo(&hackers, pasajerosPorGrupo);
		incrementarSemaforo(&serfs, pasajerosPorGrupo);
		printf("------------------------------------\n");
	}
	subirSemaforo(&mutex);
}

void navegarBote() {
	incrementarSemaforo(&capitan, 2);
}

void bajarPasajeros() {
	incrementarSemaforo(&abordo, NumPasajeros);
	bajarSemaforo(&capitan);
}

void incrementarSemaforo(semaforo * semaforoControl, int numeroIteraciones) {
	int iteracion = 0;
	for (iteracion = 0; iteracion < numeroIteraciones; iteracion++) {
		subirSemaforo(semaforoControl);
	}
}

void bajarSemaforo(semaforo * semaforoControl) {
	sem_wait(semaforoControl);
}

void subirSemaforo(semaforo * semaforoControl) {
	sem_post(semaforoControl);
}
