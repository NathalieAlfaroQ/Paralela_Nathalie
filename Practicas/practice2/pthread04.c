// Paso de argumentos a un hilo
// Crea un hilo para sumar dos números enteros y luego imprime el resultado

#include <pthread.h>
#include <stdio.h>

/*
Se define una estructura llamada arguments que contiene dos campos enteros, a y b.
para pasar los argumentos a la función sum que se ejecutará en un hilo separado.
*/
struct arguments {
  int a;
  int b;
};

/*
Función que toma un argumento de tipo void * y lo convierte a un puntero a struct arguments.
Luego, suma los valores de a y b de la estructura pasada como argumento, imprime el resultado
y sale del hilo utilizando pthread_exit.
*/
void *sum(void *arg) {
  struct arguments *args = (struct arguments *)arg;
  int result = args->a + args->b;
  printf("Result: %d\n", result);
  pthread_exit(NULL);
} // End sum

int main(void) {
  // Declara hilo
  pthread_t thread;
  // Se crea una instancia de la estructura arguments llamada args con los valores 42 y 23.
  struct arguments args = {42, 23};

  // Crear hilo: direccion hilo, null atributos del hilo, funcion a ejejcutar en el hilo, direccion de la estructura
  // Se verifica que se haya podido crear el hilo
  if (pthread_create(&thread, NULL, sum, &args) != 0) {
    perror("Error creating thread\n");
    return 1;
  } // End if

  // Esperar al que hilo termine
  pthread_join(thread, NULL);
  return 0;
} // End main

// Compilar  gcc -pthread -o pthread04 pthread04.c
// Ejecutar ./pthread04

/*
Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o pthread04 pthread04.c
  valgrind --tool=helgrind ./pthread04

Compilen y ejecuten el programa anterior. Reporten los resultados.

    Al ejejcutarlo se obtiene:
        Result: 65
    
    Al ejecutarlo con Valgrind se obtienen 0 errores:

        ==166565== Helgrind, a thread error detector
        ==166565== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
        ==166565== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
        ==166565== Command: ./pthread04
        ==166565== 
        Result: 65
        ==166565== 
        ==166565== Use --history-level=approx or =none to gain increased speed, at
        ==166565== the cost of reduced accuracy of conflicting-access information
        ==166565== For lists of detected and suppressed errors, rerun with: -s
        ==166565== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/