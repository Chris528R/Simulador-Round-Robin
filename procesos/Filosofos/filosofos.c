#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "../proceso.h"

#define FILOSOFOS 5
#define HAMBRIENTO 0
#define COMIENDO 1
#define PENSANDO 2

int estados[FILOSOFOS];
sem_t semaforos[FILOSOFOS], semEdos;
Proceso *memoria;
void* funcFilosofo(void* arg);

void terminar(int s){
  int i;
  kill(memoria->pid_main, SIGUSR1);
  desvincular(memoria);

  for(i=0; i < FILOSOFOS; i++) sem_destroy(&semaforos[i]);
  sem_destroy(&semEdos);
  exit(1); 
}

int main() {
  signal(SIGINT, terminar);
  srand(time(NULL));
  pthread_t filosofos[FILOSOFOS];
  int i, error, *t;
  sem_init(&semEdos, 0, 1);
  memoria = mandarPid(getpid(), 1);

  for(i=0; i < FILOSOFOS; i++){
    int *id = malloc(sizeof(int));
    *id = i;
    sem_init(&semaforos[i], 0, 0);
    estados[i] = PENSANDO;
    error = pthread_create(&filosofos[i], NULL, funcFilosofo, (void *)id);
    if(error){
      perror("ERROR EN CREATE");
      exit(1);
    }
  }

  for(i=0; i < FILOSOFOS; i++){
    error = pthread_join(filosofos[i], (void **) &t);
    if(error){
      perror("ERROR EN JOIN");
      exit(1);
    }
  }
  //Problema (nunca se va a acabar, ya que este problema es con un while infinito)
  //Podemos intentar recortar, para que acabe o lo dejamos así
  kill(memoria->pid_main, SIGUSR1);
  desvincular(memoria);

  for(i=0; i < FILOSOFOS; i++) sem_destroy(&semaforos[i]);
  sem_destroy(&semEdos);

  return  0;
}

//_________________________________FUNCIONES______________________________
void pensar(int i){
  printf("Filosofo %d: Pensando\n", i+1);
  sleep(rand() % 3 + 1); //Aleatorio entre 1 - 3 seg
}

int filosofoIzquiero(int i) {
  return (i+FILOSOFOS-1) % FILOSOFOS;
}

int filosofoDerecho(int i){
  return (i+1) % FILOSOFOS;
}

void tenedoresDisponibles(int i) {
  int izq = filosofoIzquiero(i), der = filosofoDerecho(i);
  if(estados[i] == HAMBRIENTO && estados[izq] != COMIENDO && estados[der] != COMIENDO){
    estados[i] = COMIENDO;
    printf("Filosofo %d: Voy a COMER\n", i+1);
    sem_post(&semaforos[i]);
  }
}

void tomarTenedores(int i){
  sem_wait(&semEdos);
  estados[i] = HAMBRIENTO;
  printf("Filosofo %d: Ahora estoy HAMBRIENTO\n", i+1);
  tenedoresDisponibles(i);
  sem_post(&semEdos);
  sem_wait(&semaforos[i]);
}

void comer (int i){ 
  printf("Filosofo %d: COMIENDO\n", i+1);
  sleep(1);
}

void dejarTenedores (int i) {
  sem_wait(&semEdos);
  estados[i] = PENSANDO;
  printf("Filosofo %d: Ya termine de comer\n", i+1);
  tenedoresDisponibles(filosofoIzquiero(i));
  tenedoresDisponibles(filosofoDerecho(i));
  sem_post(&semEdos);
}

void* funcFilosofo(void* arg){
  while (1) {
    pensar(*(int *) arg);
    tomarTenedores(*(int *) arg);
    comer(*(int *) arg);
    dejarTenedores(*(int *) arg);
  }
  pthread_exit(arg);
}
