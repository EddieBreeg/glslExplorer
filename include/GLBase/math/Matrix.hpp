#pragma once

#include <stddef.h>

namespace GLBase
{
    template<size_t N, size_t P>
    struct Matrix
    {
        float _data[N * P]{0};
        float& at(size_t i, size_t j) { return _data[P * i + j]; }
        const float& at(size_t i, size_t j) const { return _data[P * i + j]; }
        float& operator()(size_t i, size_t j) { return at(i, j); }
        const float& operator()(size_t i, size_t j) const { return at(i, j); }
        float& operator[](size_t i) { return _data[i]; }
        const float& operator[](size_t i) const { return _data[i]; }

        Matrix& add(const Matrix& other, Matrix& result) const {
            for(size_t i=0; i < N * P; ++i)
                result._data[i] = _data[i] + other._data[i];
            return result;
        }
        Matrix<N, P> operator+(const Matrix& other) const {
            Matrix<N, P> result;
            add(other, result);
            return result;
        }
        Matrix& sub(const Matrix& other, Matrix& result) const {
            size_t i=0;
            for(auto& x: result._data) x = _data[i] - other._data[i++];
            return result;
        }
        Matrix<N, P> operator-(const Matrix& other) const {
            Matrix<N, P> result;
            sub(other, result);
            return result;
        }
        template<size_t Q>
        Matrix<N, Q>& mul(const Matrix<P, Q>& other, Matrix<N, Q>& result) const {
            auto *ptr = result._data;
            for(size_t i = 0; i < N; ++i){
                for (size_t j = 0; j < Q; j++)
                {
                    for(size_t k = 0; k < P; ++k)
                        *ptr += at(i, k) * other.at(k, j);
                    ++ptr;
                }
            }
            return result;
        }
        Matrix<N, P>& mul(double x, Matrix<N, P>& result) const {
            for(size_t i = 0; i < N * P; ++i) result._data[i] = _data[i] * x;
            return result;
        }
        Matrix<N, P>& div(double x, Matrix<N, P>& result) const {
            for(size_t i = 0; i < N * P; ++i) result._data[i] = _data[i] / x;
            return result;
        }
        Matrix<N, P> operator*(double x) const {
            Matrix<N, P> res;
            mul(x, res);
            return res;
        }
        Matrix<N, P> operator/(double x) const {
            Matrix<N, P> res;
            div(x, res);
            return res;
        }
        Matrix<N, P>& operator*=(double x) {
            for(auto& y: _data) y *= x;
            return *this;
        }
        Matrix<N, P>& operator/=(double x) {
            for(auto& y: _data) y /= x;
            return *this;
        }
        Matrix<P, N>& transpose(Matrix<P, N>& result) const {
            for (size_t i = 0; i < N; i++)
            {
                for (size_t j = 0; j < P; j++)
                {
                    result(j, i) = at(i, j);
                }
            }
            return result;
        }
        Matrix<P, N> transpose() const {
            Matrix<P, N> result;
            transpose(result);
            return result;
        }
        template<size_t Q>
        Matrix<N, Q> operator*(const Matrix<P, Q>& other) const {
            Matrix<N ,Q> result;
            mul(other, result);
            return result;
        }
        bool operator==(const Matrix& other) const {
            for(size_t i = 0; i < N * P; ++i)
                if(_data[i] != other._data[i]) return 0;
            return 1;
        }
        bool operator!=(const Matrix& other) const { return !(*this == other); }
    };

    template<size_t N, size_t P>
    Matrix<N, P> operator*(double x, const Matrix<N, P>& m){
        return m * x;
    }
    template<size_t N, size_t P>
    Matrix<N, P> operator/(double x, const Matrix<N, P>& m){
        return m / x;
    }
    template<size_t N, size_t P>
    Matrix<N, P>& operator*=(double x, const Matrix<N, P>& m){
        return m *= x;
    }
    template<size_t N, size_t P>
    Matrix<N, P>& operator/=(double x, const Matrix<N, P>& m){
        return m /= x;
    }

    double det(const Matrix<3, 3>& m);
    double det(const Matrix<2, 2>& m);
    Matrix<2, 2>& invert(const Matrix<2, 2>& m, Matrix<2, 2>& result);
    Matrix<3, 3>& invert(const Matrix<3, 3>& m, Matrix<3, 3>& result);

    Matrix<4, 4> ortho(float left, float right, float bottom, float top, float near, float far);
} // namespace GLBase

