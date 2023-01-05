#include <GLBase/core/RNG.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    GLBase::RNG& rng = GLBase::RNG::Instance();

    for (size_t i = 0; i < 10; i++)
    {
        std::cout << rng() << '\n';
    }
    

    return 0;
}
