#include <iostream>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <gmpxx.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 20;
    int part_size = n / size;
    int array[n];
    int result[n];

    int *recv = new int[part_size];
    int *sendcounts = new int[size];
    int *displs = new int[size];
    int *rev = new int[size];

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            array[i] = rand() % 1000;
            printf("%d ", array[i]);
        }
        printf("\n");
        for (int i = 0; i < size; i++) {
            sendcounts[i] = part_size;
            displs[i] = i * part_size;
            rev[size - i - 1] = i * part_size;
        }
    }

    MPI_Scatterv(array, sendcounts, displs, MPI_INT, recv, part_size, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < part_size / 2; i++) {
        swap(recv[i], recv[part_size - i - 1]);
    }

    delete[] displs;
    MPI_Gatherv(recv, part_size, MPI_INT, result, sendcounts, rev, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            printf("%4d ", result[i]);
        }
    }

    delete[] recv;
    delete[] sendcounts;
    delete[] rev;
    MPI_Finalize();

    return MPI_SUCCESS;
}