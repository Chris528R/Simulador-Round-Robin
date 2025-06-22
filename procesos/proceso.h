/*
LIBRERIA: CABECERA DE LA LIBRERIA PARA EL MANEJO DE LOS PROCESOS (proceso.h)
AUTORES:    Rodriguez Mendoza Christopher
            Barcenas Pineda Luca Alexander
            Peredo Borgonio Daniel
VERSIÓN: 1.0

DESCRIPCIÓN: Código que se encarga de vincular a un proceso externo con la memoria compartida,
    así como de comunicar al main con los demás procesos
 
*/
#include <sys/types.h>
// Estructura para compartir PID
typedef struct{
  pid_t pid;
  pid_t pid_main;
} Proceso; //memoria compartida

Proceso* mandarPid(pid_t pid, int flag);
void desvincular(Proceso *memoria);
