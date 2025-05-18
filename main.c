/*
 * Idea principal, hacer uso de memoria compartida, para recibir el pid de cada proceso
 * Una vez que tengamos el pid de un nuevo proceso, lo agregamos a la cola.
 * Un hilo se encargara de permitir a cada proceso ejecutarse, dependiendo del quantumdado
 //Vamos a usar un hilo que va a estar esperando la llegada de algún proceso, cuando algún proceso mande su pid, este se agregará a la cola de listos
  //Otro hilo, se encargara de empezar a ejecutar los programas que esten dentro de la cola
*
*/


#include <stdio.h>
#include <stdlib.h>
#include "TADColaDin.h"
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include "procesos/proceso.h"
#include <signal.h>
#include <unistd.h>
// VARIABLES GLOBALES
cola listos, terminados;
key_t llave;
int shm;
Proceso *memoria;

// DEFINICIONES DE FUNCIONES
void *funcionSimularTiempos(void* args);
void *funcionEsperarProcesos(void* args);
void onReceive(int s);
void onSignal( int s);

//-----------------------------MAIN-----------------------------
int main(int argc, char *argv[]){
  //Señales
  signal(SIGINT, &onSignal);
  signal(SIGCONT, &onReceive);

  pthread_t simularTiempos, esperarProcesos;
  //Inicializamos las colas
  Initialize(&listos);
  Initialize(&terminados);
  //verificamos que nos den el quantum
  if(argc < 2){
    perror("DEBES INGRESAR EL QUANTUM");
    exit(1);
  }
  int quantum = atoi(argv[1]), i, error, *t;

  //Creamos la memoria compartida
  llave = ftok("usr/bin/ls", 10);
  shm = shmget(llave, sizeof(Proceso), IPC_CREAT | 0666);
  

  if(shm == -1){
    perror("ERROR EN EL SHMGET");
    exit(0);
  }

  memoria = shmat(shm, 0, 0);
  memoria->pid = 0;
  memoria->pid_main = getpid();
  //HILO QUE SIMULA LOS TIEMPOS
  error = pthread_create(&simularTiempos, NULL, funcionSimularTiempos, NULL);
  if (error){
    perror("ERROR EN CREATE");
    exit(1);
  } 

  //HILO QUE ESPERA LA LLEGADA DE LOS PROCESOS
  error = pthread_create(&esperarProcesos, NULL, funcionEsperarProcesos, NULL);
  if (error){
    perror("ERROR EN CREATE");
    exit(1);
  } 
  
  //Para finalizar los hilos
  error = pthread_join(simularTiempos, (void**) &t);
  if (error){
    perror("ERROR EN JOIN");
    exit(1);
  }
  error = pthread_join(esperarProcesos, (void**) &t);
  if (error){
    perror("ERROR EN JOIN");
    exit(1);
  }

  Destroy(&listos);
  Destroy(&terminados);

  return 0;
}

// FUNCIONES
void *funcionSimularTiempos(void* args){
  int n;
  printf("Esperando por la entrada: para iniciar");
  scanf("%d", &n);

  while (!Empty(&listos)) {
    printf("Prueba: %d\n", Front(&listos).pid);
    Dequeue(&listos);
  }
  pthread_exit(args);
}
void *funcionEsperarProcesos(void* args){
  
  pthread_exit(args);
}

void onReceive(int s){
  elemento e;
  e.pid = memoria->pid;
  Queue(&listos, e);
  printf("Un nuevo proceso con PID %d a llegado: \n", memoria->pid);
  kill(e.pid, SIGCONT);
}

void onSignal(int s){
  shmctl(shm, IPC_RMID, 0);
}
