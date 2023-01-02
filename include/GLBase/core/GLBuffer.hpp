#pragma once


namespace GLBase
{
    class GLBuffer
    {
    private:
        unsigned _bufferId, _target;
    protected:
        GLBuffer(const void *data, unsigned size, unsigned target, unsigned usage);
        void setData(const void* data, unsigned size, unsigned usage);
    public:
        void bind() const;
        void unbind() const;
        virtual ~GLBuffer();
    };
    
} // namespace GLBase
