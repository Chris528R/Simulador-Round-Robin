#include "proceso.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
void mandarPid(int pid, Proceso *memoria){
  printf("El pid es : %d\n", pid);
 //VARIABLES PARA PASAR EL PID
  key_t llave;
  int shmg;
  llave = ftok("usr/bin/ls", 10);
  shmg = shmget(llave, sizeof(Proceso), 0666);

  if (shmg == -1){
    perror("ERROR EN EL SHMGET");
    exit(0);
  }
  //Vinculamos la memoria
  memoria = shmat(shmg, 0, 0);
  memoria->pid = pid;

  kill( memoria->pid_main , SIGCONT); //Indicamos que ya mandamos nuestro pid

  kill(pid, SIGSTOP); //Bloqueamos este proceso, hasta que nos desbloqueen, para indicar que ya podemos continuar
}

void desvincular(Proceso * memoria){
  shmdt(memoria);
}
