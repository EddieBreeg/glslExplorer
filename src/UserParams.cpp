#include <UserSettings.hpp>

static inline std::string_view trimWhiteSpace(std::string_view str)
{
    size_t i = 0;
    while(i < str.size() &&  iswspace(str[i])) ++i;
    return str.substr(i);
}
static inline std::string_view getWord(std::string_view str)
{
    size_t p = 0;
    while(p < str.size() && (std::isalnum(str[p]) || str[p] == '_')){
        ++p;
    }
    return str.substr(0, p);
}

static constexpr const char *typeNames[] = {
    "float",
    "vec2",
    "vec3",
    "vec4",
    "int",
    "ivec2",
    "ivec3",
    "ivec4",
    "uint",
    "uvec2",
    "uvec3",
    "uvec4",
    "bool",
    "bvec2",
    "bvec3",
    "bvec4",
};

UserParam parseUniform(std::string_view line)
{
    line = trimWhiteSpace(line);
    line = trimWhiteSpace(line.substr(7)); // remove "uniform" keyword
    auto typeName = getWord(line);
    line = trimWhiteSpace(line.substr(typeName.size()));
    uint8_t t = 0;
    for(const char* tn: typeNames){
        if(typeName == tn) break;
        ++t;
    }
    std::string_view name = getWord(line);
    return UserParam{std::string(name.begin(), name.end()), (UserParamType_t)t};
}