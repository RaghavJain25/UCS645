#include <mpi.h>
#include <iostream>
using namespace std;

bool isPerfect(int n) {
    int sum = 1;
    for (int i = 2; i <= n/2; i++)
        if (n % i == 0) sum += i;
    return (sum == n && n != 1);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max = 100;

    if (rank == 0) {
        int num = 2;

        for (int i = 1; i < size; i++)
            MPI_Send(&num++, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        while (num <= max) {
            int result;
            MPI_Status status;

            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            if (result > 0)
                cout << "Perfect: " << result << endl;

            MPI_Send(&num++, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
        }
    } else {
        while (true) {
            int num;
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int result = isPerfect(num) ? num : -num;

            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}