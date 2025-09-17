/**
 * @file BatchMandelCalculator.cc
 * @author FULL NAME <xkouma02@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over small batches
 * @date DATE
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <stdexcept>
#include <stdlib.h>

#include "BatchMandelCalculator.h"

BatchMandelCalculator::BatchMandelCalculator(unsigned matrixBaseSize, unsigned limit)
    : BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
    // zarovnani data na velikost cache radku
    data = static_cast<int*>(aligned_alloc(64, height * width * sizeof(int)));

    x_values = static_cast<float*>(aligned_alloc(64, width * sizeof(float)));
    y_values = static_cast<float*>(aligned_alloc(64, height * sizeof(float)));

    for (int i = 0; i < width; ++i) {
        x_values[i] = x_start + i * dx;
    }

    for (int i = 0; i < height; ++i) {
        y_values[i] = y_start + i * dy;
    }

#pragma omp simd aligned(data : 64) simdlen(16)
    for (int i = 0; i < width * height; ++i) {
        data[i] = limit;
    }
}

BatchMandelCalculator::~BatchMandelCalculator()
{
    free(data);
    free(x_values);
    free(y_values);
}

int* BatchMandelCalculator::calculateMandelbrot()
{
    int* pdata = data;
    const int block_size = 512;
    const int half_height = height / 2;
    const size_t half_matrix_size = half_height * width;

    for (int block_start_index = 0; block_start_index < half_matrix_size; block_start_index += block_size) {
        int row_number = block_start_index / width * block_size;

        for (int i = row_number; i < std::min(row_number + block_size, half_height); ++i) {
            float y = y_values[i];
            int col_number = block_start_index % width;

#pragma omp simd aligned(pdata : 64) simdlen(16)
            for (int j = col_number; j < std::min(col_number + block_size, width); ++j) {
                float x = x_values[j];

                float zReal = x;
                float zImag = y;

                for (int k = 0; k < limit; ++k) {
                    float r2 = zReal * zReal;
                    float i2 = zImag * zImag;

                    if (r2 + i2 > 4.0f) {
                        pdata[i * width + j] = k;
                        pdata[(height - 1 - i) * width + j] = k;
                        break;
                    }

                    zImag = 2.0f * zReal * zImag + y;
                    zReal = r2 - i2 + x;
                }
            }
        }
    }
    return data;
}
