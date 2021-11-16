#include "mpi.h"
#include <stdlib.h>
#include <math.h>

const int TAG = 42;

int main(int argc, char **argv) {
    const int n = strtol(argv[1], NULL, 10);
    const int m = strtol(argv[2], NULL, 10);

    int size, *sendbuf;
    int rank, *rbuf, *displs, *scounts;
    int max, min, tmp;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    sendbuf = (int *) malloc(n * m);
    rbuf = (int *) malloc(n * m);
    displs = (int *) malloc(size);
    scounts = (int *) malloc(size);

    for (int i = 0; i < n * m; i++) {
        rbuf[i] = 0;
        if (rank == 0) {
            sendbuf[i] = rand() % 100;
        }
    }

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            if (i == size - 1) scounts[i] = m * (n - (i * (n / size)));
            else scounts[i] = n / size * m;
            displs[i] = m * i * (n / size);
        }
    }

    MPI_Scatterv(sendbuf, scounts, displs, MPI_INT, rbuf, n * m, MPI_INT, 0, MPI_COMM_WORLD);

    int lmax = rbuf[0], lmin = rbuf[0];
    for (int i = 0; i < n * m; i++) {
        if (rbuf[i] <= 0) break;
        if (rbuf[i] > lmax) lmax = rbuf[i];
        if (rbuf[i] < lmin) lmin = rbuf[i];
    }

    int dest = rank + 1;
    int source = rank - 1;
    if (dest >= size) {
        dest = MPI_PROC_NULL;
        MPI_Recv(&tmp, 1, MPI_INT, source, TAG, MPI_COMM_WORLD, &status);
        max = (lmax > tmp ? lmax : tmp);
        MPI_Recv(&tmp, 1, MPI_INT, source, TAG, MPI_COMM_WORLD, &status);
        min = (lmin < tmp ? lmin : tmp);
    } else {
        if (source < 0) {
            source = MPI_PROC_NULL;
            MPI_Send(&lmax, 1, MPI_INT, dest, TAG, MPI_COMM_WORLD);
            MPI_Send(&lmin, 1, MPI_INT, dest, TAG, MPI_COMM_WORLD);
        } else {
            MPI_Recv(&tmp, 1, MPI_INT, source, TAG, MPI_COMM_WORLD, &status);
            max = (lmax > tmp ? lmax : tmp);
            MPI_Recv(&tmp, 1, MPI_INT, source, TAG, MPI_COMM_WORLD, &status);
            min = (lmin < tmp ? lmin : tmp);
            MPI_Send(&max, 1, MPI_INT, dest, TAG, MPI_COMM_WORLD);
            MPI_Send(&min, 1, MPI_INT, dest, TAG, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return MPI_SUCCESS;
}

