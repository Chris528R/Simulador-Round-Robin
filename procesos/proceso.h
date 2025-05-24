#include <sys/types.h>
// Estructura para compartir PID
typedef struct{
  pid_t pid;
  pid_t pid_main;
} Proceso; //memoria compartida

Proceso* mandarPid(pid_t pid, int flag);
void desvincular(Proceso *memoria);
