#include <omp.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

// ---------- Molecular Dynamics Kernel ----------
double run_md(int num_threads) {
    const int N = 1000;
    const double epsilon = 1.0;
    const double sigma = 1.0;

    std::vector<double> x(N), y(N), z(N);
    std::vector<double> fx(N, 0.0), fy(N, 0.0), fz(N, 0.0);

    for (int i = 0; i < N; i++) {
        x[i] = rand() / (double)RAND_MAX;
        y[i] = rand() / (double)RAND_MAX;
        z[i] = rand() / (double)RAND_MAX;
    }

    double potential_energy = 0.0;
    omp_set_num_threads(num_threads);

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(+:potential_energy) schedule(dynamic)
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            double dz = z[i] - z[j];

            double r2 = dx*dx + dy*dy + dz*dz;
            double inv_r6 = pow(sigma*sigma / r2, 3);
            double force = 24 * epsilon * inv_r6 * (2*inv_r6 - 1) / r2;

            double fx_ij = force * dx;
            double fy_ij = force * dy;
            double fz_ij = force * dz;

            #pragma omp atomic
            fx[i] += fx_ij;
            #pragma omp atomic
            fy[i] += fy_ij;
            #pragma omp atomic
            fz[i] += fz_ij;

            #pragma omp atomic
            fx[j] -= fx_ij;
            #pragma omp atomic
            fy[j] -= fy_ij;
            #pragma omp atomic
            fz[j] -= fz_ij;

            potential_energy += 4 * epsilon * (inv_r6*inv_r6 - inv_r6);
        }
    }

    double end = omp_get_wtime();
    return end - start;
}

// ---------- Main ----------
int main() {
    std::vector<int> threads = {1, 2, 4, 8};
    std::vector<double> times;

    std::cout << "Running Molecular Dynamics with OpenMP\n";
    std::cout << "--------------------------------------\n";

    for (int t : threads) {
        double exec_time = run_md(t);
        times.push_back(exec_time);
        std::cout << "Threads: " << t
                  << " | Time: " << exec_time << " s\n";
    }

    // ---------- Write data files ----------
    std::ofstream timeFile("md_time.dat");
    std::ofstream speedFile("md_speedup.dat");

    double T1 = times[0];
    for (int i = 0; i < threads.size(); i++) {
        timeFile << threads[i] << " " << times[i] << "\n";
        speedFile << threads[i] << " " << T1 / times[i] << "\n";
    }

    timeFile.close();
    speedFile.close();

    // ---------- Gnuplot script ----------
    std::ofstream gp("md_plot.gp");
    gp << "set terminal png size 800,600\n";
    gp << "set grid\n";

    gp << "set output 'md_time.png'\n";
    gp << "set title 'Execution Time vs Number of Threads'\n";
    gp << "set xlabel 'Number of Threads'\n";
    gp << "set ylabel 'Execution Time (s)'\n";
    gp << "plot 'md_time.dat' using 1:2 with linespoints lw 2\n";

    gp << "set output 'md_speedup.png'\n";
    gp << "set title 'Speedup vs Number of Threads'\n";
    gp << "set xlabel 'Number of Threads'\n";
    gp << "set ylabel 'Speedup'\n";
    gp << "plot 'md_speedup.dat' using 1:2 with linespoints lw 2\n";

    gp.close();

    system("gnuplot md_plot.gp");

    std::cout << "\nGraphs generated successfully:\n";
    std::cout << " - md_time.png\n";
    std::cout << " - md_speedup.png\n";

    return 0;
}