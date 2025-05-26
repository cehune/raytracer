#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "vec3.h"

/* If any other dimen matrices than 4x4 add here */
template <int N>
class squareMatrix {
public:
    double matrix[N][N];
    
    squareMatrix();
    squareMatrix(double x);
    squareMatrix(const double m[N][N]);
    squareMatrix(const squareMatrix& other);
    squareMatrix operator=(const squareMatrix& m);

    squareMatrix operator+(squareMatrix& m);
    squareMatrix operator*(squareMatrix& m);
    squareMatrix operator*(double x);
    squareMatrix operator/(double x);

    double* operator[](int row);
    const double* operator[](int row) const;
};

template <int N>
squareMatrix<N> transpose(squareMatrix<N>& m);

double determinant_2x2(double a, double d, double c, double b);

squareMatrix<4> inverse(const squareMatrix<4>& m);

squareMatrix<4> operator*(const squareMatrix<4>& m1, const squareMatrix<4>& m2);

vec3h operator*(const vec3h& u, const squareMatrix<4>& m);

#include "matrix.inl"

#endif // MATRIX_H
