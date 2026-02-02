#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int N = 300;   
    static double A[N][N], B[N][N], C[N][N];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = 1.0;
            B[i][j] = 2.0;
            C[i][j] = 0.0;
        }

    double t1 = omp_get_wtime();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    double t2 = omp_get_wtime();

    double seq_time = t2 - t1;
    cout << "Sequential Time = " << seq_time << " seconds\n\n";

    cout << "1D Parallel:\n";
    for (int threads = 2; threads <= 8; threads++) {

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                C[i][j] = 0.0;

        omp_set_num_threads(threads);

        t1 = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    C[i][j] += A[i][k] * B[k][j];
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        double speedup = seq_time / par_time;

        cout << "Threads = " << threads
             << "   Time = " << par_time
             << "   Speedup = " << speedup << endl;
    }


    cout << "\n2D Parallel:\n";
    for (int threads = 2; threads <= 8; threads++) {

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                C[i][j] = 0.0;

        omp_set_num_threads(threads);

        t1 = omp_get_wtime();
#pragma omp parallel for collapse(2)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    C[i][j] += A[i][k] * B[k][j];
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        double speedup = seq_time / par_time;

        cout << "Threads = " << threads
             << "   Time = " << par_time
             << "   Speedup = " << speedup << endl;
    }

    return 0;
}
