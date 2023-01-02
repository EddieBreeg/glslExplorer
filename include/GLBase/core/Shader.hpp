#pragma once

#include <string_view>
#include <unordered_map>


namespace GLBase
{
    class Shader
    {
    private:
        unsigned _vsId=0, _fsId=0;
        unsigned _prog=0;
        bool _valid = false;
        std::unordered_map<std::string, int> _uniformCache;

    public:
        Shader();
        Shader(std::string_view vertexShaderCode, std::string_view fragShaderCode);
        bool isValid() const;
        operator bool() const;
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& other);
        Shader& operator=(Shader&& other);

        void setSource(std::string_view vertexSource, std::string_view fragmentSource);
        void bind() const;
        void unbind() const;

        int getUniformID(std::string_view name);

        void setUniform(std::string_view name, float v0);
        void setUniform(std::string_view name, float v0, float v1);
        void setUniform(std::string_view name, float v0, float v1, float v2);
        void setUniform(std::string_view name, float v0, float v1, float v2, float v3);

        void setUniform(std::string_view name, int v0);
        void setUniform(std::string_view name, int v0, int v1);
        void setUniform(std::string_view name, int v0, int v1, int v2);
        void setUniform(std::string_view name, int v0, int v1, int v2, int v3);

        void setUniform(std::string_view name, unsigned v0);
        void setUniform(std::string_view name, unsigned v0, unsigned v1);
        void setUniform(std::string_view name, unsigned v0, unsigned v1, unsigned v2);
        void setUniform(std::string_view name, unsigned v0, unsigned v1, unsigned v2, unsigned v3);


        ~Shader();
    };
    
} // namespace GLBase
