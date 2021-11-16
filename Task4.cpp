#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
    int rank;
    int size;
    int root = 0;
    int data[100];
    int data_loc[100];
    float final_res[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int input_size = 0;
    if (rank == 0) {
        printf("Input number elements: ");
        scanf("%d", &input_size);

        printf("Input the elements of the array: ");
        for (int i = 0; i < input_size; i++) {
            scanf("%d", &data[i]);
        }
    }

    MPI_Bcast(&input_size, 1, MPI_INT, root, MPI_COMM_WORLD);

    int loc_num = input_size / size;

    MPI_Scatter(&data, loc_num, MPI_INT, data_loc, loc_num, MPI_INT, root, MPI_COMM_WORLD);

    int loc_sum = 0;
    for (int i = 0; i < loc_num; i++) {
        if (data_loc[i] >= 0) {
            loc_sum += data_loc[i];
        }
    }
    float loc_avg = (float) loc_sum / (float) loc_num;
    MPI_Gather(&loc_avg, 1, MPI_FLOAT, final_res, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

    if (rank == 0) {
        float fin = 0;
        for (int i = 0; i < size; i++)
            if (final_res[i] >= 0) {
                fin += final_res[i];
            }
        float avg = fin / (float) size;
        printf("Final average: %f \n", avg);
    }
    MPI_Finalize();
    return 0;
}
