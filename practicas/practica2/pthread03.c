// Incrementa una variable global en un entorno de múltiples hilos (threads)

// Biblioteca pthread (POSIX Threads) que permite trabajar con hilos en sistemas operativos tipo UNIX
#include <pthread.h>
#include <stdio.h>

// Define una macro TOTAL_THREADS que representa el número total de hilos a crear
#define TOTAL_THREADS 16

// Varibale global
int global = 0;

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

// Compilar  gcc -pthread -o pthread03 pthread03.c
// Ejecutar ./pthread03

/*
Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o pthread03 pthread03.c
  valgrind --tool=helgrind ./pthread03

Compilen y ejecuten el programa varias veces. Reporten ¿Qué observan? ¿Por qué ocurre esto?

  Al ejecutarlo varias veces se observa que algunas veces se llega al reusltado
  esperado y otras no ya que siguen compartiendo memoria y no hay mutex/semaforo
  para hacer sincronizacion y un hilo puede empezar a la vez que otro.

    Global before: 0
    Global after: 16000
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread03
    Global before: 0
    Global after: 15657
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread03
    Global before: 0
    Global after: 15512
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread03
    Global before: 0
    Global after: 15369
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread03
    Global before: 0
    Global after: 14956

  Al ejecutar con Valgrind, aun se presentan los 28 errores:

    ==146216== Helgrind, a thread error detector
    ==146216== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
    ==146216== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==146216== Command: ./pthread03
    ==146216== 
    Global before: 0
    ==146216== ---Thread-Announcement------------------------------------------
    ==146216== 
    ==146216== Thread #4 was created
    ==146216==    at 0x498F9F3: clone (clone.S:76)
    ==146216==    by 0x49908EE: __clone_internal (clone-internal.c:83)
    ==146216==    by 0x48FE6D8: create_thread (pthread_create.c:295)
    ==146216==    by 0x48FF1FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
    ==146216==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==146216==    by 0x10929C: main (pthread03.c:39)
    ==146216== 
    ==146216== ---Thread-Announcement------------------------------------------
    ==146216== 
    ==146216== Thread #3 was created
    ==146216==    at 0x498F9F3: clone (clone.S:76)
    ==146216==    by 0x49908EE: __clone_internal (clone-internal.c:83)
    ==146216==    by 0x48FE6D8: create_thread (pthread_create.c:295)
    ==146216==    by 0x48FF1FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
    ==146216==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==146216==    by 0x10929C: main (pthread03.c:39)
    ==146216== 
    ==146216== ----------------------------------------------------------------
    ==146216== 
    ==146216== Possible data race during read of size 4 at 0x10C014 by thread #4
    ==146216== Locks held: none
    ==146216==    at 0x109202: increment (pthread03.c:16)
    ==146216==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==146216==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==146216==    by 0x498FA03: clone (clone.S:100)
    ==146216== 
    ==146216== This conflicts with a previous write of size 4 by thread #3
    ==146216== Locks held: none
    ==146216==    at 0x10920B: increment (pthread03.c:16)
    ==146216==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==146216==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==146216==    by 0x498FA03: clone (clone.S:100)
    ==146216==  Address 0x10c014 is 0 bytes inside data symbol "global"
    ==146216== 
    ==146216== ----------------------------------------------------------------
    ==146216== 
    ==146216== Possible data race during write of size 4 at 0x10C014 by thread #4
    ==146216== Locks held: none
    ==146216==    at 0x10920B: increment (pthread03.c:16)
    ==146216==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==146216==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==146216==    by 0x498FA03: clone (clone.S:100)
    ==146216== 
    ==146216== This conflicts with a previous write of size 4 by thread #3
    ==146216== Locks held: none
    ==146216==    at 0x10920B: increment (pthread03.c:16)
    ==146216==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==146216==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==146216==    by 0x498FA03: clone (clone.S:100)
    ==146216==  Address 0x10c014 is 0 bytes inside data symbol "global"
    ==146216== 
    Global after: 16000
    ==146216== 
    ==146216== Use --history-level=approx or =none to gain increased speed, at
    ==146216== the cost of reduced accuracy of conflicting-access information
    ==146216== For lists of detected and suppressed errors, rerun with: -s
    ==146216== ERROR SUMMARY: 28 errors from 2 contexts (suppressed: 270 from 18)
*/