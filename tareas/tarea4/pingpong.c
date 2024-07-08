// Compilar con: mpicc -o pingpong pingpong.c
// Ejecutar con: mpirun -np 4 ./pingpong

#include <mpi.h>   
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>  

int main(int argc, char *argv[])
{
    // Inicia MPI
    MPI_Init(&argc, &argv); 
    int rank, size;

    // Obtiene el rango (identificador) del proceso y el tamaño del comunicador
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Inicializa la semilla del generador de números aleatorios con un valor diferente para cada proceso
    srand(time(NULL) + rank); 

    if (rank == 0)
    {
        // Mensaje inicial
        int message = 1; 
        // Elige un proceso destino aleatorio (diferente de 0)
        int dest = rand() % (size - 1) + 1; 
        // Envía el mensaje al proceso destino
        MPI_Send(&message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD); 
        printf("Proceso %d envía %d a proceso %d\n", rank, message, dest);
        // Recibe el mensaje de vuelta de cualquier proceso
        MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Proceso %d recibe %d y finaliza.\n", rank, message);
    }
    else
    {
        while (1)
        {
            int message;
            // Recibe el mensaje de cualquier proceso
            MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d recibe %d\n", rank, message);
            // Incrementa el mensaje
            message += 1; 

            int dest;
            do
            {
                // Elige un proceso destino aleatorio
                dest = rand() % size; 
            } while (dest == rank); 
            
            // Envía el mensaje al nuevo destino
            MPI_Send(&message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD); 
            printf("Proceso %d envía %d a proceso %d\n", rank, message, dest);

            if (dest == 0)
            {
                // Sale del bucle si el destino es el proceso 0
                break; 
            }
        }
    }
    // Finaliza MPI
    MPI_Finalize(); 
    // Termina el programa
    return 0; 
}
