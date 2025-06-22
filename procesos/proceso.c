/*
IMPLEMENTACION DE LA LIBRERIA PARA EL MANEJO DE LOS PROCESOS (proceso.h)
AUTORES:    Rodriguez Mendoza Christopher
            Barcenas Pineda Luca Alexander
            Peredo Borgonio Daniel
VERSIÓN: 1.0

DESCRIPCIÓN: Código que se encarga de vincular a un proceso externo con la memoria compartida,
    así como de comunicar al main con los demás procesos
 
*/
#include "proceso.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

Proceso* mandarPid(pid_t pid, int flag){ //Bandera para indicar si bloquear el proceso o no
  printf("El pid es : %d\n", pid);
 //VARIABLES PARA PASAR EL PID
  key_t llave;
  int shmg;
  Proceso * memoria;
  llave = ftok("usr/bin/ls", 10);
  shmg = shmget(llave, sizeof(Proceso), 0666);

  if (shmg == -1){
    perror("ERROR EN EL SHMGET");
    exit(0);
  }
  //Vinculamos la memoria
  memoria = shmat(shmg, 0, 0);
  memoria->pid = pid;
  //printf("El pid del main es : %d\n", memoria->pid_main);
  printf("Mandando a main a nuestro pid %d\n", memoria->pid_main);
  kill( memoria->pid_main , SIGCONT); //Indicamos que ya mandamos nuestro pid
  if (flag)
    kill(pid, SIGSTOP); //Bloqueamos este proceso, hasta que nos desbloqueen, para indicar que ya podemos continuar
  return memoria;
}

void desvincular(Proceso * memoria){
  shmdt(memoria);
}
