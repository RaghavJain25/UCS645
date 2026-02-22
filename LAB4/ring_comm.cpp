#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    int value;

    if (rank == 0) {
        value = 100;
        cout << "Process 0 starts with value: " << value << endl;

        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "Process 0 received final value: " << value << endl;
    }
    else {
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        value += rank;
        cout << "Process " << rank << " updated value to: " << value << endl;

        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}