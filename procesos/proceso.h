#include <sys/types.h>
// Estructura para compartir PID
typedef struct{
  int pid;
  int pid_main;
} Proceso; //memoria compartida

void mandarPid(pid_t pid, Proceso *memoria);
void desvincular(Proceso *memoria);
