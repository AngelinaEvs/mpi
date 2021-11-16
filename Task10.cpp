#include <mpi.h>
#include <cstdlib>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    const int n = 100000000;
    int* array = new int[n];
    double start, end;

    if (rank == 0)
        for (int i = 0; i < n; i++)
            array[i] = rand();

    if (rank == 0)
    {
        start = MPI_Wtime();
        MPI_Send(array, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        printf("Send time: %f\n", end - start);
    }
    else if (rank == 1)
    {
        MPI_Recv(array, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        start = MPI_Wtime();
        MPI_Ssend(array, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        printf("SSend time: %f\n", end - start);
    }
    else if (rank == 1)
    {
        MPI_Recv(array, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        int buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int) * n;
        int* buffer_attached = new int[buffer_attached_size];
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);

        start = MPI_Wtime();
        MPI_Bsend(array, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(array, n, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        end = MPI_Wtime();

        MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);

        delete[] buffer_attached;
        printf("Bsend time: %f\n", end - start);
    }
    else if (rank == 1)
    {
        MPI_Recv(array, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int* buffer_attached = new int[10 * n];
        MPI_Buffer_attach(buffer_attached, 10 * n);

        MPI_Bsend(array, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        start = MPI_Wtime();
        MPI_Rsend(array, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        printf("Rsend time: %f\n", end - start);
    }
    else if (rank == 1)
    {
        MPI_Recv(array, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    delete[] array;
    MPI_Finalize();

    return MPI_SUCCESS;
}