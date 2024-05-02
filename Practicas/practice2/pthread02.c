// Incrementa una variable global en un entorno de múltiples hilos (threads)

// Biblioteca pthread (POSIX Threads) que permite trabajar con hilos en sistemas operativos tipo UNIX
#include <pthread.h>
#include <stdio.h>

// Define una macro TOTAL_THREADS que representa el número total de hilos a crear
#define TOTAL_THREADS 16

// Varibale global
int global = 0;


/*
Punto de entrada para cada hilo
Recibe un argumento arg, que no se utiliza en este caso
Aumenta el valor de la variable global en 1000 unidades
*/
void *increment(void *arg) {
  int readGlobal;

  for (int i = 0; i < 1000; i++) {
    readGlobal = global;
    readGlobal++;
    global = readGlobal;
  } // End for

    // Termina el hilo actual. En este caso, no devuelve ningún valor
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

// Compilar   gcc -pthread -o pthread02 pthread02.c
// Ejecutar ./pthread02

/*
Para buscar errores de sincronizacion se usa Helgrind:
  gcc -pthread -g -o pthread02 pthread02.c
  valgrind --tool=helgrind ./pthread02

o ThreadSanitizer:
  gcc -pthread -fsanitize=thread -o pthread02gcc pthread02.c
  ./pthread02gcc

También pueden compilar con Clang:
  clang -pthread -fsanitize=thread -o pthread02clang pthread02.c
  ./pthread02clang

Compilen el programa anterior y ejecútenlo varias veces. ¿Qué observan? ¿Por
qué ocurre esto?

  Se observa que global before siempre es 0 y que el after puede aumentar o reducirse,
  esto debido a que no hay sincronizacion y puede ser que un hilo no haya terminado
  y ya el otro empiece, por eso es necesario los mutexes o semaforos. Ademas, que
  comparten memoria y eso afecta las variables globales.

    Global before: 0
    Global after: 14212
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread02 
    Global before: 0 
    Global after: 15716
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread02 
    Global before: 0 
    Global after: 15677
    alfanath@Alfa:~/paralelaEsteban/Paralela_Nathalie/Practicas/practice2$./pthread02 
    Global before: 0 
    Global after: 13354

Ejecuten el programa anterior con helgrind y reporten los resultados: Se reportan 28 errores con valgrind

    ==99359== Helgrind, a thread error detector
    ==99359== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
    ==99359== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==99359== Command: ./pthread02
    ==99359== 
    Global before: 0
    ==99359== ---Thread-Announcement------------------------------------------
    ==99359== 
    ==99359== Thread #4 was created
    ==99359==    at 0x498F9F3: clone (clone.S:76)
    ==99359==    by 0x49908EE: __clone_internal (clone-internal.c:83)
    ==99359==    by 0x48FE6D8: create_thread (pthread_create.c:295)
    ==99359==    by 0x48FF1FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
    ==99359==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==99359==    by 0x1092A3: main (pthread02.c:51)
    ==99359== 
    ==99359== ---Thread-Announcement------------------------------------------
    ==99359== 
    ==99359== Thread #3 was created
    ==99359==    at 0x498F9F3: clone (clone.S:76)
    ==99359==    by 0x49908EE: __clone_internal (clone-internal.c:83)
    ==99359==    by 0x48FE6D8: create_thread (pthread_create.c:295)
    ==99359==    by 0x48FF1FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
    ==99359==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==99359==    by 0x1092A3: main (pthread02.c:51)
    ==99359== 
    ==99359== ----------------------------------------------------------------
    ==99359== 
    ==99359== Possible data race during read of size 4 at 0x10C014 by thread #4
    ==99359== Locks held: none
    ==99359==    at 0x109202: increment (pthread02.c:23)
    ==99359==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==99359==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==99359==    by 0x498FA03: clone (clone.S:100)
    ==99359== 
    ==99359== This conflicts with a previous write of size 4 by thread #3
    ==99359== Locks held: none
    ==99359==    at 0x109212: increment (pthread02.c:25)
    ==99359==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==99359==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==99359==    by 0x498FA03: clone (clone.S:100)
    ==99359==  Address 0x10c014 is 0 bytes inside data symbol "global"
    ==99359== 
    ==99359== ----------------------------------------------------------------
    ==99359== 
    ==99359== Possible data race during write of size 4 at 0x10C014 by thread #4
    ==99359== Locks held: none
    ==99359==    at 0x109212: increment (pthread02.c:25)
    ==99359==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==99359==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==99359==    by 0x498FA03: clone (clone.S:100)
    ==99359== 
    ==99359== This conflicts with a previous write of size 4 by thread #3
    ==99359== Locks held: none
    ==99359==    at 0x109212: increment (pthread02.c:25)
    ==99359==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==99359==    by 0x48FEAC2: start_thread (pthread_create.c:442)
    ==99359==    by 0x498FA03: clone (clone.S:100)
    ==99359==  Address 0x10c014 is 0 bytes inside data symbol "global"
    ==99359== 
    Global after: 16000
    ==99359== 
    ==99359== Use --history-level=approx or =none to gain increased speed, at
    ==99359== the cost of reduced accuracy of conflicting-access information
    ==99359== For lists of detected and suppressed errors, rerun with: -s
    ==99359== ERROR SUMMARY: 28 errors from 2 contexts (suppressed: 270 from 18)

Ejecuten el programa anterior con ThreadSanitizer y reporten los errores detectados. ¿GCC reporta errores
diferentes que la versión compilada con Clang?:

  El GCC de ThreadSanitizer reporta 1 error:

    Global before: 0
    ==================
    WARNING: ThreadSanitizer: data race (pid=112055)
      Read of size 4 at 0x55e52bca9014 by thread T2:
        #0 increment <null> (pthread02gcc+0x12dc)

      Previous write of size 4 at 0x55e52bca9014 by thread T1:
        #0 increment <null> (pthread02gcc+0x12f8)

      Location is global 'global' of size 4 at 0x55e52bca9014 (pthread02gcc+0x000000004014)

      Thread T2 (tid=112058, running) created by main thread at:
        #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
        #1 main <null> (pthread02gcc+0x13a9)

      Thread T1 (tid=112057, finished) created by main thread at:
        #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
        #1 main <null> (pthread02gcc+0x13a9)

    SUMMARY: ThreadSanitizer: data race (/home/alfanath/paralelaEsteban/Paralela_Nathalie/Practicas/practice2/pthread02gcc+0x12dc) in increment
    ==================
    Global after: 15743
    ThreadSanitizer: reported 1 warnings

  El Clang reporta 1 error, igual que ThreadSanitizer:

    Global before: 0
    ==================
    WARNING: ThreadSanitizer: data race (pid=117851)
      Write of size 4 at 0x562df60fd258 by thread T2:
        #0 increment <null> (pthread02clang+0xcc154) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb)

      Previous write of size 4 at 0x562df60fd258 by thread T1:
        #0 increment <null> (pthread02clang+0xcc154) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb)

      Location is global 'global' of size 4 at 0x562df60fd258 (pthread02clang+0x1473258)

      Thread T2 (tid=117854, running) created by main thread at:
        #0 pthread_create <null> (pthread02clang+0x4d3bd) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb)
        #1 main <null> (pthread02clang+0xcc1f9) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb)

      Thread T1 (tid=117853, finished) created by main thread at:
        #0 pthread_create <null> (pthread02clang+0x4d3bd) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb)
        #1 main <null> (pthread02clang+0xcc1f9) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb)

    SUMMARY: ThreadSanitizer: data race (/home/alfanath/paralelaEsteban/Paralela_Nathalie/Practicas/practice2/pthread02clang+0xcc154) (BuildId: 8490df00a00f6813c97046aa474e14ffb521fabb) in increment
    ==================
    Global after: 16000
    ThreadSanitizer: reported 1 warnings
*/