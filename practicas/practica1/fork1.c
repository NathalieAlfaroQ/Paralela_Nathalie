#include <stdio.h>
#include <unistd.h>

int main(void) {
  /* Llamada a fork que la guardamos en una variable de tipo pid_t
  * Esta guarda el valor de retorno de la llamada fork
  * Si valor de retorno es 0, es hijo
  * Si valor de retorno es el PID (process ID) del hijo, es padre
  */
  pid_t pid = fork();

  if (pid == 0) {
    // Proceso hijo
    printf("HIJO\n");
  } else {
    // Proceso padre
    printf("PADRE\n");
  } // End if
  return 0;
} // End main

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