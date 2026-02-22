#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

const int N = 8;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> A = {1,2,3,4,5,6,7,8};
    vector<int> B = {8,7,6,5,4,3,2,1};

    int local_size = N / size;
    vector<int> local_A(local_size);
    vector<int> local_B(local_size);

    MPI_Scatter(A.data(), local_size, MPI_INT,
                local_A.data(), local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    MPI_Scatter(B.data(), local_size, MPI_INT,
                local_B.data(), local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    int local_dot = 0;
    for (int i = 0; i < local_size; i++)
        local_dot += local_A[i] * local_B[i];

    int global_dot = 0;
    MPI_Reduce(&local_dot, &global_dot, 1,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Dot Product: " << global_dot << endl;
    }

    MPI_Finalize();
    return 0;
}