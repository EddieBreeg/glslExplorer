#include <string_view>
#include <iostream>
#include <UserSettings.hpp>

int main(int argc, char const *argv[])
{
    UserParam p = parseUniform("uniform float foobar;");
    std::cout << p.name << ' ' << (unsigned)p.type << '\n';
    return 0;
}
