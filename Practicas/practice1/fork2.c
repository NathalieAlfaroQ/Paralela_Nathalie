#include <stdio.h>
#include <unistd.h>

int main(void) {
  // Tipo de process id es pid_t
  pid_t pid = fork();
  if (pid == 0) {
    sleep(5);
    printf("Hello from the other side\n");
  } else {
    printf("Hello, it's me\n");
  }
  return 0;
}

/*
1. ¿Se imprimen en el orden esperado?
  Si.

2. ¿Cómo imprime cada proceso?
  Primero imprime el proceso padre y termina saliendo el mensaje de la
		terminal, porque es como si terminara el prgrama y automaticamente se imprime el
otro proceso que es el hijo pero al lado del mensaje de la terminal.

3. ¿Se ejecuta el proceso hijo antes o después del proceso padre?
  Se ejecuta el proceso hijo despues del proceso padre.
*/