#include <mpi.h>
#include <iostream>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i*i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max = 50;

    if (rank == 0) {
        int num = 2;

        for (int i = 1; i < size; i++)
            MPI_Send(&num++, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        while (num <= max) {
            int result;
            MPI_Status status;

            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            if (result > 0)
                cout << "Prime: " << result << endl;

            MPI_Send(&num++, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
        }
    } else {
        while (true) {
            int num;
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int result = isPrime(num) ? num : -num;

            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}