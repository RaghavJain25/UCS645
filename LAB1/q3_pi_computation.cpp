#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const long steps = 1000000;
    double step = 1.0 / steps;
    double sum = 0.0;

    double t1 = omp_get_wtime();
    for (long i = 0; i < steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    double pi = step * sum;
    double t2 = omp_get_wtime();
    double seq_time = t2 - t1;
    cout << "Sequential:\n";
    cout << "Pi = " << pi << endl;
    cout << "Time = " << seq_time << " seconds\n\n";

    cout << "Parallel:\n";
    for (int threads = 2; threads <= 8; threads++) {
        sum = 0.0;
        omp_set_num_threads(threads);
        t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum)
        for (long i = 0; i < steps; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        pi = step * sum;
        t2 = omp_get_wtime();
        double par_time = t2 - t1;
        double speedup = seq_time / par_time;
        cout << "Threads = " << threads
             << "   Pi = " << pi
             << "   Time = " << par_time
             << "   Speedup = " << speedup << endl;
    }

    return 0;
}
