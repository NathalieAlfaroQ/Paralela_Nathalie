#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	printf("Process start\n");
	int total_forks = 10;
	char *msg = malloc(100);
	snprintf(msg, 100, "I'm the parent process!!!\n");
	printf("%s", msg);
	for (int i = 0; i < total_forks; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			snprintf(msg, 100, "I'm child process number %d\n", i);
			printf("%s", msg);
			return 0;
		}
	}
	for (int i = 0; i < total_forks; i++)
	{
		wait(NULL);
	}
	printf("The program ends\n");
	printf("The final value of msg is: %s\n", msg);
	return 0;
}

/* 
1. ¿Qué valor tiene la variable msg al final del programa?


2. ¿Por qué el valor de msg cambia o no?


3. ¿Porqué los hijos pueden imprimir el mensaje correcto?


4. Al final, qué valor tiene la variable msg? ¿Porqué?


5. ¿Qué pasó con la asignación de memoria de msg realizada en los hijos?

*/