// EJERCICIO 1 Y 2

#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define MAX_THREADS 50000  // Número máximo de hilos permitidos
pthread_mutex_t mutex;  // Candado para sincronizar

// Variables globales:
int total_threads;            // Número total de hilos a usar
int points_inside_circle = 0; // Cuenta los puntos dentro del círculo
int total_points;             // Número total de puntos a lanzar

struct msg_buffer {  // Estructura para los mensajes de la cola
  int points;
} message;

/*
Simula el lanzamiento de dardos en un cuadrado unitario y cuenta cuántos caen dentro del círculo.

Toma un puntero void como argumento y devuelve un puntero void.

Esto permite que la función sea utilizada como un hilo de ejecución (pthread)
ya que esta API requiere que la función de inicio sea de tipo void* (*)(void*).
*/
void *throw_darts(void *arg)
{
  int localPoints = 0;
  key_t key;     // Inicia llave de la cola y el mensaje ID
  int msgid;

  // Clave unica para la cola de mensajes
  key = ftok("queuefile", 65);

  // Conectar a la cola de mensajes existentes y obtenemos su ID
  msgid = msgget(key, 0666 | IPC_CREAT);

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
  
  // Guarda info
  struct msg_buffer msg;

  // Mensaje con los puntos que hace cada hilo
  msg.points = localPoints;

  pthread_mutex_lock(&mutex);

  // Envia mensaje
  msgsnd(msgid, &msg, sizeof(message), 0);

  // Punto enviado
  printf("\nMensaje enviado: %i", msg.points);

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
  key_t key;
  int msgid;
  key = ftok("queuefile", 65);
  msgid = msgget(key, 0666 | IPC_CREAT);
  pthread_mutex_init(&mutex, NULL);

  // Crear hilos y verificar que se hayan creado bien
  for (int i = 0; i < total_threads; i++) {
    if (pthread_create(&threads[i], NULL, throw_darts, NULL) != 0) {
      perror("Error creating thread\n");
      return 1;
    } // End if
  } // End for

  for (int i = 0; i < total_threads; i++) {
    pthread_join(threads[i], NULL);
  } // End for

  points_inside_circle = 0;

  // Obtener los mensajes
  for (int i = 0; i < total_threads; i++) {
    // Recibe el mensaje
    msgrcv(msgid, &message, sizeof(message), 0, 0);
    points_inside_circle += message.points;
  } // End for

  // Se calcula el valor de π utilizando la fórmula de Monte Carlo
  double pi = 4.0 * points_inside_circle / total_points;
  printf("\nValor de pi: %f\n", pi);
  struct timespec after;
  clock_gettime(CLOCK_MONOTONIC, &after);
  long secs = after.tv_sec - before.tv_sec;
  uint64_t nanosecs = ((after.tv_sec * 1000000000) + after.tv_nsec) - ((before.tv_sec * 1000000000) + before.tv_nsec);
  printf("%ld sec, %ld nanosecs. \n\n", secs, nanosecs);
  msgctl(msgid, IPC_RMID, NULL);
  pthread_mutex_destroy(&mutex);
  return 0;
} // End main

/*
Compilar gcc -pthread -o ejercicio1  ejercicio1.c -lm
Ejecutar time ./ejercicio1 100000000 4

Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o ejercicio1 ejercicio1.c -lm
  valgrind --tool=helgrind time ./ejercicio1 100000000 4

==346988== Helgrind, a thread error detector
==346988== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==346988== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==346988== Command: time ./ejercicio1 100000000 4
==346988== 

Mensaje enviado: 19635719
Mensaje enviado: 19635719
Mensaje enviado: 19635719
Mensaje enviado: 19635719
Valor de pi: 3.141715
0 sec, 583901225 nanosecs. 

2.16user 0.01system 0:00.60elapsed 362%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+963minor)pagefaults 0swaps
==346988== 
==346988== Use --history-level=approx or =none to gain increased speed, at
==346988== the cost of reduced accuracy of conflicting-access information
==346988== For lists of detected and suppressed errors, rerun with: -s
==346988== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio1 200000000 10
==349860== Helgrind, a thread error detector
==349860== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==349860== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==349860== Command: time ./ejercicio1 200000000 10
==349860== 

Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Mensaje enviado: 15708033
Valor de pi: 3.141607
1 sec, 864661059 nanosecs. 

5.81user 0.02system 0:00.87elapsed 664%CPU (0avgtext+0avgdata 61420maxresident)k
0inputs+0outputs (0major+975minor)pagefaults 0swaps
==349860== 
==349860== Use --history-level=approx or =none to gain increased speed, at
==349860== the cost of reduced accuracy of conflicting-access information
==349860== For lists of detected and suppressed errors, rerun with: -s
==349860== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



valgrind --tool=helgrind time ./ejercicio1 100000000 1
==350400== Helgrind, a thread error detector
==350400== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==350400== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==350400== Command: time ./ejercicio1 100000000 1
==350400== 

Mensaje enviado: 78538199
Valor de pi: 3.141528
2 sec, 1860689193 nanosecs. 

1.85user 0.00system 0:01.87elapsed 99%CPU (0avgtext+0avgdata 61416maxresident)k
0inputs+0outputs (0major+958minor)pagefaults 0swaps
==350400== 
==350400== Use --history-level=approx or =none to gain increased speed, at
==350400== the cost of reduced accuracy of conflicting-access information
==350400== For lists of detected and suppressed errors, rerun with: -s
==350400== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

Con menos hilos, se dura mas.
*/