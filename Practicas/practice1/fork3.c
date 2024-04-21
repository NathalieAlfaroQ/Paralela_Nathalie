#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int status = 0;
  // Tipo de process id es pid_t
  pid_t pid = fork();
  if (pid == 0) {
    printf("Hello from the other side\n");
  } else {
    printf("LokinLindo.");
    printf("Hello, it's me\n");
    // process id - para sabe si el hijo falla o no - 0 buscar info
    // -1 en pid para elegir un fork aleatorio
    waitpid(pid, &status, 0);
    printf("LokinMalo.");
  }
  return 0;
}

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