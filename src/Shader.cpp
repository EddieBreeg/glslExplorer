#include <GLBase/core/Shader.hpp>
#include <GL/glew.h>
#include <GLBase/core/glErrors.h>
#include <iostream>

#define TEXT_STYLE_ERROR        "\033[31;1m"
#define TEXT_STYLE_WARN         "\033[38;5;202;1m"
#define TEXT_STYLE_DEFAULT      "\033[0m"


static inline GLuint createShader(GLenum type){
    glCheckCall(GLenum shader = glCreateShader(type));
    return shader;
}
static inline GLuint setSource(GLuint shader, std::string_view source)
{
    if(!shader) return 0;
    const char *str = source.data();
    glShaderSource(shader, 1, &str, NULL);
    if(glLogError("glShaderSource(shader, 1, &str, NULL);", __FILE__, __LINE__))
    {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
static inline GLuint compileShader(GLuint shader){
    if(!shader) return 0;
    glCompileShader(shader);
    if(glLogError("glCompileShader(shader);", __FILE__, __LINE__)){
        glDeleteShader(shader);
        return 0;
    }
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success) return shader;
    GLint logSize = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    char *msg = (char*)alloca(logSize);
    glGetShaderInfoLog(shader, logSize, NULL, msg);
    std::cerr << TEXT_STYLE_ERROR "GLSL Error: " << msg << TEXT_STYLE_DEFAULT "\n";
    glDeleteShader(shader);
    return 0;
}
static inline GLuint linkShader(GLuint prog){
    if(!prog) return 0;
    glLinkProgram(prog);
    GLint success = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if(success) return prog;

    GLint logSize = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logSize);
    char *msg = (char*)alloca(logSize);
    glGetProgramInfoLog(prog, logSize, NULL, msg);
    std::cerr << TEXT_STYLE_ERROR "GLSL Error: " << msg << TEXT_STYLE_DEFAULT "\n";
    glDeleteProgram(prog);
    return 0;
}
static inline bool attachShader(GLuint& shader, GLuint& prog)
{
    if(!shader || !prog) return 0;
    glAttachShader(prog, shader);
    if(!glLogError("glAttachShader", __FILE__, __LINE__))
        return 1;
    glDeleteShader(shader);
    glDeleteProgram(prog);
    shader = prog = 0;
    return 0;
}

namespace GLBase
{
    Shader::Shader(){
        glCheckCall(_prog = glCreateProgram());
        _vsId = createShader(GL_VERTEX_SHADER);
        _fsId = createShader(GL_FRAGMENT_SHADER);
    }
    Shader::Shader(std::string_view vSource, std::string_view fSource){
        setSource(vSource, fSource);
    }
    void Shader::setSource(std::string_view vSource, std::string_view fSource){
        if(!_vsId) _vsId = createShader(GL_VERTEX_SHADER);
        if(!_fsId) _fsId = createShader(GL_FRAGMENT_SHADER);
        if(!_prog) glCheckCall(_prog = glCreateProgram());
        _vsId = compileShader(::setSource(_vsId, vSource));
        _fsId = compileShader(::setSource(_fsId, fSource));
        attachShader(_vsId, _prog);
        attachShader(_fsId, _prog);
        _prog = linkShader(_prog);

        _valid = _prog && _vsId && _fsId;
    }
    bool Shader::isValid() const {
        return _valid;
    }
    Shader::operator bool() const { return isValid(); }
    void Shader::bind() const {
        glCheckCall(glUseProgram(_prog));
    }
    void Shader::unbind() const {
        glCheckCall(glUseProgram(0));
    }
    Shader::Shader(Shader&& other): _vsId(other._vsId), _fsId(other._fsId),
        _prog(other._prog), _valid(other._valid)
    {
        _uniformCache = std::move(other._uniformCache);
        other._fsId = other._vsId = other._prog = other._valid = 0;
    }
    Shader& Shader::operator=(Shader&& other){
        _uniformCache = std::move(other._uniformCache);
        std::swap(_vsId, other._vsId);
        std::swap(_fsId, other._fsId);
        std::swap(_prog, other._prog);
        std::swap(_valid, other._valid);
        return *this;
    }
    Shader::~Shader(){
        if(_prog) glCheckCall(glDeleteProgram(_prog));
        if(_vsId) glCheckCall(glDeleteShader(_vsId));
        if(_fsId) glCheckCall(glDeleteShader(_fsId));
    }
    int Shader::getUniformID(std::string_view name)  {
        if(!_prog) return -1;
        auto it = _uniformCache.find(name.data());
        if(it != _uniformCache.end()) return it->second;
        glCheckCall(int loc = glGetUniformLocation(_prog, name.data()));
        if(loc == -1)
            std::cerr << TEXT_STYLE_WARN "Warning: uniform '" << name << "' not found\n" TEXT_STYLE_DEFAULT;
        return _uniformCache[name.data()] = loc;
    }

    void Shader::setUniform(std::string_view name, float v0){
        glCheckCall(glUniform1f(getUniformID(name), v0));
    }
    void Shader::setUniform(std::string_view name, float v0, float v1){
        glCheckCall(glUniform2f(getUniformID(name), v0, v1));
    }
    void Shader::setUniform(std::string_view name, float v0, float v1, float v2){
        glCheckCall(glUniform3f(getUniformID(name), v0, v1, v2));
    }
    void Shader::setUniform(std::string_view name, float v0, float v1, float v2, float v3){
        glCheckCall(glUniform4f(getUniformID(name), v0, v1, v2, v3));
    }

    void Shader::setUniform(std::string_view name, int v0){
        glCheckCall(glUniform1i(getUniformID(name), v0));
    }
    void Shader::setUniform(std::string_view name, int v0, int v1){
        glCheckCall(glUniform2i(getUniformID(name), v0, v1));
    }
    void Shader::setUniform(std::string_view name, int v0, int v1, int v2){
        glCheckCall(glUniform3i(getUniformID(name), v0, v1, v2));
    }
    void Shader::setUniform(std::string_view name, int v0, int v1, int v2, int v3){
        glCheckCall(glUniform4i(getUniformID(name), v0, v1, v2, v3));
    }

    void Shader::setUniform(std::string_view name, unsigned v0){
        glCheckCall(glUniform1ui(getUniformID(name), v0));
    }
    void Shader::setUniform(std::string_view name, unsigned v0, unsigned v1){
        glCheckCall(glUniform2ui(getUniformID(name), v0, v1));
    }
    void Shader::setUniform(std::string_view name, unsigned v0, unsigned v1, unsigned v2){
        glCheckCall(glUniform3ui(getUniformID(name), v0, v1, v2));
    }
    void Shader::setUniform(std::string_view name, unsigned v0, unsigned v1, unsigned v2, unsigned v3){
        glCheckCall(glUniform4ui(getUniformID(name), v0, v1, v2, v3));
    }
} // namespace GLBase
