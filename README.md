# Simulador-Round-Robin
Simulación en C, en donde se asigna un tiempo Quantum para cada proceso
## Funcionamiento
Para el correcto funcionamiento, se tiene lo siguiente, el main, se encarga de manejar los procesos con la cola, tenemos una cola dinamica (listos), para compilar el main:
* gcc main.c TADColaDin.c procesos/proceso.c -o main
* Para ejecutar: ./main 5 

El argumento indica el quantum

Dentro de la carpeta /procesos/: Se tiene pensado guardar los procesos que se comunicaran con el main, para las pruebas, tengo un archivo llamado prueba.c
Este archivo, lo que hace es imprimir números desde el 10,000 al 1 (se puede modificar). Lo importante de esta prueba es que quede claro que tiene que tener estos procesos, lo que hago es usar memoria compartida para que cada vez que se habra un archivo, este mande su pid al main y este se encargue de meterlos en una cola.
Para que sea más sencillo estar trabajando en los diferentes procesos a agregar, me encargue de diseñar un archivo proceso.c y proceso.h, este archivo se encargará de obtener la memoria compartida, así como de mandar el pid del proceso, usando la función mandarPid y la función desvincular para liberar la memoria.

Con estas funciones ya se maneja el correcto funcionamiento:
### NOTA:
Para evitar errores, recomiendo ejectutar el programa y una vez que el main te indique que ya cuenta con el pid de ese proceso, ejecutar el siguiente.
El main iniciará a trabajar con los procesos, cuando se le indique (esto se puede cambiar, ya que utilizó hilos para que nada se interrumpa)

Para ejecutar prueba.c, compilar:
* gcc prueba.c proceso.c -o pru
* Ejecutar: ./pru

Siempre ejecutar primero el main (crea la memoria compartida) y después los demás procesos.

Chavitos, lo siento xd, ya se acabo, no tenia otra cosa que hacer xd
Para los programas que se crean hijos usando fork, hago uso de un semaforo con nombre para mandar correctamente los pid's 
Para el problema de los filosofos, como este consiste de un ciclo infinito, lo que hice fue que cuando reciba una señal SIGINT, termine el programa y mande la señal de terminado

Además, ya agregue una prueba de como sería usando mi código de Edgardo, sin embargo, no me gusta el resultado

Prepare un archivo sh para compilar los programas :)
