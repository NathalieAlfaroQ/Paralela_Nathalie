#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  printf("Process start\n");
  int total_forks = 10;

  /*malloc(100): La función malloc() aloca memoria en el montón (heap)
  * del programa para un bloque de memoria contigua de 100 bytes.

  * char *msg = malloc(100);: La variable msg es un puntero a un carácter
  * (char *) que se inicializa con la dirección de inicio de la memoria
  * asignada por malloc(). En resumen, msg ahora apunta al comienzo de un
  * bloque de memoria que tiene capacidad para almacenar 100 bytes.*/
  char *msg = malloc(100);

  // Escribe en un buffer, msg es el buffer de destino
  // 100 es el tamano maximo del buffer
  // Lo que esta en "" es lo que se escribe
  snprintf(msg, 100, "I'm the parent process!!!\n");
  printf("%s", msg);

  for (int i = 0; i < total_forks; i++) {
    pid_t pid = fork();

    if (pid == 0) {
      // HIJO
      snprintf(msg, 100, "I'm child process number %d\n", i);
      printf("%s", msg);
      return 0;
    } // End if
  }   // End for

  for (int i = 0; i < total_forks; i++) {
    wait(NULL);
  } // End for

  printf("The program ends\n");
  printf("The final value of msg is: %s\n", msg);
  return 0;
} // End main

/*
1. ¿Qué valor tiene la variable msg al final del programa?
	

2. ¿Por qué el valor de msg cambia o no?


3. ¿Porqué los hijos pueden imprimir el mensaje correcto?


4. Al final, qué valor tiene la variable msg? ¿Porqué?


5. ¿Qué pasó con la asignación de memoria de msg realizada en los hijos?

*/