#include <GLBase/math/Vector.hpp>
#include <cmath>

namespace GLBase
{
    float& Vector3::operator[](long unsigned i) { return _data[i]; }
    const float& Vector3::operator[](long unsigned i) const { return _data[i]; }

    Vector3 Vector3::operator+(const Vector3& other) const{
        return {x + other.x, y + other.y, z + other.z};
    }
    Vector3 Vector3::operator-(const Vector3& other) const{
        return {x - other.x, y - other.y, z - other.z};
    }
    Vector3 Vector3::operator*(const Vector3& other) const{
        return {x * other.x, y * other.y, z * other.z};
    }
    Vector3 Vector3::operator*(float s) const{
        return {x * s, y * s, z * s};
    }
    Vector3 Vector3::operator/(float s) const{
        return {x / s, y / s, z / s};
    }
    Vector3 operator*(float s, const Vector3& v){
        return v * s;
    }

    Vector3& Vector3::operator+=(const Vector3& b){
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }
    Vector3 Vector3::operator-() const{
        return {-x, -y, -z};
    }
    Vector3& Vector3::operator-=(const Vector3& b){
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }
    Vector3& Vector3::operator*=(float s){
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    Vector3& Vector3::operator*=(const Vector3& other){
        return *this = *this * other;
    }

    Vector3& Vector3::operator/=(float s){
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    bool Vector3::operator==(const Vector3& b) const {
        return (x == b.x) &&
            (y == b.y) &&
            (z == b.z);
    }
    bool Vector3::operator!=(const Vector3& b) const {
        return !(*this == b);
    }

    double Vector3::dotProduct(const Vector3& other) const{
        return x * other.x + y * other.y + z * other.z;
    }
    Vector3 Vector3::crossProduct(const Vector3& other) const{
        return Vector3 {
            (y * other.z) - (z * other.y),
            (z * other.x) - (x * other.z),
            (x * other.y) - (y * other.x)
        };
    }


    double Vector3::squaredDistance(const Vector3& other) const{
        return (*this - other).squaredLength();
    }
    double Vector3::distance(const Vector3& other) const{
        return sqrt(squaredDistance(other));
    }
    double Vector3::squaredLength() const{
        return dotProduct(*this);
    }
    double Vector3::length() const{
        return sqrt(squaredLength());
    }


    Vector3 operator*(const Matrix<3, 3>& m, const Vector3& v){
        return Vector3 {
            m._data[0] * v.x + m._data[1] * v.y + m._data[2] * v.z,
            m._data[3] * v.x + m._data[4] * v.y + m._data[5] * v.z,
            m._data[6] * v.x + m._data[7] * v.y + m._data[8] * v.z,
        };
    }

    Vector3 operator*(const Vector3& v, const Matrix<3, 3>& m){
        return Vector3 {
            v.x * m._data[0] + v.y * m._data[3] + v.z * m._data[6],
            v.x * m._data[1] + v.y * m._data[4] + v.z * m._data[7],
            v.x * m._data[2] + v.y * m._data[5] + v.z * m._data[8],
        };
    }

    Vector3::operator Matrix<3, 1>() const {
        return Matrix<3, 1> {x, y, z};
    }
    Vector3::operator Matrix<1, 3>() const {
        return Matrix<1, 3> {x, y, z};
    }
    bool Vector3::operator==(const Matrix<3, 1>& m) const {
        return (x == m._data[0]) && (y == m._data[1])
            && (z == m._data[2]);
    }
    Matrix<4, 4> translate(const Vector3& v)
    {
        return Matrix<4, 4>{
            1, 0, 0, v.x,
            0, 1, 0, v.y,
            0, 0, 1, v.z,
            0, 0, 0, 1
        };
    }
    Matrix<4, 4> scale(const Vector3& s){
        return Matrix<4, 4>{
            s.x, 0, 0, 0,
            0, s.y, 0, 0,
            0, 0, s.z, 0,
            0, 0, 0,   1
        };
    }
    Matrix<4, 4> scale(float s){
        return Matrix<4, 4>{
            s, 0, 0, 0,
            0, s, 0, 0,
            0, 0, s, 0,
            0, 0, 0, 1
        };
    }
} // namespace GLBase
