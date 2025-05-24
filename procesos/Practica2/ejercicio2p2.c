#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../proceso.h"
#include <fcntl.h>

int main() {
    pid_t pidm, pidz, pida;
    int x, y, z;
    Proceso *memoria;
    sem_t *semaforo = sem_open("/m_semaforo", O_CREAT, 0666, 1);
    if (semaforo == SEM_FAILED) {
      perror("sem_open");
      exit(1);
    }
   
    for (x = 0; x < 2; x++) {
        pidm = fork();
	if( pidm == -1){
	  perror("Error en fork");
	  exit(0);
	}else if (pidm == 0) {
	    sem_wait(semaforo);
            printf("Proceso: %d Padre: %d Generacion: Millenial\n", getpid(), getppid());
	    memoria = mandarPid(getpid(), 0);
	    sem_post(semaforo);
	    kill(getpid(), SIGSTOP);
            for (y = 0; y < 3; y++) {
                pidz = fork();
		if(pidz == -1){
		  perror("Error en fork");
		  exit(0);
		}else if (pidz == 0) {
		    sem_wait(semaforo);
                    printf("Proceso: %d Padre: %d Generacion: Z\n", getpid(), getppid());
		    memoria = mandarPid(getpid(), 0);
		    sem_post(semaforo);
		    kill(getpid(), SIGSTOP);
                    for (z = 0; z < 2; z++) {
                        pida = fork();
			if(pida == -1){
			  perror("Error en fork");
			  exit(0);
			}else if (pida == 0) {
			    sem_wait(semaforo);
                            printf("Proceso: %d Padre: %d Generacion: Alfa\n", getpid(), getppid());
			    memoria = mandarPid(getpid(), 0);
			    sem_post(semaforo);
			    kill(getpid(), SIGSTOP);
                            sleep(2);
			    kill(memoria->pid_main, SIGUSR1);
		            desvincular(memoria);
                            exit(0);
                        }
                    }

		    kill(memoria->pid_main, SIGUSR1);
		    desvincular(memoria);
                   
                    for (z = 0; z < 2; z++) {
                        wait(NULL);
                    }
                    printf("Proceso %d Generacion Z terminó. Los hijos Alfa finalizaron\n", getpid());
                    exit(0);
                }
            }

	    kill(memoria->pid_main, SIGUSR1);
	    desvincular(memoria);
           
            for (y = 0; y < 3; y++) {
                wait(NULL);
            }
            printf("Proceso %d Generacion Millenial terminó. Los hijos Z finalizaron\n", getpid());
            exit(0);
        }
    }
    //PROCESO PADRE
    sem_wait(semaforo);
    pid_t pid_gx = getpid();
    printf("%d\n", pid_gx);
    printf("Proceso: %d Padre: %d Generacion: X\n", getpid(), getppid());
    memoria = mandarPid(getpid(), 0);
    sem_post(semaforo);
 
   
    if (getpid() == pid_gx) {
        sleep(1);
        system("gnome-terminal -- bash -c 'pstree -p $PPID; exec bash'");
    }

    kill(memoria->pid_main, SIGUSR1);
    desvincular(memoria);
    for (x = 0; x < 2; x++) {
        wait(NULL);
    }
    printf("Proceso %d Generacion X terminó. Los hijos Millenial finalizaron\n", getpid());
    sem_close(semaforo);
    sem_unlink("/m_semaforo");
    return 0;
}
