#include <mpi.h>
#include <ctime>
#include <cstdlib>

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char** argv)
{
    int iterations = 2500000;
    int total;
    int count = 0;
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    srand(time(0) * rank / size);
    if (rank == 0)
    {
        total = 0;
    }
    for (int i = 0; i < iterations; i++)
    {
        double x = fRand(-1, 1);
        double y = fRand(-1, 1);
        if (x * x + y * y <= 1)
        {
            count += 1;
        }
    }
    MPI_Reduce(&count, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        printf("pi/4 = %f", double(total) / (iterations * size) * 4);
    }
    MPI_Finalize();
    return 0;
}