#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(void) {
  printf("Process start\n");
  int total_forks = 3;
  // Asignar memoria
  char *msg = malloc(1024 * 1024 * 1024);
  memset(msg, 3, 1024 * 1024 * 1024);

  // Escribir el buffer
  snprintf(msg, 100, "I'm the parent process!!!\n");
  // Mostrar su direccion de memoria y su valor
  printf("[Parent] The address of msg is: %p | Value is: %s\n", msg, msg);

  // Ciclo para crear hijos
  for (int i = 0; i < total_forks; i++) {
    // Llamado a copiar
    pid_t pid = fork();

    // Hijo
    if (pid == 0) {
      // Sobreescribe el buffer
      snprintf(msg, 100, "hello from child %d", i);
      memset(msg, 3, 1024 * 1024 * 1024);

      // Mostrar su direccion de memoria y su valor
      printf("[Child %d] The address of msg is: %p | Value is: %s\n", i, msg,
             msg);

      sleep(10);
      return 0;
    } // End if
  }   // End for

  // Ciclo para esperar cada hijo
  for (int i = 0; i < total_forks; i++) {
    wait(NULL);
  } // End for

  printf("[Parent] The program ends\n");
  printf("[Parent] The final value of msg is: %s\n", msg);
  return 0;
} // End main

/*
El resultado se tardo mucho, de las 3 GB que tenia, las use para este programa y viendo el task manager vi
que lo que quedaba disponible era un 1 GB a 966 MB, esto fue lo que imprimio:

Process start
[Parent] The address of msg is: 0x7f4a38be2010 | Value is: I'm the parent process!!!

[Child 1] The address of msg is: 0x7f4a38be2010 | Value is: [Child 2] The address of msg is: 0x7f4a38be2010 | Value is: [Child 0] The address of msg is: 0x7f4a38be2010 | Value is: 


[Parent] The program ends
[Parent] The final value of msg is: I'm the parent process!!!

 1. ¿Cuánta memoria gasta el programa (incluyendo cada proceso)?
    3 GB.

 2. ¿Ahora sí se gasta más memoria?
    Si.

 3. ¿Porqué?
    Por el memset que llena la memoria.

 4. ¿Colgó la computadora? ¿Porqué?
    No, solo se tardo bastante en dar el resultado.
    Por la poca memoria disponible.
*/