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
int actualPid = 0;

// DEFINICIONES DE FUNCIONES
void *funcionSimularTiempos(void* args);
void onReceive(int s);
void onSignal( int s);
void onEnd (int s);
void continuarProceso (cola *c){
  elemento e = Front(c);
  kill(e.pid, SIGCONT);
}
void pausarProceso (cola *c){
  elemento e = Front(c);
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
  Initialize(&terminados);
  //verificamos que nos den el quantum
  if(argc < 2){
    perror("DEBES INGRESAR EL QUANTUM");
    exit(1);
  }
  int quantum = atoi(argv[1]), i, error, *t;

  printf("Bienvenido al simulador del algoritmo Round Robin\n");

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
  Destroy(&terminados);

  shmctl(shm, IPC_RMID, 0);
  return 0;
}

// FUNCIONES
void *funcionSimularTiempos(void* args){
  int n, quantum = *(int *) args;
  printf("Esperando por la entrada, para iniciar: \n");
  scanf("%d", &n);
  elemento p;

  while (!Empty(&listos)) {
    //Reanudamos el proceso
    continuarProceso(&listos);
    //Lo sacamos de la cola de listos
    p = Dequeue(&listos);
    //Esperamos que se ejecute el tiempo del QUANTUM
    actualPid = p.pid;
    sleep(quantum);
    //En caso de que alguna señal interrumpa durante la ejecución, este proceso no se volvera a encolar
    //En caso contrario, lo volvemos a encolar
    if(!Empty(&terminados)){
      if(Front(&terminados).pid != p.pid){
	Queue(&listos, p);
	pausarProceso(&listos);
     }
    }else{
	Queue(&listos, p);
	pausarProceso(&listos);
    }
  }
  pthread_exit(args);
}

void onReceive(int s){
  elemento e;
  e.pid = memoria->pid;
  Queue(&listos, e);
  pausarProceso(&listos);
  printf("Un nuevo proceso con PID %d a llegado: \n", memoria->pid);
}

void onSignal(int s){
  shmctl(shm, IPC_RMID, 0);
  exit(1);
}

void onEnd(int s){
  //Significa que un proceso ya termino
  elemento e;
  e.pid = actualPid;
  Queue(&terminados, e);
  printf("Finalizo el proceso %d", actualPid);
}
