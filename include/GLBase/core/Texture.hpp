#pragma once


namespace GLBase
{
    class Texture
    {
    private:
        unsigned _id;
        unsigned _w, _h;
    public:
        Texture(unsigned width, unsigned height);
        Texture();
        void resize(unsigned w, unsigned h);
        void bind() const;
        void unbind() const;
        unsigned getID() const { return _id; }
        ~Texture();
    };
    
} // namespace GLBase

