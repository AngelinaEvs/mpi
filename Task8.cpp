#include "mpi.h"
#include <stdlib.h>

const int n = 20;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int part = n / size;
    int* recv = new int[part];
    if (rank == 0)
    {
        int array[n];
        for (int i = 0; i < n; i++)
        {
            array[i] = rand() % 1000;
            printf("%d ", array[i]);
        }
        printf("\n");

        for (int i = 0; i < size; i++)
        {
            int* send_buf = new int[part];
            for (int j = 0; j < part; j++)
                send_buf[j] = array[part * i + j];

            if (i == 0)
                recv = send_buf;
            else
                MPI_Send(send_buf, part, MPI_INT, i, i, MPI_COMM_WORLD);

            delete[] send_buf;
        }
    }
    MPI_Status status;
    if (rank > 0) MPI_Recv(recv, part, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
    printf("rank = %d: ", rank);
    for (int i = 0; i < part; i++) {
        printf("%d ", recv[i]);
    }
    printf("\n");
    if (rank != size - 1) MPI_Send(recv, part, MPI_INT, size - 1, size - 1, MPI_COMM_WORLD);
    else {
        int result[n];
        for (int i = 0; i < part; i++)
            result[rank * part + i] = recv[i];
        delete[] recv;

        for (int i = 0; i < size - 1; i++)
            MPI_Recv(result + i * part, part, MPI_INT, i, rank, MPI_COMM_WORLD, &status);

        for (int i = 0; i < n; i++)
            printf("%d ", result[i]);
    }
    MPI_Finalize();
    return MPI_SUCCESS;
}