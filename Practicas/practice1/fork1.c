#include <stdio.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();
  if (pid == 0) {
    printf("Hello from the other side\n");
  } else {
    printf("Hello, it's me\n");
  }
  return 0;
}

// Compilar gcc -o fork1 fork1.c
// Ejecutar ./fork1

/*
1. ¿Se imprimen en el orden esperado?
  Si se imprime en el orden esperado ya que el fork hace una copia del
	proceso y toma la primera como el padre y la copia como el hijo dandole retorno
	0 a este y creando un PID para el padre.

2. ¿Siempre se imprimen en el mismo orden?
  Si.

3. ¿A sus compañeros les sucede lo mismo?
  Si.
*/