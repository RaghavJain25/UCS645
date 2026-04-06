#include <mpi.h>
#include <iostream>
using namespace std;

#define N 1000000

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double multiplier = 2.0;
    MPI_Bcast(&multiplier, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int chunk = N / size;
    double local_sum = 0;

    for (int i = 0; i < chunk; i++) {
        local_sum += 1.0 * (2.0 * multiplier);
    }

    double total;
    MPI_Reduce(&local_sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        cout << "Dot Product: " << total << endl;

    MPI_Finalize();
    return 0;
}