#include <iostream>
#include <limits>
//#include <omp.h>
#include <iostream>
#include <fstream>
#include "parser.h"

using namespace std;

int reduce_row(int *tensor, int n) {
    int i{}, minimum = std::numeric_limits<int>::max();
    #pragma omp parallel
    {
        #pragma omp for
        for (i = 0; i < n; i++) {
            #pragma omp critical
            minimum = std::min(minimum, tensor[i]);
            if (minimum == 0) return minimum;
        }
        #pragma omp for
        for (i = 0; i < n; i++)
            tensor[i] -= minimum;
    }
    return minimum;
}

int reduce_col(int **tensor, int j, int n) {
    int i{}, minimum = std::numeric_limits<int>::max();
    #pragma omp parallel
    {
        for (i = 0; i < n; i++) {
        #pragma omp critical
            minimum = std::min(minimum, tensor[i][j]);
            if (minimum == 0) return minimum;
        }
        #pragma omp for
        for (i = 0; i < n; i++)
            tensor[i][j] -= minimum;
    }
    return minimum;
}

int reduction(int **matrix, int n) {
    int i{}, sum_reduction_elements = 0;
    for (i = 0; i < n; ++i)
        sum_reduction_elements += reduce_row(matrix[i], n);
    for (i = 0; i < n; ++i)
        sum_reduction_elements += reduce_col(matrix, i, n);
    return sum_reduction_elements;
}

int main(int argc, char* argv[]) {

    ifstream datafile {argv[1]}; /* first arg is filename */
    llenar_datos(datafile);
    datafile.close();
/*
    int inf = std::numeric_limits<int>::max();
    int values[5][5] = {
            {inf, 20,  30,  10,  11},
            {15,  inf, 16,  4,  2},
            {3,   5,   inf, 2,   4},
            {19,  6,   18,  inf, 3},
            {16,  4,   7,   16,  inf},
    };
    int i{}, j{};
    int **matrix = new int *[5];
    for (i = 0; i < 5; ++i) {
        matrix[i] = new int[5];
        for (j = 0; j < 5; ++j)
            matrix[i][j] = values[i][j];
    }

    std::cout << reduction(matrix, 5) << std::endl;
    */
    return 0;
}
