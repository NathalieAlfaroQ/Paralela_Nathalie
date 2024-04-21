#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("Main process\n");
  int n = 5;
  for (int i = 1; i <= n; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      printf("I'm child process number %d\n", i);
      return 0;
    } else {
      printf("I'M YOUR FATHER.\n");
    }
  }
  printf("DONE\n");
  return 0;
}

/*
1. ¿Cuántos procesos hijos se crean?
  5

2. ¿En qué orden se crean los procesos hijos?
  Al ejecutarlo mas de una vez se observa que se imprime primero el Main process
  y luego el father de primero, pero luego se puede imprimir el father o el child
  seguidos o intercalados, en todas las ejecuciones sale diferente, pero en la
  ultima ejecucion del padre esta siempre sale de antepenultima linea, luego el
  DONE y despues el ultimo hijo.

3. ¿En qué orden se ejecutan los procesos hijos? ¿Siempre es el mismo orden?
  El orden es diferente en cada ejecucion, lo unico que se mantiene en los child
  es que el ultimo siempre aparece en la ultima linea de la impresion.

4. ¿Cómo sabe el proceso hijo cual es su número?
  Quizas el father no retorna y se vuelve a
  repetir??????????????????????????????????????????
*/