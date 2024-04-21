#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int status = 0;
	printf("Main process\n");
	int n = 5;
	for (int i = 1; i <= n; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			printf("I'm child process number %d\n", i);
			return 0;
		}
		else
		{
			printf("I'M YOUR FATHER.\n");
			waitpid(pid, &status, 0);
		}
	}
	printf("DONE\n");
	return 0;
}

/*
1. ¿Ahora se imprimen en el orden esperado?


2. ¿El proceso padre espera a que todos los procesos hijos terminen antes de terminar?

*/