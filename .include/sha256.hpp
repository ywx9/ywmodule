/// \file sha256.hpp
/// \brief defines SHA-256 hash functions

#pragma once

#include "array.hpp"
#include "string.hpp"
#include "utility.hpp"


namespace yw::_ {

inline static constexpr Array<nat4,  8> _sha256_H{
  0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
  0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19, };

inline static constexpr Array<nat4, 64> _sha256_K{
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

} // namespace yw::_


export namespace yw {


/// console out bits (big-endian)
void cout_bits(const void* p, const nat n_bytes) {
  const auto bytes = reinterpret_cast<const nat1*>(p);
  for (nat i{}; i < n_bytes; ++i) {
    for (nat j{8}; j--;) std::cout << (bytes[i] >> j & 1);
    std::cout << ' ';
    if (i % 8 == 7) std::cout << std::endl;
  }
  std::cout << std::endl;
}


/// struct to calculate the SHA-256 hash
struct Sha256 {

  /// result array of 32-bit integers
  Array<nat4, 8> array{_::_sha256_H};

  /// conversion operator to `String<cat1>`
  operator String<cat1>() const noexcept {
    return std::format("{:08x}{:08x}{:08x}{:08x}{:08x}{:08x}{:08x}{:08x}",
                       array[0], array[1], array[2], array[3],
                       array[4], array[5], array[6], array[7]);
  }

  /// calculates the SHA-256 hash of a string
  Sha256(StringView<cat1> Str) noexcept {
    for (nat i{}; i < Str.size(); ++i) std::cout << std::format("{}, ", bitcast<nat1>(Str[i]));
    std::cout << std::endl;
    std::cout << Str.size() << std::endl;
    String<cat1> ss;
    ss.resize(((Str.size() + 8) / 64 + 1) * 64);
    std::ranges::copy(Str, ss.begin());
    ss[Str.size()] = 0x80_c1;
    nat nn = Str.size() * 8;

    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 8) = nat1(Str.size() * 8 >> 56 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 7) = nat1(Str.size() * 8 >> 48 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 6) = nat1(Str.size() * 8 >> 40 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 5) = nat1(Str.size() * 8 >> 32 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 4) = nat1(Str.size() * 8 >> 24 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 3) = nat1(Str.size() * 8 >> 16 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 2) = nat1(Str.size() * 8 >> 8 & 0xff);
    *reinterpret_cast<nat1*>(ss.data() + ss.size() - 1) = nat1(Str.size() * 8 & 0xff);
    const nat nn = ss.size() / 64;
    const Array<nat1, 64>* blocks = reinterpret_cast<const Array<nat1, 64>*>(ss.data());
    for (nat i{}; i < ss.size(); ++i) std::cout << std::format("{}, ", bitcast<nat1>(ss[i]));
    std::cout << std::endl;
    Array<nat4, 64> w;
    for (nat ii{}; ii < nn; ++ii) {
      for (nat i{}; i < 8; ++i) std::cout << std::format("{:08x}, ", array[i]);
      std::cout << std::endl;
      for (nat i{}; i < 16; ++i) {
        w[i] = blocks[ii][i * 4] << 24 | blocks[ii][i * 4 + 1] << 16 | blocks[ii][i * 4 + 2] << 8 | blocks[ii][i * 4 + 3];
      }
      for (nat i{16}; i < 64; ++i) {
        const auto s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        const auto s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
      }
      auto [a, b, c, d, e, f, g, h] = array;
      for (nat i{}; i < 64; ++i) {
        // const auto S1 = ;
        const auto t1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + _::_sha256_K[i] + w[i];
        const auto t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
      }
      array[0] += a, array[1] += b, array[2] += c, array[3] += d;
      array[4] += e, array[5] += f, array[6] += g, array[7] += h;
    }
    for (nat i{}; i < 8; ++i) std::cout << std::format("{:08x}, ", array[i]);
    std::cout << std::endl;
  }
};


/// calculates the SHA-256 hash of a string
/// \param s (in) input string
/// \return SHA-256 hash
inline constexpr auto sha256 = [](StringView<cat1> s) noexcept
  -> String<cat1> { return Sha256(s); };

} // namespace yw
