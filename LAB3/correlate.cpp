#include <cmath>
#include <omp.h>
#include "correlate.h"

void correlate(int rows, int cols, const float* data, float* result) {

#pragma omp parallel for schedule(static)
    for (int i = 0; i < rows; ++i) {

        const float* row_i = data + i * cols;

        for (int j = 0; j <= i; ++j) {

            const float* row_j = data + j * cols;

            double sum_i = 0.0, sum_j = 0.0;
            double sum_i2 = 0.0, sum_j2 = 0.0;
            double dot_product = 0.0;

            for (int k = 0; k < cols; ++k) {
                double a = row_i[k];
                double b = row_j[k];

                sum_i += a;
                sum_j += b;
                sum_i2 += a * a;
                sum_j2 += b * b;
                dot_product += a * b;
            }

            double numerator = cols * dot_product - sum_i * sum_j;
            double denominator = std::sqrt((cols * sum_i2 - sum_i * sum_i) *
                                           (cols * sum_j2 - sum_j * sum_j));

            result[i + j * rows] = numerator / denominator;
        }
    }
}
