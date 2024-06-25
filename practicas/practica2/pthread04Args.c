// Paso de argumentos a un hilo
// Crea un hilo para sumar dos números enteros y luego imprime el resultado

#include <pthread.h>
#include <stdio.h>

#define TOTAL_THREADS 16 // Para hacer 16 hilos
pthread_mutex_t mutex; // Candado de sincronizacion

/*
Se define una estructura llamada arguments que contiene dos campos enteros, a y
b. para pasar los argumentos a la función sum que se ejecutará en un hilo separado.
*/
struct arguments 
{
  int a;
  int b;
};

/*
Función que toma un argumento de tipo void * y lo convierte a un puntero a
struct arguments. Luego, suma los valores de a y b de la estructura pasada como
argumento, imprime el resultado y sale del hilo utilizando pthread_exit.
*/
void *sum(void *arg) 
{
  pthread_mutex_lock(&mutex);
  struct arguments *args = (struct arguments *)arg;
  int result = args->a + args->b;
  printf("Result: %d\n", result);
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
} // End sum

int main(void) 
{
  // Declara hilo
  pthread_t threads[TOTAL_THREADS];

  // Array de struct arguments para 16 hilos
  struct arguments args[TOTAL_THREADS] = {
      {1, 2},   {3, 4},   {5, 6},   {7, 8},   {9, 10},  {20, 21},
      {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33},
      {34, 35}, {36, 37}, {38, 39}, {40, 41}};

  pthread_mutex_init(&mutex, NULL);

  // Hacer 16 hilos y verificar que se hayan creado
  for (int i = 0; i < TOTAL_THREADS; i++)
  { 
    if (pthread_create(&threads[i], NULL, sum, &args[i]) != 0) 
    {
      perror("Error creating thread\n");
      return 1;
    } // End if
  } // End for

  // Esperar a que los hilos terminen la ejecución para cerrarlos y terminar el programa
  for (int i = 0; i < TOTAL_THREADS; i++) {
    pthread_join(threads[i], NULL);
  } // End for

  // Eliminar mutex
  pthread_mutex_destroy(&mutex);

  return 0;
} // End main

// Compilar  gcc -pthread -o pthread04Args pthread04Args.c
// Ejecutar ./pthread04Args

/*
Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o pthread04Args pthread04Args.c
  valgrind --tool=helgrind ./pthread04Args

Compilen y ejecuten el programa varias veces. Reporten los resultados.

    Al ejejcutarlo se obtienen 16 resultados de las sumas:
      Result: 3
      Result: 15
      Result: 19
      Result: 11
      Result: 7
      Result: 45
      Result: 41
      Result: 49
      Result: 53
      Result: 57
      Result: 65
      Result: 61
      Result: 69
      Result: 73
      Result: 77
      Result: 81

    Al ejecutarlo con Valgrind se obtienen 0 errores:
      ==175077== Helgrind, a thread error detector
      ==175077== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
      ==175077== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
      ==175077== Command: ./pthread04Args
      ==175077== 
      Result: 3
      Result: 7
      Result: 11
      Result: 15
      Result: 19
      Result: 41
      Result: 45
      Result: 49
      Result: 53
      Result: 57
      Result: 61
      Result: 65
      Result: 69
      Result: 73
      Result: 77
      Result: 81
      ==175077== 
      ==175077== Use --history-level=approx or =none to gain increased speed, at
      ==175077== the cost of reduced accuracy of conflicting-access information
      ==175077== For lists of detected and suppressed errors, rerun with: -s
      ==175077== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 368 from 25)
*/