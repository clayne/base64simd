#include <immintrin.h>
#include <x86intrin.h>

#include <cstdint>

namespace base64 {

    namespace avx512 {

        template <typename LOOKUP_FN, typename UNPACK_FN>
        void encode(LOOKUP_FN lookup, UNPACK_FN unpack, uint8_t* input, size_t bytes, uint8_t* output) {

            uint8_t* out = output;

            static const uint32_t input_offsets[16] = {
                 0*3,  1*3,  2*3,  3*3,
                 4*3,  5*3,  6*3,  7*3,
                 8*3,  9*3, 10*3, 11*3,
                12*3, 13*3, 14*3, 15*3
            };

            const __m512i input_gather = _mm512_loadu_si512(reinterpret_cast<const __m512i*>(input_offsets));

            for (size_t i = 0; i < bytes; i += 4 * 12) {
                // load bytes
                const __m512i in = _mm512_i32gather_epi32(input_gather, (const int*)(input + i), 1);

                const __m512i indices = unpack(in);

                // do lookup
                const __m512i result = lookup(indices);

                _mm512_storeu_si512(reinterpret_cast<__m512i*>(out), result);

                out += 64;
            }
        }

    } // namespace avx512

} // namespace base64
