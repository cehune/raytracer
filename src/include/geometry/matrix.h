/* If any other dimen matrices than 4x4 add here*/

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "vec3.h"

template <int N>
class squareMatrix {
public:
    double matrix[N][N];
    squareMatrix() {
        // Default constructor is identity matrix
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j){
                matrix[i][j] = (i == j) ? 1 : 0;
            }
        }
    }

    squareMatrix(double x) {
        // Fills the entire matrix with entry x
        // let x = 0 for the zero matrix
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j){
                matrix[i][j] = x;
            }
        }
    }

    squareMatrix(const double m[N][N]) {
        // Fills the entire matrix with entry x
        // let x = 0 for the zero matrix
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j){
                matrix[i][j] = m[i][j];
            }
        }
    }

    squareMatrix(const squareMatrix& other) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    squareMatrix operator=(const squareMatrix& m) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                matrix[i][j] = m.matrix[i][j];
            }
        }
        return *this;
    }

    squareMatrix operator+(squareMatrix& m) {
        squareMatrix ret = *this;
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j) {
                ret.matrix[i][j] += m.matrix[i][j];
            }
        }
        return ret;
    }

    
    squareMatrix operator*(squareMatrix& m) {
        squareMatrix ret;
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j) {
                for (int k = 0; k < N; ++k){
                    ret[i][j] += matrix[i][k] * m[k][j];
                }
            }
        }
        return ret;
    }

    squareMatrix operator*(double x) {
        squareMatrix ret = *this;
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j) {
                ret.matrix[i][j] *= x;
            }
        }
        return ret;
    }

    squareMatrix operator/(double x) {
        if (x == 0.0) {
            throw std::invalid_argument("Division by zero is not allowed.");
        }
        squareMatrix ret = *this;
        for (int i = 0; i < N; ++i){
            for (int j = 0; j < N; ++j) {
                ret.matrix[i][j] /= x;
            }
        }
        return ret;
    }

    double* operator[](int row) {
        return matrix[row];  // Return a pointer to the row
    }

    // Const version for read-only access
    const double* operator[](int row) const {
        return matrix[row];  // Return a pointer to the row
    }
};

template <int N>
inline squareMatrix<N> inverse(const squareMatrix<N> &m);

template <int N>
inline squareMatrix<N> operator*(const squareMatrix<N> &m1,
                                              const squareMatrix<N> &m2);

template <int N>
inline squareMatrix<N> transpose(squareMatrix<N>& m) {
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            std::swap(m[i][j], m[j][i]);
        }
    }
    return m;
};

inline double determinant_2x2(double a, double d, double c, double b) {
    return a*d - c*b;
}

template <>
inline squareMatrix<4> inverse(const squareMatrix<4> &m) {
    // Via: https://github.com/google/ion/blob/master/ion/math/matrixutils.cc,
    // (c) Google, Apache license.
    double s0 = determinant_2x2(m[0][0], m[1][1], m[1][0], m[0][1]);
    double s1 = determinant_2x2(m[0][0], m[1][2], m[1][0], m[0][2]);
    double s2 = determinant_2x2(m[0][0], m[1][3], m[1][0], m[0][3]);

    double s3 = determinant_2x2(m[0][1], m[1][2], m[1][1], m[0][2]);
    double s4 = determinant_2x2(m[0][1], m[1][3], m[1][1], m[0][3]);
    double s5 = determinant_2x2(m[0][2], m[1][3], m[1][2], m[0][3]);

    double c0 = determinant_2x2(m[2][0], m[3][1], m[3][0], m[2][1]);
    double c1 = determinant_2x2(m[2][0], m[3][2], m[3][0], m[2][2]);
    double c2 = determinant_2x2(m[2][0], m[3][3], m[3][0], m[2][3]);

    double c3 = determinant_2x2(m[2][1], m[3][2], m[3][1], m[2][2]);
    double c4 = determinant_2x2(m[2][1], m[3][3], m[3][1], m[2][3]);
    double c5 = determinant_2x2(m[2][2], m[3][3], m[3][2], m[2][3]);

    // Calculate the determinant
    double determinant = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (determinant == 0)
        return {}; // Return empty matrix if determinant is 0 (singular matrix)
    double s = 1.0 / determinant;

    double inv[4][4] = {{s * dot(m[1][1], c5, m[1][3], c3, -m[1][2], c4),
                        s * dot(-m[0][1], c5, m[0][2], c4, -m[0][3], c3),
                        s * dot(m[3][1], s5, m[3][3], s3, -m[3][2], s4),
                        s * dot(-m[2][1], s5, m[2][2], s4, -m[2][3], s3)},

                       {s * dot(-m[1][0], c5, m[1][2], c2, -m[1][3], c1),
                        s * dot(m[0][0], c5, m[0][3], c1, -m[0][2], c2),
                        s * dot(-m[3][0], s5, m[3][2], s2, -m[3][3], s1),
                        s * dot(m[2][0], s5, m[2][3], s1, -m[2][2], s2)},

                       {s * dot(m[1][0], c4, m[1][3], c0, -m[1][1], c2),
                        s * dot(-m[0][0], c4, m[0][1], c2, -m[0][3], c0),
                        s * dot(m[3][0], s4, m[3][3], s0, -m[3][1], s2),
                        s * dot(-m[2][0], s4, m[2][1], s2, -m[2][3], s0)},

                       {s * dot(-m[1][0], c3, m[1][1], c1, -m[1][2], c0),
                        s * dot(m[0][0], c3, m[0][2], c0, -m[0][1], c1),
                        s * dot(-m[3][0], s3, m[3][1], s1, -m[3][2], s0),
                        s * dot(m[2][0], s3, m[2][2], s0, -m[2][1], s1)}};

    return squareMatrix<4>(inv);
}

template <>
inline squareMatrix<4> operator*(const squareMatrix<4> &m1,
                                              const squareMatrix<4> &m2) {
    squareMatrix<4> r;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            r[i][j] = dot(m1[i][0], m2[0][j], m1[i][1], m2[1][j], m1[i][2],
                m2[2][j], m1[i][3], m2[3][j]);
    return r;
}

inline vec3h operator*(const vec3h &u, const squareMatrix<4> &m) {
    vec3h v;
    v.x = m[0][0] * u.x + m[0][1] * u.y + m[0][2] * u.z + m[0][3] * u.w;
    v.y = m[1][0] * u.x + m[1][1] * u.y + m[1][2] * u.z + m[1][3] * u.w;
    v.z = m[2][0] * u.x + m[2][1] * u.y + m[2][2] * u.z + m[2][3] * u.w;
    v.w = m[3][0] * u.x + m[3][1] * u.y + m[3][2] * u.z + m[3][3] * u.w;
    return v;
}


#endif