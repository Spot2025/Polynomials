#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template <class T>
class Matrix {
    size_t size_;
    std::vector<std::vector<T>> matrix_;
public:
    template <class Y>
    friend class Matrix;

    Matrix(size_t n) : size_(n) {
        matrix_.resize(n);
        for (auto& row : matrix_) {
            row.resize(n);
        }
    }

    Matrix(size_t n, const T& value) : Matrix(n) {
        for (size_t i = 0; i < n; i++) {
            matrix_[i][i] = value;
        }
    }

    explicit Matrix(std::vector<std::vector<T>> matrix) : size_(matrix.size()), matrix_(matrix) {}

    template <class Y>
    Matrix& operator+=(const Matrix<Y>& rhs) {
        for (size_t i = 0; i < size_; i++) {
            for (size_t j = 0; j < size_; j++) {
                matrix_[i][j] += rhs.matrix_[i][j];
            }
        }

        return *this;
    }

    template <class Y>
    Matrix operator+(const Matrix<Y>& rhs) {
        Matrix lhs(*this);
        lhs += rhs;
        return lhs;
    }

    template <class Y>
    Matrix& operator-=(const Matrix<Y>& rhs) {
        for (size_t i = 0; i < size_; i++) {
            for (size_t j = 0; j < size_; j++) {
                matrix_[i][j] -= rhs.matrix_[i][j];
            }
        }

        return *this;
    }

    template <class Y>
    Matrix operator-(const Matrix<Y>& rhs) {
        Matrix lhs(*this);
        lhs -= rhs;
        return lhs;
    }

    template <class Y>
    Matrix operator*(const Matrix<Y>& rhs) {
        Matrix res(size_);
        for (size_t k = 0; k < size_; k++) {
            for (size_t i = 0; i < size_; i++) {
                for (size_t j = 0; j < size_; j++) {
                    res.matrix_[i][j] += matrix_[i][k] * rhs.matrix_[k][j];
                }
            }
        }

        return res;
    }

    template <class Y>
    Matrix& operator*=(const Matrix<Y>& rhs) {
        *this = *this * rhs;
        return *this;
    }

    template <class Y>
    Matrix& operator*=(const Y& value) {
        for (size_t i = 0; i < size_; i++) {
            for (size_t j = 0; j < size_; j++) {
                matrix_[i][j] *= value;
            }
        }

        return *this;
    }

    template <class Y>
    friend Matrix operator*(const Y& lhs, Matrix rhs) {
        rhs *= lhs;
        return rhs;
    }

    template <class Y>
    Matrix operator*(const Y& value) {
        Matrix lhs(*this);
        lhs *= value;
        return lhs;
    }

    template <class Y>
    Matrix& operator+=(const Y& value) {
        *this += Matrix(size_, value);
        return *this;
    }

    template <class Y>
    Matrix operator+(const Y& value) {
        Matrix lhs(*this);
        lhs += value;
        return lhs;
    }

    template <class Y>
    Matrix& operator-=(const Y& value) {
        *this -= Matrix(size_, value);
        return *this;
    }

    template <class Y>
    Matrix operator-(const Y& value) {
        Matrix lhs(*this);
        lhs -= value;
        return lhs;
    }

    size_t Size() const {
        return size_;
    }

    auto GetMatrix() const {
        return matrix_;
    }
};

template <class T>
std::ostream& operator <<(std::ostream& os, const Matrix<T>& matrix) {
    for (size_t i = 0; i < matrix.Size(); i++) {
        for (size_t j = 0; j < matrix.Size(); j++) {
            os << matrix.GetMatrix()[i][j] << " ";
        }
        os << '\n';
    }
    return os;
}

#endif //MATRIX_H
