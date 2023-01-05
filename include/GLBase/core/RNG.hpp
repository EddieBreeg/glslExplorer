#pragma once

#include <inttypes.h>

namespace GLBase
{
    class RNG
    {
    private:
        uint64_t _state[4];
        RNG();
    public:
        using result_type = uint64_t;
        static constexpr result_type min() { return 0; }
        static constexpr result_type max() { return (result_type)-1; }
        result_type operator()();

        void seed(uint64_t s);

        RNG(const RNG&) = delete;
        RNG& operator=(const RNG&) =  delete;

        static RNG& Instance();
        
        ~RNG() = default;
    };    
} // namespace GLBase
