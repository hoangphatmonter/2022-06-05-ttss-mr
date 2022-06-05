// Sparse matrices in uBLAS are *very* slow if debug mode is enabled. Disable it:
#ifndef NDEBUG
#define BOOST_UBLAS_NDEBUG true
#endif

#include <iostream>
#include <sstream>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <time.h>

namespace ublas = boost::numeric::ublas;

int matrixSize = 100;

ublas::compressed_matrix<int> m1(matrixSize, matrixSize); // 10x10 compressed matrix
ublas::compressed_matrix<int> m2(matrixSize, matrixSize); // 10x10 compressed matrix
int *A;
int *B;
int *C;

clock_t begin_time;

void useNormal(int matrixSize)
{
    A = (int *)malloc(sizeof(int) * matrixSize * matrixSize);
    B = (int *)malloc(sizeof(int) * matrixSize * matrixSize);
    C = (int *)malloc(sizeof(int) * matrixSize * matrixSize);

    // A = new int[matrixSize];
    // B = new int[matrixSize];
    // C = new int[matrixSize];

    printf("Carrying out matrix-matrix multiplication\n");

    for (size_t r; r < matrixSize; r++)
        for (size_t c; c < matrixSize; c++)
        {
            A[r * matrixSize + c] = 0;
            B[r * matrixSize + c] = 0;
        }

    int numOfNoneZero = floor(matrixSize * 0.5);
    std::cout << "NumOfNoneZero: " << numOfNoneZero << std::endl;
    for (size_t i = 0; i < numOfNoneZero; i++)
    {
        int r = rand() % matrixSize;
        int c = rand() % matrixSize;
        A[r * matrixSize + c] = rand() % 10000;
        B[r * matrixSize + c] = rand() % 10000;
    }

    /* for (int i = 0; i < matrixSize * matrixSize; i++)
    {
        A[i] = 0;
        B[i] = 0;
    }

    // Cilk Plus array notation
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            // A[i * matrixSize + j] = (double)i / (double)matrixSize;
            // B[i * matrixSize + j] = (double)j / (double)matrixSize;
            A[i * matrixSize + j] = m1(i, j);
            B[i * matrixSize + j] = m2(i, j);
        }
    } */

    // C [0:n * n] = 0.0;
    for (int i = 0; i < matrixSize * matrixSize; i++)
    {
        C[i] = 0;
    }

    // C = A x B
    begin_time = clock();
    //#pragma omp parallel for schedule(guided)
    //#pragma omp parallel collapse(2)
    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            for (int k = 0; k < matrixSize; k++)
            {
                C[i * matrixSize + j] += A[i * matrixSize + k] * B[k * matrixSize + j];
                // C[0] += 1;
            }
        }
    }

    printf("Computing time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
    // free(A);
    // free(B);
    // free(C);
    // delete[] A;
    // delete[] B;
    // delete[] C;

    // print out
    /* for (size_t i = 0; i < matrixSize; ++i)
    {
        for (size_t j = 0; j < matrixSize; ++j)
        {
            std::cout << A[i * matrixSize + j] << ' ';
            // if (C[i, j] != 0)
            //     std::cout << i << "," << j << ":" << C[i, j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // print out
    for (size_t i = 0; i < matrixSize; ++i)
    {
        for (size_t j = 0; j < matrixSize; ++j)
        {
            std::cout << B[i * matrixSize + j] << ' ';
            // if (C[i, j] != 0)
            //     std::cout << i << "," << j << ":" << C[i, j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // print out
    for (size_t i = 0; i < matrixSize; ++i)
    {
        for (size_t j = 0; j < matrixSize; ++j)
        {
            std::cout << C[i * matrixSize + j] << ' ';
            // if (C[i, j] != 0)
            //     std::cout << i << "," << j << ":" << C[i, j] << ' ';
        }
        std::cout << '\n';
    } */
}

void useBoost(int matrixSize)
{
    // for (size_t r; r < matrixSize; r++)
    //     for (size_t c; c < matrixSize; c++)
    //     {
    //         m1(r, c) = 0;
    //         m2(r, c) = 0;
    //     }

    // int numOfNoneZero = floor(matrixSize * 0.5);
    // std::cout << "NumOfNoneZero: " << numOfNoneZero << std::endl;
    // for (size_t i = 0; i < numOfNoneZero; i++)
    // {
    //     int r = rand() % matrixSize;
    //     int c = rand() % matrixSize;
    //     m1(r, c) = rand();
    //     m2(r, c) = rand();
    // }

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            m1(i, j) = A[i * matrixSize + j];
            m2(i, j) = B[i * matrixSize + j];
        }
    }

    begin_time = clock();

    ublas::compressed_matrix<int> m3 = ublas::prod(m1, m2);

    printf("Computing time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);

    // print out
    /* for (size_t i = 0; i < m3.size1(); ++i)
    {
        for (size_t j = 0; j < m3.size2(); ++j)
            std::cout << m1(i, j) << ' ';
        // if (m3(i, j) != 0)
        //     std::cout << i << "," << j << ":" << m3(i, j) << ' ';
        std::cout << '\n';
    }

    // print out
    for (size_t i = 0; i < m3.size1(); ++i)
    {
        for (size_t j = 0; j < m3.size2(); ++j)
            std::cout << m2(i, j) << ' ';
        // if (m3(i, j) != 0)
        //     std::cout << i << "," << j << ":" << m3(i, j) << ' ';
        std::cout << '\n';
    }

    // print out
    for (size_t i = 0; i < m3.size1(); ++i)
    {
        for (size_t j = 0; j < m3.size2(); ++j)
            std::cout << m3(i, j) << ' ';
        // if (m3(i, j) != 0)
        //     std::cout << i << "," << j << ":" << m3(i, j) << ' ';
        std::cout << '\n';
    }
    std::cout << '\n'; */

    // info on the storage
    /* std::cout << "Non-zeroes: " << m1.nnz() << '\n'
              << "Allocated storage for " << m1.nnz_capacity() << '\n';
    std::cout << "Non-zeroes: " << m2.nnz() << '\n'
              << "Allocated storage for " << m2.nnz_capacity() << '\n';
    std::cout << "Non-zeroes: " << m3.nnz() << '\n'
              << "Allocated storage for " << m3.nnz_capacity() << '\n'; */
}

int main()
{
    useNormal(matrixSize);
    std::cout << "-------------------------\n";
    useBoost(matrixSize);

    free(A);
    free(B);
    free(C);
}