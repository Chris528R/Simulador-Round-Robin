#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include "../proceso.h"

#define SILLAS 5
#define HILOS 10
#define TIEMPO_LLEGAR 5
int clientesSinLLegar = HILOS;

sem_t semBarbero, semSillas, semClientes; 
//sembarbero: Si esta dormido o DESPIERTO
//semSillas: Para mostrar cuando estan disponibles las sillas
//semClientes: si es que hay cleintes formados

int numClientes = 0;

void* barbero(void* arg){
  while (clientesSinLLegar > 0) {
    //printf("\tBarbero: Esperando por clientes\n");
    sem_wait(&semClientes);
    sem_wait(&semSillas);
    numClientes--;
    printf("\tBarbero: Atiende a un cliente: Sillas disponibles %d\n", SILLAS-numClientes);
    sem_post(&semBarbero);
    sem_post(&semSillas);

    sleep(rand() % 2 +1);
    printf("\tBarbero: Termino de cortar el cabello\n");

  }
  pthread_exit(arg);
}

void* cliente(void * arg){
  int i = *(int *) arg;
  sleep(rand() % TIEMPO_LLEGAR + 1);
  //printf("Cliente %d: Ha llegado\n", i);

  sem_wait(&semSillas);
  if(SILLAS - numClientes > 0){
    numClientes++;
    printf("Cliente %d: Esperando", i);
    printf("\t%d lugares disponibles\n", SILLAS-numClientes);
    sem_post(&semClientes);
    sem_post(&semSillas);
    sem_wait(&semBarbero);
    printf("Cliente %d: Siendo atendido\n", i);
    clientesSinLLegar--;
  }else{
    sem_post(&semSillas);
    printf("Cliente %d: No hay sillas disponibles\n", i);
    clientesSinLLegar--;
  }

  pthread_exit(arg);
}

int main() {
  srand(time(NULL));
  pthread_t hilos[HILOS+1];
  int idHilos[HILOS];
  sem_init(&semBarbero, 0, 0);
  sem_init(&semClientes, 0, 0);
  sem_init(&semSillas, 0, 1);

  Proceso *memoria = mandarPid(getpid(), 1);

  int i, *t, error;
  error = pthread_create(&hilos[0], NULL, barbero, NULL);
  if (error) {
    perror("ERROR EN CREATE (barbero)");
    exit(1);
  }
  for(i=1; i < HILOS+1; i++){
    idHilos[i] = i;
    error = pthread_create(&hilos[i], NULL, cliente, (void *)&idHilos[i]);
    if(error){
      perror("ERROR EN CREATE");
      exit(1);
    }
  }

  for(i=0; i < HILOS+1; i++){
    error = pthread_join(hilos[i], (void **)&t);
    if(error){
      perror("ERROR EN JOIN");
      exit(1);
    }
  }
  kill(memoria->pid_main, SIGUSR1);
  desvincular(memoria);
  sem_destroy(&semBarbero);
  sem_destroy(&semSillas);
  sem_destroy(&semClientes);
  return 0;
}
