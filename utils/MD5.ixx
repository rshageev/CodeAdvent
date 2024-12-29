export module md5;

import std;

export
{
	std::string CalcMD5(std::string_view str);
}

module : private;

namespace
{
    using uint64 = std::uint64_t;
    using uint32 = std::uint32_t;
    using uint8 = std::uint8_t;

    using State = std::array<uint32, 4>;

    inline constexpr uint32 S[64] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
    };

    inline constexpr uint32 K[64] =
    {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
    };

    void ProcessChunk(std::span<const uint8, 64> block, State& state)
    {
        std::array<uint32, 16> m;
        std::memcpy(m.data(), block.data(), block.size_bytes());

        std::array<uint32, 4> st = state;

        /* Round 1 */
        for (int i = 0; i < 16; ++i) {
            int g = i;
            uint32 f = st[1] & st[2] | ~st[1] & st[3];
            st[0] = std::rotl(st[0] + f + m[g] + K[i], S[i]) + st[1];
            std::ranges::rotate(st, st.end() - 1);
        }

        for (int i = 16; i < 32; ++i)
        {
            int g = (i * 5 + 1) % 16;
            uint32 f = st[1] & st[3] | st[2] & ~st[3];
            st[0] = std::rotl(st[0] + f + m[g] + K[i], S[i]) + st[1];
            std::ranges::rotate(st, st.end() - 1);
        }

        for (int i = 32; i < 48; ++i) {
            int g = (3 * i + 5) % 16;
            uint32 f = st[1] ^ st[2] ^ st[3];
            st[0] = std::rotl(st[0] + f + m[g] + K[i], S[i]) + st[1];
            std::ranges::rotate(st, st.end() - 1);
        }

        for (int i = 48; i < 64; ++i) {
            int g = (7 * i) % 16;
            uint32 f = st[2] ^ (st[1] | ~st[3]);
            st[0] = std::rotl(st[0] + f + m[g] + K[i], S[i]) + st[1];
            std::ranges::rotate(st, st.end() - 1);
        }

        state[0] += st[0];
        state[1] += st[1];
        state[2] += st[2];
        state[3] += st[3];
    }

    std::string State2Hex(std::span<const uint32, 4> state)
    {
        std::array<uint8, 16> digest;
        std::memcpy(digest.data(), state.data(), state.size_bytes());

        std::string result;
        for (auto d : digest) {
            result += std::format("{:0>2x}", d);
        }
        return result;
    }
}

std::string CalcMD5(std::span<const uint8> data)
{
    const uint64 data_size = data.size() * 8;
    State state = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

    // process whole 512 bit / 64 byte chunks
    while (data.size() >= 64) {
        ProcessChunk(data.first<64>(), state);
        data = data.subspan<64>();
    }


    // add padding and length to remaining data
    std::vector<uint8> buffer(data.begin(), data.end());
    buffer.push_back(0x80); // append 1 bit
    
    const size_t new_size = (buffer.size() <= 56) ? 56 : 120;
    buffer.resize(56, 0x00); // pad zeros

    auto len_bits = std::bit_cast<std::array<uint8, 8>>(data_size);
    buffer.append_range(len_bits);

    std::span<const uint8> buf(buffer);
    while (buf.size() >= 64) {
        ProcessChunk(buf.first<64>(), state);
        buf = buf.subspan<64>();
    }

    return State2Hex(state);
}

std::string CalcMD5(std::string_view str)
{
    std::span<const uint8> data((const uint8*)str.data(), str.size());
    return CalcMD5(data);
}
