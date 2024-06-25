#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int status = 0;
  pid_t pid = fork();

  if (pid == 0) {
    printf("HIJO\n");
  } else {
    // Inicia el proceso padre
    printf("Inicia el proceso padre.");
    printf("PADRE\n");
    // process id - para saber si el hijo falla o no - 0 buscar info
    // -1 en pid para elegir un fork aleatorio
    // wait es para esperar
    waitpid(pid, &status, 0);
    // Termina proceso padre
    printf("Termina proceso padre.");
  } // End if
  return 0;
} // End main

/*
1. ¿Se imprimen en el orden esperado?
  Si.

2. ¿Cómo imprime cada proceso?
  Se imprime el inicio del padre, pero cuando llega al wait le dice al padre
	que se duerma para que el hijo se ejecute completamente y luego vuelve al padre
  para finalizarlo.

3. ¿Se ejecuta el proceso hijo antes o después del proceso padre?
  Durante.
*/