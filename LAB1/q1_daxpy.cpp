#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int N = 65536;
    double a = 2.5;

    double *X = new double[N];
    double *Y = new double[N];

    for (int i = 0; i < N; i++) {
        X[i] = 1.0;
        Y[i] = 2.0;
    }

    double t1 = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        X[i] = a * X[i] + Y[i];
    }
    double t2 = omp_get_wtime();

    double seq_time = t2 - t1;

    cout << "Sequential Time = " << seq_time << " seconds\n\n";

    for (int threads = 2; threads <= 12; threads++) {

        for (int i = 0; i < N; i++) {
            X[i] = 1.0;
        }

        omp_set_num_threads(threads);

        t1 = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < N; i++) {
            X[i] = a * X[i] + Y[i];
        }
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        double speedup = seq_time / par_time;

        cout << "Threads = " << threads
             << "   Time = " << par_time
             << "   Speedup = " << speedup << endl;
    }

    delete[] X;
    delete[] Y;

    return 0;
}
