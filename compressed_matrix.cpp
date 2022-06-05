#include <iostream>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <time.h>
#include <iomanip>

#define NDEBUG

namespace ublas = boost::numeric::ublas;

int matrixSize = 100;

ublas::compressed_matrix<int> m1(matrixSize, matrixSize);
ublas::compressed_matrix<int> m2(matrixSize, matrixSize);
ublas::compressed_matrix<int> m3(matrixSize, matrixSize);

void useBoost(int matrixSize)
{
    int numOfNoneZero = floor(matrixSize);
    std::cout << "NumOfNoneZero: " << numOfNoneZero << std::endl;
    for (size_t i = 0; i < numOfNoneZero; i++)
    {
        int r = rand() % matrixSize;
        int c = rand() % matrixSize;
        m1(r, c) = rand();
        m2(r, c) = rand();
    }

    clock_t begin_time = clock();

    m3 = ublas::prod(m1, m2);

    printf("Computing time: %f\n", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

int main()
{
    useBoost(matrixSize);
}