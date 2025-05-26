// matrix.inl

template <int N>
squareMatrix<N>::squareMatrix() {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = (i == j) ? 1 : 0;
}

template <int N>
squareMatrix<N>::squareMatrix(double x) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = x;
}

template <int N>
squareMatrix<N>::squareMatrix(const double m[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = m[i][j];
}

template <int N>
squareMatrix<N>::squareMatrix(const squareMatrix& other) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = other.matrix[i][j];
}

template <int N>
squareMatrix<N> squareMatrix<N>::operator=(const squareMatrix& m) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = m.matrix[i][j];
    return *this;
}

template <int N>
squareMatrix<N> squareMatrix<N>::operator+(squareMatrix& m) {
    squareMatrix ret = *this;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            ret.matrix[i][j] += m.matrix[i][j];
    return ret;
}

template <int N>
squareMatrix<N> squareMatrix<N>::operator*(squareMatrix& m) {
    squareMatrix ret;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                ret[i][j] += matrix[i][k] * m[k][j];
    return ret;
}

template <int N>
squareMatrix<N> squareMatrix<N>::operator*(double x) {
    squareMatrix ret = *this;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            ret.matrix[i][j] *= x;
    return ret;
}

template <int N>
squareMatrix<N> squareMatrix<N>::operator/(double x) {
    if (x == 0.0) {
        throw std::invalid_argument("Division by zero is not allowed.");
    }
    squareMatrix ret = *this;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            ret.matrix[i][j] /= x;
    return ret;
}

template <int N>
double* squareMatrix<N>::operator[](int row) {
    return matrix[row];
}

template <int N>
const double* squareMatrix<N>::operator[](int row) const {
    return matrix[row];
}

template <int N>
squareMatrix<N> transpose(squareMatrix<N>& m) {
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            std::swap(m[i][j], m[j][i]);
        }
    }
    return m;
}
