# Simulador Round Robin

Este proyecto consiste en la simulación del algoritmo de planificación Round Robin utilizando el lenguaje C. Cada proceso recibe una porción de tiempo denominada *quantum*, que define cuánto tiempo puede ejecutarse antes de ceder el control al siguiente proceso en la cola.

## Estructura del Proyecto

### Archivos principales

- `main.c`: Controla el flujo principal del programa y la gestión de procesos mediante una cola dinámica (`listos`).
- `TADColaDin.c`: Implementación de la cola dinámica.
- `proceso.c` / `proceso.h`: Encargados de gestionar la memoria compartida y la comunicación de los procesos con `main`.

### Carpeta `/procesos`

Aquí se ubican los procesos individuales que se comunicarán con el `main`. Como ejemplo, se incluye `prueba.c`, un proceso que imprime números del 10,000 al 1. Este archivo es modificable y sirve como plantilla base para agregar nuevos procesos al simulador.

## Compilación y Ejecución

### Compilación del simulador principal

```bash
gcc main.c TADColaDin.c procesos/proceso.c -o main
```

### Compilación del simulador principal
```bash
./main <quantum>
```
   Donde `<quantum>` es el tiempo asignado para cada proceso

### Compilación de procesos de prueba
``` bash 
gcc prueba.c proceso.c -o pru
```
### Ejecución de un proceso de prueba

```bash 
./pru
```
**Importante:** siempre se debe ejecutar primero el programa `main`, ya que este se encarga de crear la memoria compartida. Después de que `main` indique que ha recibido el PID del proceso, se pueden iniciar los demás procesos.

## Detalles Técnicos
- Cada proceso que se lanza utiliza memoria compartida para enviar su PID al `main`, que lo encola para su ejecución según el algoritmo Round Robin.

- Se utilizan `semáforos con nombre` para sincronizar el envío de PIDs cuando los procesos se generan mediante `fork()`.

- En casos donde el proceso tiene ejecución indefinida (como en el problema de los filósofos), se utiliza una señal `SIGINT` para finalizarlo adecuadamente y liberar recursos.

- El `main` inicia la ejecución de procesos de forma controlada utilizando `hilos` para evitar bloqueos y permitir una planificación concurrente.

## Recursos Adicionales
- Se incluye un script `.sh` para facilitar la compilación de los distintos componentes del proyecto.