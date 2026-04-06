#include <mpi.h>
#include <iostream>
using namespace std;

#define N 65536

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 2.5;
    int chunk = N / size;

    double *X = new double[chunk];
    double *Y = new double[chunk];

    for (int i = 0; i < chunk; i++) {
        X[i] = 1.0;
        Y[i] = 2.0;
    }

    for (int i = 0; i < chunk; i++) {
        X[i] = a * X[i] + Y[i];
    }

    if (rank == 0) cout << "DAXPY Done" << endl;

    delete[] X;
    delete[] Y;

    MPI_Finalize();
    return 0;
}