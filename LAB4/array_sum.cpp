#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

const int N = 100;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> data;
    int local_size = N / size;
    vector<int> local_data(local_size);

    if (rank == 0) {
        data.resize(N);
        for (int i = 0; i < N; i++)
            data[i] = i + 1;
    }

    MPI_Scatter(data.data(), local_size, MPI_INT,
                local_data.data(), local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int val : local_data)
        local_sum += val;

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1,
               MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Global Sum: " << global_sum << endl;
        cout << "Average: " << (double)global_sum / N << endl;
    }

    MPI_Finalize();
    return 0;
}