#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("Main process\n");
  int n = 5;

  // Se hace un proceso for para poder hacer n hijos
  for (int i = 0; i < n; i++) {
    pid_t pid = fork();

    if (pid == 0) {
      // HIJO
      printf("I'm child process number %d\n", i);
      return 0;
    }// else {
      // PADRE
      //printf("I'M YOUR FATHER.\n");
   // } // End if
  } // End for 

  printf("DONE\n");
  return 0;
} // End main

/*
1. ¿Cuántos procesos hijos se crean?
  5

2. ¿En qué orden se crean los procesos hijos?
  Lo ejecutamos varias veces y lo que siempre sucede es que se imprime primero el 
  printf("Main process\n"), de segundo el printf("I'M YOUR FATHER.\n") y despues
  se imprime intercalado o no el padre (con sus restantes 4 impresiones) y el hijo
  se imprime unas 4 veces, luego se imprime el printf("DONE\n") y por ultimo el
  restante hijo.

3. ¿En qué orden se ejecutan los procesos hijos? ¿Siempre es el mismo orden?
  El orden es diferente en cada ejecucion, lo unico que se mantiene en los child
  es que el ultimo siempre aparece en la ultima linea de la impresion.

4. ¿Cómo sabe el proceso hijo cual es su número?
  Cada hijo sabe cual es su numero porque se imprimen con la i.

  Al parecer el proceso hijo hace que el for continue?????????????????????????
  
*/