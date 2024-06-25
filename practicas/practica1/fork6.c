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
      // Sobreescribe el buffer
      snprintf(msg, 100, "I'm child process number %d\n", i);
      printf("%s", msg);
      return 0;
    } // End if
  }   // End for

  // Ciclo para esperar cada hijo
  for (int i = 0; i < total_forks; i++) {
    wait(NULL);
  } // End for

  printf("The program ends\n");
  printf("The final value of msg is: %s\n", msg);
  return 0;
} // End main

/*
1. ¿Qué valor tiene la variable msg al final del programa?
	'The final value of msg is: I'm the parent process!!!'

2. ¿Por qué el valor de msg cambia o no?
  El valor no cambia, al inicio y al final se imprime msg y da el mismo
  mensaje.

3. ¿Por qué los hijos pueden imprimir el mensaje correcto?
  Porque cada llamado a fork hace una copia de la memoria.

4. Al final, qué valor tiene la variable msg? ¿Porqué?
  Tiene el mismo resultado que el inicial, porque es del parent, el
  cual debe esperar a que todos los hijos se ejecuten y luego el debe
  terminar, entonces msg no se modifica en el parent.

5. ¿Qué pasó con la asignación de memoria de msg realizada en los hijos?
  La sobreescriben en cada hijo por el snprintf(msg, 100, "I'm child process number %d\n", i);
  Pues a cada hijo se le hace una copia de memoria.
*/