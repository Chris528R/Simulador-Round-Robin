/*
 *Este programa crea dos procesos hijo (esta programado para que funcione para n hijos), el padre imprime su pid y un contador que empiece en 1 y se incremente hasta 30
 * El hijo impime su pid y un contando empueza en 1 y se incrementa hasta que llegue a 50
 * Para evitar la condición de carrera hacemos uso de un semaforo con nombre :)
 */
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../proceso.h"
#include <semaphore.h>

#define N_HIJOS 2

int main() {
  pid_t pid;
  int t, p;
  Proceso *memoria;
  sem_t *semaforo = sem_open("/m_semaforo", O_CREAT, 0666, 1);
  if (semaforo == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }

  // Crear hijos
  for (p = 0; p < N_HIJOS; p++) {
    pid = fork();
    if (pid == -1) {
      perror("Error en fork");
      exit(1);
    } else if (pid == 0) {
      // PROCESO HIJO
      sem_wait(semaforo);
      printf("Soy el hijo con pid %d\n", getpid());
      memoria = mandarPid(getpid(), 0);
      sem_post(semaforo);
      kill(getpid(), SIGSTOP);

      int i = 1;
      while (i <= 50) {
        printf("\t\tHIJO %d: i %d\n", getpid(), i);
        sleep(1);
        i++;
      }

      kill(memoria->pid_main, SIGUSR1);
      desvincular(memoria);
      exit(0);
    }
    // El padre sigue creando hijos
  }

  // PROCESO PADRE
  sem_wait(semaforo);
  printf("Soy el padre con pid %d\n", getpid());
  memoria = mandarPid(getpid(), 0);
  sem_post(semaforo);
  kill(getpid(), SIGSTOP);

  int i = 1;
  while (i <= 30) {
    printf("\tPADRE: i %d\n", i);
    sleep(2);
    i++;
  }

  kill(memoria->pid_main, SIGUSR1);
  desvincular(memoria);

  for (p = 0; p < N_HIJOS; p++) {
    wait(&t);
  }

  sem_close(semaforo);
  sem_unlink("/m_semaforo");

  return 0;
}
