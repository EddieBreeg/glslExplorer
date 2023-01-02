#pragma once

#include "Matrix.hpp"

namespace GLBase
{
    struct Vector3
    {
        union
        {
            float _data[3]{0};
            struct {float x, y , z;};
            struct {float r, g , b;};
        };
        float& operator[](long unsigned i);
        const float& operator[](long unsigned i) const;
        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator*(const Vector3& other) const;
        Vector3 operator*(float s) const;
        Vector3 operator/(float s) const;
        Vector3 operator-() const;

        Vector3& operator+=(const Vector3& other);
        Vector3& operator-=(const Vector3& other);
        Vector3& operator*=(const Vector3& other);
        Vector3& operator*=(float s);
        Vector3& operator/=(float s);

        bool operator==(const Vector3& other) const;
        bool operator!=(const Vector3& other) const;

        double dotProduct(const Vector3& other) const;
        Vector3 crossProduct(const Vector3& other) const;

        double squaredDistance(const Vector3& other) const;
        double distance(const Vector3& other) const;
        double squaredLength() const;
        double length() const;
        operator Matrix<3, 1>() const;
        operator Matrix<1, 3>() const;
        bool operator==(const Matrix<3, 1>& m) const;
    };

    Vector3 operator*(double s, const Vector3& v);
    Vector3 operator*(const Matrix<3, 3>& m, const Vector3& v);
    Vector3 operator*(const Vector3& v, const Matrix<3, 3>& m);

    Matrix<4, 4> translate(const Vector3& v);
    Matrix<4, 4> scale(const Vector3& s);
    Matrix<4, 4> scale(float s);
    
} // namespace GLBase
