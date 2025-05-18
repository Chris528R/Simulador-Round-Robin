//Mi idea es que para todo proceso, primero este tenga que mandar su pid, de esta forma
//Nos aseguramos de que esté ya este listado en la cola y el proceso padre, solo se encargarái de despausrlos cuando se le indique, de esta forma solo se necesitaran de señales
//para manejar los procesos

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
// -----Se incluye la siguiente libreria que se encargara de mandar señal y hacer que el proceso
// espere a el main
#include "proceso.h"



// DEFINICION DE FUNCIONES

// ----------------------------------------MAIN-----------------------------
int main(){
  Proceso *memoria;
  printf("%d\n", getpid());
  mandarPid(getpid(), memoria);
    //PROGRAMA
  int i = 100000;
  while (i) {
    printf("Hola %d\n", i);
    usleep(100);
    i--;
  }
  //una vez que termine de ejecutarse:
  //Desvinculamos memoria y mandamos señal indicando que ya acabo nuestro proceso
  printf("Mandando la señal al main: %d", memoria->pid_main);
  kill(memoria->pid_main, SIGUSR1);
  desvincular(memoria);
}

// FUNCIONES
