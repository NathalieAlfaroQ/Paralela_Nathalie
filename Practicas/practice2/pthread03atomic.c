// Incrementa una variable global en un entorno de múltiples hilos (threads)

// Biblioteca pthread (POSIX Threads) que permite trabajar con hilos en sistemas operativos tipo UNIX
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

// Define una macro TOTAL_THREADS que representa el número total de hilos a crear
#define TOTAL_THREADS 16

// Varibale global
atomic_int global = 0;

// Intento de atomica
void *increment(void *arg) {
  for (int i = 0; i < 1000; i++) {
    global++;
  } // End for
  pthread_exit(NULL);
} // End increment

int main(void) {
  // Array de identificadores de hilos para almacenar los identificadores de los hilos que se crearán
  pthread_t threads[TOTAL_THREADS];
  // Imprime el valor de la variable global antes de que se realicen los incrementos
  printf("Global before: %d\n", global);

  // Itera sobre el número total de hilos que se crearán.
  for (int i = 0; i < TOTAL_THREADS; i++) {

    /*
    Intenta crear un nuevo hilo.
    La función pthread_create recibe 4 argumentos:
        - una referencia al identificador del hilo,
        - atributos del hilo (en este caso, NULL para usar los valores por defecto),
        - la función que se ejecutará en el hilo (en este caso, increment),
        - y un argumento para esa función (también NULL en este caso).
    Si hay un error al crear el hilo, se imprime un mensaje de error y se devuelve 1 (indicando un fallo).
    */
    if (pthread_create(&threads[i], NULL, increment, NULL) != 0) {
      perror("Error creating thread\n");
      return 1;
    } // End if
  }   // End for

  // Itera sobre todos los hilos creados y espera a que cada hilo termine su ejecución antes de continuar
  for (int i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], NULL);
  } // End for

  // Imprime el valor final de la variable global después de que todos los hilos han terminado.
  printf("Global after: %d\n", global);
  return 0;
} // End main

// Compilar  gcc -pthread -o pthread03atomic pthread03atomic.c
// Ejecutar ./pthread03atomic

/*
 Compilen y ejecuten el programa varias veces. Reporten ¿Qué observan? ¿Se solucionó el problema?

 Al ejecutarlo varias veces se observa que se llega al resultado esperado
 gracias a la funcionalidad de las operaciones atomicas que no tienen posibilidad de interrupcion:

 Global before: 0
 Global after: 16000
 alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread03atomic
 Global before: 0
 Global after: 16000
 alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread03atomic
 Global before: 0
 Global after: 16000
*/