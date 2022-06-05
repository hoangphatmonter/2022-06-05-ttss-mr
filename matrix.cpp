// Sparse matrices in uBLAS are *very* slow if debug mode is enabled. Disable it:
#ifndef NDEBUG
#define BOOST_UBLAS_NDEBUG true
#endif

#include <iostream>
#include <sstream>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <time.h>
#include <chrono>
#include <iomanip>
using namespace std::chrono;

namespace ublas = boost::numeric::ublas;

int matrixSize = 100;

int *A;
int *B;
int *C;

void useNormal(int matrixSize)
{
    A = (int *)malloc(sizeof(int) * matrixSize * matrixSize);
    B = (int *)malloc(sizeof(int) * matrixSize * matrixSize);
    C = (int *)malloc(sizeof(int) * matrixSize * matrixSize);

    printf("Carrying out matrix-matrix multiplication\n");

    for (size_t r; r < matrixSize; r++)
        for (size_t c; c < matrixSize; c++)
        {
            A[r * matrixSize + c] = 0;
            B[r * matrixSize + c] = 0;
        }

    int numOfNoneZero = floor(matrixSize);
    std::cout << "NumOfNoneZero: " << numOfNoneZero << std::endl;
    for (size_t i = 0; i < numOfNoneZero; i++)
    {
        int r = rand() % matrixSize;
        int c = rand() % matrixSize;
        A[r * matrixSize + c] = rand();
        B[r * matrixSize + c] = rand();
    }

    for (int i = 0; i < matrixSize * matrixSize; i++)
    {
        C[i] = 0;
    }

    clock_t begin_time = clock();

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            for (int k = 0; k < matrixSize; k++)
            {
                C[i * matrixSize + j] += A[i * matrixSize + k] * B[k * matrixSize + j];
            }
        }
    }

    printf("Computing time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

int main()
{
    useNormal(matrixSize);

    free(A);
    free(B);
    free(C);
}