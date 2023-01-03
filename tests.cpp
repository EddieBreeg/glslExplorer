#include <string_view>

#include <regex>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::regex pattern("\\d[ \\t]*\\(");
    
    std::cout << std::regex_search("vec(", pattern) << '\n';
    std::cout << std::regex_search("vec (", pattern) << '\n';

    std::cout << std::regex_search("vec4(", pattern) << '\n';
    std::cout << std::regex_search("vec4\t(", pattern) << '\n';
    std::cout << std::regex_search("vec4  (", pattern) << '\n';

    return 0;
}
