// Mutex para sincronizar el acceso a la variable global

#include <pthread.h>
#include <stdio.h>

#define TOTAL_THREADS 16

int global = 0;

// Candado
pthread_mutex_t mutex;

void *increment(void *arg) {
  for (int i = 0; i < 1000; i++) {
    // Retorna el identificador del hilo que la llama
    printf("Thread %ld\n", pthread_self());
    // Inicia la sección crítica
    pthread_mutex_lock(&mutex);
    // Realizamos las operaciones que NO pueden ser realizadas de forma concurrente.
    global++;
    // Finaliza la sección crítica
    pthread_mutex_unlock(&mutex);
  } // eND FOR
  pthread_exit(NULL);
} // End increment

int main(void) {
  pthread_t threads[TOTAL_THREADS];
  pthread_mutex_init(&mutex, NULL);
  printf("Global before: %d\n", global);

  for (int i = 0; i < TOTAL_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, increment, NULL) != 0) {
      perror("Error creating thread\n");
      return 1;
    } // End if
  } // End for

  for (int i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }  // End for

  pthread_mutex_destroy(&mutex);
  printf("Global after: %d\n", global);
  return 0;
} // End main

// Compilar  gcc -pthread -o pthread03mutex pthread03mutex.c
// Ejecutar ./pthread03mutex

/*
Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o pthread03mutex pthread03mutex.c
  valgrind --tool=helgrind ./pthread03mutex

Compilen y ejecuten el programa varias veces. Reporten los resultados.

    Al ejecutarlo varias veces se observa que da el resultado esperado:
    
        Global before: 0
        Global after: 16000
        alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$ ./pthread03mutex
        Global before: 0
        Global after: 16000
        alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$ ./pthread03mutex
        Global before: 0
        Global after: 16000
        alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$ ./pthread03mutex
        Global before: 0
        Global after: 16000

    Al ejecutarlo con Valgrind se observa que no hay errores:

        ==149854== Helgrind, a thread error detector
        ==149854== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
        ==149854== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
        ==149854== Command: ./pthread03mutex
        ==149854== 
        Global before: 0
        Global after: 16000
        ==149854== 
        ==149854== Use --history-level=approx or =none to gain increased speed, at
        ==149854== the cost of reduced accuracy of conflicting-access information
        ==149854== For lists of detected and suppressed errors, rerun with: -s
        ==149854== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 42326 from 25)

Descomenten la línea printf("Thread%ld\n", pthread_self()); y ejecuten el programa varias veces. ¿Qué observan?

    Al ejecutarlo varias veces se observa que si da el resultado esperado y muestra tambien el identificador
    del hilo.

    Al ejecutarlo con Valgrind se observa que hay 2157 errores:

        ...
        Thread 115971648
        Thread 115971648
        Thread 115971648
        Global after: 16000
        ==153742== 
        ==153742== Use --history-level=approx or =none to gain increased speed, at
        ==153742== the cost of reduced accuracy of conflicting-access information
        ==153742== For lists of detected and suppressed errors, rerun with: -s
        ==153742== ERROR SUMMARY: 2157 errors from 3 contexts (suppressed: 133418 from 90)
*/