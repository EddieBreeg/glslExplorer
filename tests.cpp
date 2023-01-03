#include <string_view>

#include <regex>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::regex pattern("[+-]?((([0-9]*\\.)?[0-9]+)|([0-9]+\\.?))");
    
    std::cout << std::regex_match("1.67", pattern) << '\n';
    std::cout << std::regex_match("1.", pattern) << '\n';
    std::cout << std::regex_match(".1", pattern) << '\n';
    std::cout << std::regex_match("3", pattern) << '\n';
    std::cout << std::regex_match("+3", pattern) << '\n';
    std::cout << std::regex_match("-3.2", pattern) << '\n';

    return 0;
}
