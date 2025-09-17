/**
 * @file LineMandelCalculator.cc
 * @author FULL NAME <xkouma02@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date DATE
 */
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h>

#include "LineMandelCalculator.h"

LineMandelCalculator::LineMandelCalculator(unsigned matrixBaseSize, unsigned limit)
    : BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
{
    // zarovnani dat na velikost cache radku
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

LineMandelCalculator::~LineMandelCalculator()
{
    free(data);
    free(x_values);
    free(y_values);
}

int* LineMandelCalculator::calculateMandelbrot()
{
    int* pdata = data;

    for (int i = 0; i < height / 2; ++i) {
        float y = y_values[i];

#pragma omp simd aligned(pdata, x_values, y_values : 64) simdlen(16)
        for (int j = 0; j < width; ++j) {
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
    return data;
}
