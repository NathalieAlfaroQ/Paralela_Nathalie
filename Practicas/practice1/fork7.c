#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Process start\n");
  int total_forks = 10;
  // Asignar memoria de 100 bytes a un puntero de tipo char
  char *msg = malloc(100);
  // Escribir en el buffer
  snprintf(msg, 100, "I'm the parent process!!!\n");
  // Muestra la direccion de memoria y su valor almacenado
  printf("[Parent] The address of msg is: %p | Value is: %s\n", msg, msg);

  // Ciclo para crear hijos
  for (int i = 0; i < total_forks; i++) {
    // Para hacer los hijos
    pid_t pid = fork();

    // Hijo
    if (pid == 0) {
      // Sobreescribe el buffer
      snprintf(msg, 100, "hello from child %d", i);
      // Muestra la direccion de memoria y su valor almacenado
      printf("[Child %d] The address of msg is: %p | Value is: %s\n", i, msg,
             msg);
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
1. ¿La dirección de memoria de msg es la misma en el proceso padre y en los
procesos hijos? Si.

2. ¿Porqué?
        Porque fork lo que hace es una copia del programa, entonces tambien se
copia la direccion.
*/