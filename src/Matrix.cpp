#include <GLBase/math/Matrix.hpp>

namespace GLBase
{
    double det(const Matrix<3, 3>& m) {
        return 
            m._data[0] * (m._data[4] * m._data[8] - m._data[5] * m._data[7])
            - m._data[1] * (m._data[3] * m._data[8] - m._data[5] * m._data[6])
            + m._data[2] * (m._data[3] * m._data[7] - m._data[4] * m._data[6]);
    }
    double det(const Matrix<2, 2>& m) {
        return m._data[0] * m._data[3] - m._data[2] * m._data[1];
    }

    Matrix<2, 2>& invert(const Matrix<2, 2>& m, Matrix<2, 2>& result) {
        double d = det(m);
        if(!d) {
            return (result = Matrix<2, 2>{});
        }
        result._data[0] = (float)(m._data[3] / d);
        result._data[1] = (float)(-m._data[1] / d);
        result._data[2] = (float)(-m._data[2] / d);
        result._data[3] = (float)(m._data[0] / d);
        return result;
    }

    Matrix<3, 3>& invert(const Matrix<3, 3>& m, Matrix<3, 3>& result) {
        double d = det(m);
        if(!d) {
            return (result = Matrix<3, 3>{});
        }
        result._data[0] =(float)((m._data[4] * m._data[8] - m._data[5] * m._data[7]) / d);
        result._data[1] =(float)((m._data[2] * m._data[7] - m._data[1] * m._data[8]) / d);
        result._data[2] =(float)((m._data[1] * m._data[5] - m._data[2] * m._data[4]) / d);
        result._data[3] =(float)((m._data[5] * m._data[6] - m._data[3] * m._data[8]) / d);
        result._data[4] =(float)((m._data[0] * m._data[8] - m._data[2] * m._data[6]) / d);
        result._data[5] =(float)((m._data[2] * m._data[3] - m._data[0] * m._data[5]) / d);
        result._data[6] =(float)((m._data[3] * m._data[7] - m._data[4] * m._data[6]) / d);
        result._data[7] =(float)((m._data[1] * m._data[6] - m._data[0] * m._data[7]) / d);
        result._data[8] =(float)((m._data[0] * m._data[4] - m._data[1] * m._data[3]) / d);
        return result;
    }

    Matrix<4, 4> ortho(float left, float right, float bottom, float top, float near, float far)
    {
        return Matrix<4, 4> {
            2 / (right - left), 0, 0, -(right + left)/(right - left),
            0, 2 / (top - bottom), 0, -(top + bottom)/(top - bottom),
            0, 0, -2/(far - near), -(far + near)/(far - near),
            0, 0, 0, 1
        };
    }
} // namespace GLBase

