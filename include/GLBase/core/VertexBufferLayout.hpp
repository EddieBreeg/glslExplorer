#pragma once

namespace GLBase
{
    struct VertexLayoutElement
    {
    private:
        enum {
            FLOAT = 0x1406, 
            UINT = 0x1405, 
            USHORT = 0x1403, 
            BYTE = 0x1401, 
        } _type;
        unsigned _n;
        const bool _normalized;
        VertexLayoutElement(decltype(_type) type, unsigned n, bool normalized): _type(type), _n(n), _normalized(normalized) {}
    public:
        template<typename T>
        static VertexLayoutElement create(unsigned n);
        unsigned size() const {
            switch (_type)
            {
                case FLOAT:     return _n * sizeof(float);
                case UINT:      return _n * sizeof(unsigned);
                case USHORT:    return _n * sizeof(unsigned short);
                case BYTE:      return _n;
                default: return 0;
            }
        }
        unsigned count() const { return _n; }
        int type() const { return _type; }
        bool normalized() const { return _normalized; }
    };
    template<> inline VertexLayoutElement VertexLayoutElement::create<float>(unsigned n) { 
        return VertexLayoutElement(VertexLayoutElement::FLOAT, n, false);
    }
    template<> inline VertexLayoutElement VertexLayoutElement::create<unsigned>(unsigned n) { 
        return VertexLayoutElement(VertexLayoutElement::UINT, n, false);
    }
    template<> inline VertexLayoutElement VertexLayoutElement::create<unsigned short>(unsigned n) { 
        return VertexLayoutElement(VertexLayoutElement::USHORT, n, false);
    }
    template<> inline VertexLayoutElement VertexLayoutElement::create<unsigned char>(unsigned n) { 
        return VertexLayoutElement(VertexLayoutElement::BYTE, n, true);
    }

    template<unsigned N>
    struct VertexLayout
    {
    private:
        VertexLayoutElement _elements[N];
    public:
        template<typename... Args>
        VertexLayout(const Args&... elements): _elements{elements...} {
        }
        unsigned stride() const {
            unsigned s = 0;
            for(const auto& e: _elements) 
                s += e.size();
            return s;
        }
        const VertexLayoutElement* begin() const { return _elements; }
        const VertexLayoutElement* end() const { return _elements + N; }
        const VertexLayoutElement& operator[](unsigned i) const { return _elements[i]; }
    };
} // namespace GLBase

