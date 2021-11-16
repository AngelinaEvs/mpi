#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

//mpic++ main.cpp -o main
//mpiexec -n 5 ./main

int main()
{
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello world %d/%d\n", rank, size);
    MPI_Finalize();

    return EXIT_SUCCESS;
}
