#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int status = 0;
	printf("Main process\n");
	int n = 5;

	for (int i = 0; i < n; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			// HIJO
			printf("I'm child process number %d\n", i);
			return 0;
		}
		else
		{
			// PADRE
			printf("I'M YOUR FATHER.\n");
			// Esperar al hijo
			waitpid(pid, &status, 0);
		} // End if
	} // End for
	printf("DONE\n");
	return 0;
} // End main

/*
1. ¿Ahora se imprimen en el orden esperado?
	Si, primero se imprime printf("Main process\n"), luego se imprime
	un padre y un hijo hasta que se llega al final y se imprime
	printf("DONE\n").

2. ¿El proceso padre espera a que todos los procesos hijos terminen antes de terminar?
	Si.

*/