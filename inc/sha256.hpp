/// \file sha256.hpp
/// \brief defines SHA-256 hash functions

#pragma once

#include "array.hpp"
#include "list.hpp"
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

inline nat4 _sha256_rev(const nat1* const p) noexcept {
  return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

inline nat4 _sha256_s0(nat4 x) noexcept {
  return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

inline nat4 _sha256_s1(nat4 x) noexcept {
  return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}


inline void _sha256(StringView<cat1> Str, Array<nat4, 8>& H) noexcept {
  String<cat1> ss;
  ss.resize(((Str.size() + 8) / 64 + 1) * 64);
  std::ranges::copy(Str, ss.begin());
  ss[Str.size()] = 0x80_c1;
  *reinterpret_cast<nat8*>(ss.data() + ss.size() - 8) = reverse_endian(Str.size() * 8);
  const nat nn = ss.size() / 64;
  const Array<nat1, 64>* x = reinterpret_cast<const Array<nat1, 64>*>(ss.data());
  Array<nat4, 64> w;
  nat4 a, b, c, d, e, f, g, h;
  for (nat ii{}; ii < nn; ++ii) {
    for (nat i{}; i < 16; ++i) w[i] = _sha256_rev(x[ii] + 4 * i);
    for (nat i{16}; i < 64; ++i) w[i] = w[i - 16] + _sha256_s0(w[i - 15]) +
                                        w[i - 7] + _sha256_s1(w[i - 2]);
    List<>::asref(a, b, c, d, e, f, g, h) = H;
    // auto [a, b, c, d, e, f, g, h] = H;
    for (nat i{}; i < 64; ++i) {
      const auto t1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + _::_sha256_K[i] + w[i];
      const auto t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
      h = g, g = f, f = e, e = d + t1, d = c, c = b, b = a, a = t1 + t2;
    }
    H[0] += a, H[1] += b, H[2] += c, H[3] += d;
    H[4] += e, H[5] += f, H[6] += g, H[7] += h;
  }
}
} // namespace yw::_


export namespace yw {


/// struct to calculate the SHA-256 hash
struct Sha256 {

  /// result array of 32-bit integers
  Array<nat4, 8> array{_::_sha256_H};

  /// conversion operator to `String<cat1>`
  operator String<cat1>() const noexcept {
    return std::format(
      "{:08x}{:08x}{:08x}{:08x}{:08x}{:08x}{:08x}{:08x}",
      array[0], array[1], array[2], array[3],
      array[4], array[5], array[6], array[7]);
  }

  /// calculates the SHA-256 hash of a string
  Sha256(stringable auto&& Str) noexcept {
    constexpr bool b = convertible_to<decltype(Str), StringView<cat1>>;
    if constexpr (b) _::_sha256(Str, array);
    else _::_sha256(cvt<cat1>(Str), array);
  }
};


/// calculates the SHA-256 hash of a string
/// \param s (in) input string
/// \return SHA-256 hash
inline constexpr auto sha256 = [](stringable auto&& s) noexcept
  -> String<cat1> { return Sha256(s); };

} // namespace yw
