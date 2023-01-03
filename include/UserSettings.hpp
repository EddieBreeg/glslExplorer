#pragma once

#include <string>
#include <string_view>
#include <cwctype>

enum class UserParamType_t: uint8_t{
    f1, f2, f3, f4,
    i1, i2, i3, i4,
    u1, u2, u3, u4,
    b1, b2, b3, b4,
    unsupported
};
union ParamValue_t
{
    float f[4] {};
    int i[4];
    unsigned u[4];
    bool b[4];
};

struct UserParam
{
    std::string name;
    UserParamType_t type;
    ParamValue_t value;
};

UserParam parseUniform(std::string_view line);