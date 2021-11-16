#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    int rank, size, result;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *sendX = new int[400];
    int *recvX = new int[400];
    int *sendY = new int[400];
    int *recvY = new int[400];
    if (rank == 0) {
        for (int i = 0; i < 400; i++) {
            sendX[i] = rand() % 400;
            sendY[i] = rand() % 400;

        }
    }
    MPI_Scatter(sendX, 100, MPI_INT, recvX, 100, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(sendY, 100, MPI_INT, recvY, 100, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    int fractionalResult = 0;
    for (int i = 0; i < 100; i++) {
        fractionalResult += recvX[i] * recvY[i];
    }
    MPI_Reduce(&fractionalResult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("SM is %i\n", result);
    MPI_Finalize();
    return 0;
}