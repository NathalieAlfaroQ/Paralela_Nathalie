// EJERCICIO 3 PIPE

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
  int *pipefds = (int *)arg;  // Toma un puntero generico y lo pasa a puntero int
  int localPoints = 0;

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
      localPoints++;
    } // End if
  }   // End for

  pthread_mutex_lock(&mutex);
  write(pipefds[1], &localPoints, sizeof(localPoints)); // Escribir en el pipe para que otro proceso lo lea
  printf("\nPuntos en cola: %i", localPoints);
  pthread_mutex_unlock(&mutex);
  return NULL;
} // End throw_darts

int main(int argc, char *argv[])
{
  struct timespec before;
  clock_gettime(CLOCK_MONOTONIC, &before);
  pthread_t threads[MAX_THREADS];

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

  int pipefds[2];
  if (pipe(pipefds) == -1) {  // Crear pipe y verifica que se haya creado bien
    perror("Error Pipe");
    exit(EXIT_FAILURE);
  } // End if
  
  pthread_mutex_init(&mutex, NULL);

  // Crear hilos y verificar que se hayan creado bien
  for (int i = 0; i < total_threads; i++) {
    if (pthread_create(&threads[i], NULL, throw_darts, (void *)pipefds) != 0) {
      perror("Error creating thread\n");
      return 1;
    } // End if
  } // End for

  for (int i = 0; i < total_threads; i++) {
    pthread_join(threads[i], NULL);
  } // End for

  points_inside_circle = 0;
  close(pipefds[1]);

  for (int i = 0; i < total_threads; i++) {
    int lp = 0;
    read(pipefds[0], &lp, sizeof(lp));
    points_inside_circle = points_inside_circle + lp;
  } // End for

  close(pipefds[0]);

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
Compilar gcc -pthread -o ejercicio3  ejercicio3.c -lm
Ejecutar time ./ejercicio3 100000000 4

Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o ejercicio3 ejercicio3.c -lm
  valgrind --tool=helgrind time ./ejercicio3 100000000 4

==358769== Helgrind, a thread error detector
==358769== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==358769== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==358769== Command: time ./ejercicio3 100000000 4
==358769== 

Puntos en cola: 19637452
Puntos en cola: 19637452
Puntos en cola: 19637452
Puntos en cola: 19637452
Valor de pi: 3.141992
1 sec, 794797738 nanosecs. 

3.02user 0.02system 0:00.80elapsed 377%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+967minor)pagefaults 0swaps
==358769== 
==358769== Use --history-level=approx or =none to gain increased speed, at
==358769== the cost of reduced accuracy of conflicting-access information
==358769== For lists of detected and suppressed errors, rerun with: -s
==358769== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio3 200000000 10
==358923== Helgrind, a thread error detector
==358923== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==358923== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==358923== Command: time ./ejercicio3 200000000 10
==358923== 

Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Puntos en cola: 15705988
Valor de pi: 3.141198
2 sec, 1137188677 nanosecs. 

7.74user 0.06system 0:01.20elapsed 648%CPU (0avgtext+0avgdata 61424maxresident)k
0inputs+0outputs (0major+977minor)pagefaults 0swaps
==358923== 
==358923== Use --history-level=approx or =none to gain increased speed, at
==358923== the cost of reduced accuracy of conflicting-access information
==358923== For lists of detected and suppressed errors, rerun with: -s
==358923== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio3 300000000 1
==359071== Helgrind, a thread error detector
==359071== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==359071== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==359071== Command: time ./ejercicio3 300000000 1
==359071== 

Puntos en cola: 235617251
Valor de pi: 3.141563
7 sec, 7561438194 nanosecs. 

7.54user 0.01system 0:07.58elapsed 99%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+963minor)pagefaults 0swaps
==359071== 
==359071== Use --history-level=approx or =none to gain increased speed, at
==359071== the cost of reduced accuracy of conflicting-access information
==359071== For lists of detected and suppressed errors, rerun with: -s
==359071== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio3 800000000 40
==359214== Helgrind, a thread error detector
==359214== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==359214== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==359214== Command: time ./ejercicio3 800000000 40
==359214== 

Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Puntos en cola: 15710349
Valor de pi: 3.142070
4 sec, 4238228426 nanosecs. 

33.10user 0.17system 0:04.25elapsed 782%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+1040minor)pagefaults 0swaps
==359214== 
==359214== Use --history-level=approx or =none to gain increased speed, at
==359214== the cost of reduced accuracy of conflicting-access information
==359214== For lists of detected and suppressed errors, rerun with: -s
==359214== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


Entre mas puntos, mas dura.
*/