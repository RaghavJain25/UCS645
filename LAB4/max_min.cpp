#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(nullptr) + rank);

    int local_max = 0;
    int local_min = 1000;

    for (int i = 0; i < 10; i++) {
        int num = rand() % 1001;

        if (num > local_max) local_max = num;
        if (num < local_min) local_min = num;
    }

    cout << "Process " << rank
         << " Local Max: " << local_max
         << " Local Min: " << local_min << endl;

    struct {
        int value;
        int rank;
    } local_maxloc, global_maxloc,
      local_minloc, global_minloc;

    local_maxloc.value = local_max;
    local_maxloc.rank = rank;

    local_minloc.value = local_min;
    local_minloc.rank = rank;

    MPI_Reduce(&local_maxloc, &global_maxloc, 1,
               MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    MPI_Reduce(&local_minloc, &global_minloc, 1,
               MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "\nGlobal Maximum: "
             << global_maxloc.value
             << " (Process " << global_maxloc.rank << ")" << endl;

        cout << "Global Minimum: "
             << global_minloc.value
             << " (Process " << global_minloc.rank << ")" << endl;
    }

    MPI_Finalize();
    return 0;
}