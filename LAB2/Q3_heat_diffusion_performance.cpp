#include <omp.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

// Heat diffusion kernel
double run_heat(int num_threads) {
    const int N = 500;
    const int steps = 500;
    const double alpha = 0.1;

    std::vector<std::vector<double>> T(N, std::vector<double>(N, 0.0));
    std::vector<std::vector<double>> Tnew(N, std::vector<double>(N, 0.0));

    T[N/2][N/2] = 100.0;

    omp_set_num_threads(num_threads);
    double start = omp_get_wtime();

    for (int t = 0; t < steps; t++) {
        #pragma omp parallel for schedule(static)
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                Tnew[i][j] = T[i][j] + alpha * (
                    T[i+1][j] + T[i-1][j] +
                    T[i][j+1] + T[i][j-1] -
                    4 * T[i][j]
                );
            }
        }

        #pragma omp parallel for
        for (int i = 1; i < N-1; i++)
            for (int j = 1; j < N-1; j++)
                T[i][j] = Tnew[i][j];
    }

    double end = omp_get_wtime();
    return end - start;
}

int main() {
    std::vector<int> threads = {1, 2, 4, 8};
    std::vector<double> times;

    std::cout << "Heat Diffusion Performance\n";
    std::cout << "--------------------------\n";

    for (int t : threads) {
        double time = run_heat(t);
        times.push_back(time);
        std::cout << "Threads: " << t << " | Time: " << time << " s\n";
    }

    std::ofstream tfile("q3_time.dat");
    std::ofstream sfile("q3_speedup.dat");

    double T1 = times[0];
    for (int i = 0; i < threads.size(); i++) {
        tfile << threads[i] << " " << times[i] << "\n";
        sfile << threads[i] << " " << T1 / times[i] << "\n";
    }

    tfile.close();
    sfile.close();

    std::ofstream gp("q3_plot.gp");
    gp << "set terminal png size 800,600\n";
    gp << "set grid\n";

    gp << "set output 'q3_time.png'\n";
    gp << "set title 'Q3: Execution Time vs Threads'\n";
    gp << "set xlabel 'Threads'\n";
    gp << "set ylabel 'Time (s)'\n";
    gp << "plot 'q3_time.dat' using 1:2 with linespoints lw 2\n";

    gp << "set output 'q3_speedup.png'\n";
    gp << "set title 'Q3: Speedup vs Threads'\n";
    gp << "set xlabel 'Threads'\n";
    gp << "set ylabel 'Speedup'\n";
    gp << "plot 'q3_speedup.dat' using 1:2 with linespoints lw 2\n";

    gp.close();
    system("gnuplot q3_plot.gp");

    std::cout << "\nGraphs generated:\n";
    std::cout << " - q3_time.png\n - q3_speedup.png\n";

    return 0;
}