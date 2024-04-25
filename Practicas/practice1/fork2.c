#include <stdio.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();

  if (pid == 0) {
    // Espere 5 segundos
    sleep(5);
    printf("HIJO\n");
  } else {
    printf("PADRE\n");
  } // End if
  return 0;
} // End main

/*
1. ¿Se imprimen en el orden esperado?
  Si.

2. ¿Cómo imprime cada proceso?
  Primero imprime el proceso padre y termina saliendo el mensaje de la
	terminal, porque es como si terminara el programa y 5 segundos despues se imprime el
  proceso hijo pero al lado del mensaje de la terminal.

  En resumen, el padre no espera al hijo, este termina por completo y luego empieza el hijo

3. ¿Se ejecuta el proceso hijo antes o después del proceso padre?
  Se ejecuta el proceso hijo despues del proceso padre.
*/