#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Número máximo de hilos permitidos
#define MAX_THREADS 50000

// Variables globales:
int total_threads;            // Número total de hilos a usar
int points_inside_circle = 0; // Cuenta los puntos dentro del círculo
int total_points;             // Número total de puntos a lanzar

// Simula el lanzamiento de dardos en un cuadrado unitario y cuenta cuántos caen dentro del círculo
/*
Toma un puntero void como argumento y devuelve un puntero void

Esto permite que la función sea utilizada como un hilo de ejecución (pthread)
ya que esta API requiere que la función de inicio sea de tipo void* (*)(void*).
*/
void *throw_darts(void *arg) {
  // Argumento arg se convierte a un entero sin signo del tamaño de un puntero (uintptr_t)
  // Para identificar de manera única cada hilo
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
    /*
    Genera números aleatorios 'x' y 'y' entre 0 y 1 con rand_r(&seed) 
    dividido por RAND_MAX, que normaliza el resultado a un valor entre 0 y 1.
    */
    double x = (double)rand_r(&seed) / RAND_MAX;
    double y = (double)rand_r(&seed) / RAND_MAX;

    /*
    Comprueba si el punto (x, y) generado cae dentro del círculo unitario.
    Esto se hace calculando la distancia desde el origen (0, 0) hasta el 
    punto (x, y) con el teorema de Pitágoras y comparando si esta
    distancia es menor o igual a 1 (el radio del círculo).
    */
    if (sqrt(x * x + y * y) <= 1) {
      points_inside_circle++;
    } // End if
  } // End for
  return NULL;
} // End throw_darts

int main(int argc, char *argv[]) {

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

  // Acá deberán introducir el paralelismo

  // Llamado a la funcion
  throw_darts((void *)1);

  // Se calcula el valor de π utilizando la fórmula de Monte Carlo
  double pi = 4.0 * points_inside_circle / total_points;
  printf("Valor de pi: %f\n", pi);
  return 0;
} // End main