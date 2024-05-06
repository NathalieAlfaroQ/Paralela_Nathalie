// EJERCICIO 4 buffer

#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_THREADS 50000  // Número máximo de hilos permitidos
pthread_mutex_t mutex;  // Candado para sincronizar

// Variables globales:
int total_threads;            // Número total de hilos a usar
int points_inside_circle = 0; // Cuenta los puntos dentro del círculo
int total_points;             // Número total de puntos a lanzar

/*
Simula el lanzamiento de dardos en un cuadrado unitario y cuenta cuántos caen dentro del círculo.

Toma un puntero void como argumento y devuelve un puntero void.

Esto permite que la función sea utilizada como un hilo de ejecución (pthread)
ya que esta API requiere que la función de inicio sea de tipo void* (*)(void*).
*/
void *throw_darts(void *arg)
{
  int *localPoints = (int *) malloc(sizeof(int));
  *localPoints = 0;

  /* Argumento arg se convierte a un entero sin signo del tamaño de un puntero
  (uintptr_t) Para identificar de manera única cada hilo */
  int childID = (uintptr_t)arg;

  // Calcula cuántos puntos se asignarán a este hilo
  int points_per_thread = total_points / total_threads;

  /*
  Utiliza el tiempo actual multiplicado por el identificador del hilo más
  uno, para inicializar la semilla para la generación de números aleatorios.

  time(NULL) devuelve los segundos transcurridos desde el 1 de enero de 1970
  */
  unsigned int seed = time(NULL) * (childID + 1);

  for (int i = 0; i < points_per_thread; i++) {
    /* Genera números aleatorios 'x' y 'y' entre 0 y 1 con rand_r(&seed)
    dividido por RAND_MAX, que normaliza el resultado a un valor entre 0 y 1 */
    double x = (double)rand_r(&seed) / RAND_MAX;
    double y = (double)rand_r(&seed) / RAND_MAX;

    /* Comprueba si el punto (x, y) generado cae dentro del círculo unitario.
    Esto se hace calculando la distancia desde el origen (0, 0) hasta el
    punto (x, y) con el teorema de Pitágoras y comparando si esta
    distancia es menor o igual a 1 (el radio del círculo) */
    if (sqrt(x * x + y * y) <= 1) {
        (*localPoints)++;
    } // End if
  }   // End for

  pthread_exit(localPoints);
  return NULL;
} // End throw_darts

int main(int argc, char *argv[])
{
  struct timespec before;
  clock_gettime(CLOCK_MONOTONIC, &before);
  pthread_t threads[MAX_THREADS];
  int *final[MAX_THREADS];

  // Se comprueba si se proporcionan los argumentos necesarios
  if (argc != 3) {
    printf("Uso: %s <total_points> <total_threads>\n", argv[0]);
    return 1;
  } // End if

  // Se almacenan los valores proporcionados por el usuario
  total_points = atoi(argv[1]);
  total_threads = atoi(argv[2]);

  // Se verifica si el número de hilos es mayor que MAX_THREADS
  if (total_threads > MAX_THREADS) {
    printf("El número de hilos no puede ser mayor a %d\n", MAX_THREADS);
    return 1;
  } // End if

  // Se verifica si el número de puntos es divisible entre el número de hilos
  if (total_points % total_threads != 0) {
    printf("El número de puntos debe ser divisible entre el número de hilos\n");
    return 1;
  } // End if

  ////////////// Acá deberán introducir el paralelismo //////////////
  
  pthread_mutex_init(&mutex, NULL);

  // Crear hilos y verificar que se hayan creado bien
  for (int i = 0; i < total_threads; i++) {
    if (pthread_create(&threads[i], NULL, throw_darts, &i) != 0) {
      perror("Error creating thread\n");
      return 1;
    } // End if
  } // End for

  for (int i = 0; i < total_threads; i++) {
    pthread_join(threads[i], (void **)&final[i]);
    points_inside_circle += *final[i];
    free(final[i]);
  } // End for

  // Se calcula el valor de π utilizando la fórmula de Monte Carlo
  double pi = 4.0 * points_inside_circle / total_points;
  printf("\nValor de pi: %f\n", pi);

  struct timespec after;
  clock_gettime(CLOCK_MONOTONIC, &after);
  long secs = after.tv_sec - before.tv_sec;
  uint64_t nanosecs = ((after.tv_sec * 1000000000) + after.tv_nsec) - ((before.tv_sec * 1000000000) + before.tv_nsec);
  printf("%ld sec, %ld nanosecs. \n\n", secs, nanosecs);
  pthread_mutex_destroy(&mutex);
  return 0;
} // End main

/*
Compilar gcc -pthread -o ejercicio4  ejercicio4.c -lm
Ejecutar time ./ejercicio4 100000000 4

Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o ejercicio4 ejercicio4.c -lm
  valgrind --tool=helgrind time ./ejercicio4 100000000 4


valgrind --tool=helgrind time ./ejercicio4 100000000 4
==360484== Helgrind, a thread error detector
==360484== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==360484== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==360484== Command: time ./ejercicio4 100000000 4
==360484== 

Valor de pi: 3.142042
0 sec, 846593582 nanosecs. 

3.27user 0.01system 0:00.86elapsed 380%CPU (0avgtext+0avgdata 61424maxresident)k
0inputs+0outputs (1major+1076minor)pagefaults 0swaps
==360484== 
==360484== Use --history-level=approx or =none to gain increased speed, at
==360484== the cost of reduced accuracy of conflicting-access information
==360484== For lists of detected and suppressed errors, rerun with: -s
==360484== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio4 100000000 10
==360601== Helgrind, a thread error detector
==360601== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==360601== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==360601== Command: time ./ejercicio4 100000000 10
==360601== 

Valor de pi: 3.141437
1 sec, 593257648 nanosecs. 

3.96user 0.02system 0:00.61elapsed 647%CPU (0avgtext+0avgdata 61424maxresident)k
0inputs+0outputs (0major+1098minor)pagefaults 0swaps
==360601== 
==360601== Use --history-level=approx or =none to gain increased speed, at
==360601== the cost of reduced accuracy of conflicting-access information
==360601== For lists of detected and suppressed errors, rerun with: -s
==360601== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio4 100000000 20
==360694== Helgrind, a thread error detector
==360694== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==360694== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==360694== Command: time ./ejercicio4 100000000 20
==360694== 

Valor de pi: 3.139866
0 sec, 548201686 nanosecs. 

4.21user 0.01system 0:00.56elapsed 745%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+1126minor)pagefaults 0swaps
==360694== 
==360694== Use --history-level=approx or =none to gain increased speed, at
==360694== the cost of reduced accuracy of conflicting-access information
==360694== For lists of detected and suppressed errors, rerun with: -s
==360694== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

Entre mas hilos, menos precision tiene el resultado
*/