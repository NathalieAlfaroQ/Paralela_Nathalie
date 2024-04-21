#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
	printf("Process start\n");
	int total_forks = 10;
	char *msg = malloc(100);
	snprintf(msg, 100, "I'm the parent process!!!\n");
	printf("[Parent] The address of msg is: %p | Value is: %s\n", msg, msg);
	for (int i = 0; i < total_forks; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			snprintf(msg, 100, "hello from child %d", i);
			printf("[Child %d] The address of msg is: %p | Value is: %s\n", i, msg, msg);
			return 0;
		}
	}
	for (int i = 0; i < total_forks; i++)
	{
		wait(NULL);
	}
	printf("[Parent] The program ends\n");
	printf("[Parent] The final value of msg is: %s\n", msg);
	return 0;
}

/*
1. ¿La dirección de memoria de msg es la misma en el proceso padre y en los procesos hijos?
2. ¿Porqué?
*/