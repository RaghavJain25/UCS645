#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "correlate.h"

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Usage: ./correlation <rows> <cols>\n";
        return 1;
    }

    int rows = std::atoi(argv[1]);
    int cols = std::atoi(argv[2]);

    std::vector<float> matrix(rows * cols);
    std::vector<float> output(rows * rows);

    // Initialize random values
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    auto start = std::chrono::high_resolution_clock::now();

    correlate(rows, cols, matrix.data(), output.data());

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Execution Time: " << elapsed.count() << " seconds\n";

    return 0;
}
