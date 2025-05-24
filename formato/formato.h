/*
Autor: 
Versión 2.0
Descripción: Cabecera de la libreria para mostrar las colas de manera más agradable para el usuario

Observaciones: La implementación solo funciona para entornos que puedan leer caracteres de utf-8
	Funciones adapatadas para la simulación 2
*/

#include "../TADColaDin.h"

//DEFINICIONES DE CONSTANTES
#define TRUE	1
#define FALSE	0

//DEFINICIONES DE TIPOS DE DATO

//Definir un boolean (Se modela con un "char")
typedef unsigned char boolean;


//DECLARACIÓN DE FUNCIONES
void Encolar(cola * c, int i);	//Encolar : Recibe una cola y el numero de fila de esa cola e imprime la cola con el nuevo elemento
void Desencolar(cola * c, int i);			//Desencolar (Dequeue): Recibe una cola y el numero de fila de esa cola: recorre la fila para eliminar el primer elemento
void limpiarCola(int i);	//LimpiarCola: Recibe el indice de la cola a eliminar: Limpia en la cola mencionada
void limpiarLinea(int x, int y);	//LimpiarLinea: Recibe la posicion (x,y) y limpia en solo una linea de la cola
void MostrarCola(cola * c, int x); //MostrarCola: Recibe una cola y la posición en x en la que se imprimira la cola
void MostrarProceso(int pid, int pos);
