#include <pthread.h>
#include <stdio.h>

void *print_message(char *message) {
  printf("%s\n", message);
  pthread_exit(NULL);
} // End print_message

int main(void) {
  pthread_t thread;
  int argument = 42;
  char *mensaje = "hola";
  
  if (pthread_create(&thread, NULL, print_message(mensaje), &argument) != 0) {
    perror("Error creating thread\n");
    return 1;
  } // End if

  pthread_join(thread, NULL);
  return 0;
} // End main

// Compilar   gcc -pthread -o pthread01 pthread01.c
// Ejecutar ./pthread01

/*
 Corran el programa y reporten el resultado.
  El resultado fue:
  hola
*/