#include <UserSettings.hpp>
#include <regex>

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

#define MATCH_NUMBER   "[+-]?((([0-9]*\\.)?[0-9]+)|([0-9]+\\.?))" 
#define MATCH_BOOLEAN   "(true)|(false)|(" MATCH_NUMBER ")"

void parseDefaultValue(std::string_view v, UserParam& p)
{
    unsigned maxMatches = ((unsigned)p.type & 3) + 1;
    std::regex pattern;
    std::regex_iterator<std::string_view::iterator> end;
    switch (p.type)
    {
    case UserParamType_t::f1:
    case UserParamType_t::f2:
    case UserParamType_t::f3:
    case UserParamType_t::f4:
        pattern = MATCH_NUMBER; // one hell of an expression for such a simple thing...
        {
        decltype(end) it(v.begin(), v.end(), pattern);
        float *out = p.value.f;
        for(size_t i=0; it != end && i < maxMatches; ++it, ++i){
            *out++ = (float)atof(it->str().c_str());
        }
        }
        break;
    case UserParamType_t::i1:
    case UserParamType_t::i2:
    case UserParamType_t::i3:
    case UserParamType_t::i4:
        pattern = MATCH_NUMBER;
        {
        decltype(end) it(v.begin(), v.end(), pattern);
        int *out = p.value.i;
        for(size_t i=0; it != end && i < maxMatches; ++it, ++i){
            *out++ = atoi(it->str().c_str());
        }
        }
    case UserParamType_t::b1:
    case UserParamType_t::b2:
    case UserParamType_t::b3:
    case UserParamType_t::b4:
        pattern = MATCH_BOOLEAN;
        {
        decltype(end) it(v.begin(), v.end(), pattern);
        bool *out = p.value.b;
        for(size_t i=0; it != end && i < maxMatches; ++it, ++i){
            std::string s=it->str();
            if(isalpha(s[0]))
                *out++ = (s == "true");
            else *out++ = (bool)atof(s.c_str());
        }
        }
    default:
        break;
    }
}

UserParam parseUniform(std::string_view line)
{
    UserParam p;
    std::regex pattern(R"([a-zA-Z0-9]+)"); // pattern to match a word
    std::regex_iterator<decltype(line)::iterator> it(line.begin(), line.end(), pattern);
    decltype(it) end;
    if(std::distance(++it, end) < 2) {
        return p;
    }
    uint8_t t = 0;
    for(const char *tn: typeNames){
        if(it->str() == tn) break;
        ++t;
    }
    p.type = UserParamType_t(t);
    p.name = (++it)->str();
    std::string_view endOfLine = line.substr(it->position()+it->length());
    if(endOfLine.find('='))
        parseDefaultValue(endOfLine, p);

    return p;
}