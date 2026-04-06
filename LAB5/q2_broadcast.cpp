#include <mpi.h>
#include <iostream>
using namespace std;

#define N 1000000

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double *arr = new double[N];

    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(arr, N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    } else {
        MPI_Recv(arr, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Bcast(arr, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) cout << "Broadcast Done" << endl;

    delete[] arr;
    MPI_Finalize();
    return 0;
}