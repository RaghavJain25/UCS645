#include <omp.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

// Smith–Waterman kernel
double run_sw(int num_threads) {
    std::string A = "ACGTTGACACGTTGACACGTTGAC";
    std::string B = "ACTTGACACTTGAC";

    int m = A.size();
    int n = B.size();

    std::vector<std::vector<int>> H(m+1, std::vector<int>(n+1, 0));
    int gap = -2;

    omp_set_num_threads(num_threads);
    double start = omp_get_wtime();

    // Wavefront parallelization
    for (int d = 1; d <= m + n; d++) {
        #pragma omp parallel for
        for (int i = 1; i <= m; i++) {
            int j = d - i;
            if (j >= 1 && j <= n) {
                int diag = H[i-1][j-1] + (A[i-1] == B[j-1] ? 2 : -1);
                int up   = H[i-1][j] + gap;
                int left = H[i][j-1] + gap;
                H[i][j] = std::max(0, std::max(diag, std::max(up, left)));
            }
        }
    }

    double end = omp_get_wtime();
    return end - start;
}

int main() {
    std::vector<int> threads = {1, 2, 4, 8};
    std::vector<double> times;

    std::cout << "Smith–Waterman Performance\n";
    std::cout << "--------------------------\n";

    for (int t : threads) {
        double time = run_sw(t);
        times.push_back(time);
        std::cout << "Threads: " << t << " | Time: " << time << " s\n";
    }

    std::ofstream tfile("q2_time.dat");
    std::ofstream sfile("q2_speedup.dat");

    double T1 = times[0];
    for (int i = 0; i < threads.size(); i++) {
        tfile << threads[i] << " " << times[i] << "\n";
        sfile << threads[i] << " " << T1 / times[i] << "\n";
    }

    tfile.close();
    sfile.close();

    std::ofstream gp("q2_plot.gp");
    gp << "set terminal png size 800,600\n";
    gp << "set grid\n";

    gp << "set output 'q2_time.png'\n";
    gp << "set title 'Q2: Execution Time vs Threads'\n";
    gp << "set xlabel 'Threads'\n";
    gp << "set ylabel 'Time (s)'\n";
    gp << "plot 'q2_time.dat' using 1:2 with linespoints lw 2\n";

    gp << "set output 'q2_speedup.png'\n";
    gp << "set title 'Q2: Speedup vs Threads'\n";
    gp << "set xlabel 'Threads'\n";
    gp << "set ylabel 'Speedup'\n";
    gp << "plot 'q2_speedup.dat' using 1:2 with linespoints lw 2\n";

    gp.close();
    system("gnuplot q2_plot.gp");

    std::cout << "\nGraphs generated:\n";
    std::cout << " - q2_time.png\n - q2_speedup.png\n";

    return 0;
}