#include <mpi.h>
#include <iostream>

//mpic++ Task2.cpp -o Task2
//mpi % mpiexec -n 5 ./Task2

int main(int argc, char* argv[]) {
    int rank, size;
    const int array_size = 10;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array[array_size];
    if (rank == 0)
    {
        for (int i = 0; i < array_size; i++) {
            array[i] = rand() % 100;
            printf("%d ", array[i]);
        }
    }
    int diff = array_size / size;
    int start = rank * diff;
    int end = diff * (rank + 1);
    if (rank == size - 1)
        end = size;

    MPI_Bcast(&array, array_size, MPI_INT, 0, MPI_COMM_WORLD);
    int current_max = array[start];
    for (int i = start + 1; i < end; i++)
        if (array[i] > current_max) {
            current_max = array[i];
        }
    int all_max;
    MPI_Reduce(&current_max, &all_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    printf("max = %d in rank = %d \n", current_max, rank);

    if (rank == 0) {
        printf("Found max = %d \n", all_max);
    }
    MPI_Finalize();
}