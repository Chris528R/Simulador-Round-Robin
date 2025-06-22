/*
 * Idea principal, hacer uso de memoria compartida, para recibir el pid de cada proceso
 * Una vez que tengamos el pid de un nuevo proceso, lo agregamos a la cola.
 * Un hilo se encargara de permitir a cada proceso ejecutarse, dependiendo del quantumdado
  //Un hilo, se encargara de empezar a ejecutar los programas que esten dentro de la cola
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
cola listos;
key_t llave;
int shm;
Proceso *memoria;
int actualPid = 0;
volatile sig_atomic_t procesoTerminado = 0;

// DEFINICIONES DE FUNCIONES
void *funcionSimularTiempos(void* args);
void onReceive(int s);
void onSignal( int s);
void onEnd (int s);
void continuarProceso (elemento e){
  printf("\x1b[32mContinuando el proceso %d\x1b[0m\n", e.pid);
  kill(e.pid, SIGCONT);
}
void pausarProceso (elemento e){
  printf("\t\x1b[31mPausando el proceso %d\x1b[0m\n\n", e.pid);
  kill(e.pid, SIGSTOP);
}

//-----------------------------MAIN-----------------------------
int main(int argc, char *argv[]){
  //Señales
  signal(SIGINT, &onSignal);
  signal(SIGCONT, &onReceive);
  signal(SIGUSR1, &onEnd);

  pthread_t simularTiempos;
  //Inicializamos las colas
  Initialize(&listos);
  //verificamos que nos den el quantum
  if(argc < 2){
    perror("DEBES INGRESAR EL QUANTUM");
    exit(1);
  }
  int quantum = atoi(argv[1]), i, error, *t;

  printf("\n\x1b[36m------Bienvenido al simulador del algoritmo Round Robin------\x1b[0m\n");
  printf("Soy el main, con: %d\n", getpid());

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
  error = pthread_create(&simularTiempos, NULL, funcionSimularTiempos, (void*) &quantum);
  if (error){
    perror("ERROR EN CREATE");
    exit(1);
  }
  //Finalizar hilo
  error = pthread_join(simularTiempos, (void **) &t);
  Destroy(&listos);

  shmctl(shm, IPC_RMID, 0);
  return 0;
}

// FUNCIONES
void *funcionSimularTiempos(void* args){
  int n, quantum = *(int *) args;
  elemento p;
  int terminado;

  n = Size(&listos);

  while (1) {
    //Lo sacamos de la cola de listos
    if(!Empty(&listos)){
      p = Dequeue(&listos);
      continuarProceso(p);
      //Esperamos que se ejecute el tiempo del QUANTUM
      actualPid = p.pid;
      terminado = 0;
      sleep(quantum);
      //En caso de que alguna señal interrumpa durante la ejecución, este proceso no se volvera a encolar
      //En caso contrario, lo volvemos a encolar
      
      if(procesoTerminado && actualPid == p.pid) {
        procesoTerminado = 0;
        printf("\n\t\t---------Proceso %d terminado----------\n\n", p.pid);
      } else {
        pausarProceso(p);
        Queue(&listos, p);
      }
    }
  }
  pthread_exit(args);
}

void onReceive(int s){
  elemento e;
  e.pid = memoria->pid;
  Queue(&listos, e);
  pausarProceso(e);
  printf("\n\x1b[36mUn nuevo proceso con PID %d ha llegado:\x1b[0m\n\n", e.pid);
}

void onSignal(int s){
  shmctl(shm, IPC_RMID, 0);
  exit(1);
}

void onEnd(int s){
  //Significa que un proceso ya termino
  /*elemento e;
  e.pid = actualPid;
  Queue(&terminados, e);
  printf("Finalizo el proceso %d\n", e.pid);*/
  procesoTerminado = 1;
}
