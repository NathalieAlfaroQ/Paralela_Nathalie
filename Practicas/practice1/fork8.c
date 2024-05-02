#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Process start\n");
  int total_forks = 100;
  // Asignar memoria
  char *msg = malloc(1024 * 1024 * 1024);
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
1. ¿Cuánta memoria gasta el programa (incluyendo cada proceso)?
  Memoria asignada para msg en el proceso padre: 1 GB (1024 * 1024 * 1024 bytes).

  Memoria asignada para msg en cada proceso hijo: 100 bytes (debido a snprintf(msg, 100, "hello from child %d", i);).

  El programa utilizará al menos 1 GB de memoria más la memoria adicional para el proceso padre y cada proceso hijo.

2. ¿Estamos gastando 100GB de memoria o no?
    No.
*/