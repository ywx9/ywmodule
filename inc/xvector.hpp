/// \file xvector.hpp
/// \brief defines `typename yw::XVector` and its related definitions

#pragma once

#include <immintrin.h>

#include "array.hpp"
#include "value.hpp"

export namespace yw {


/// extended vector type
using XVector = __m128;

/// extended matrix type
using XMatrix = Array<XVector, 4>;

/// object to represent a constant vector
template<Value X, Value Y = X, Value Z = Y, Value W = Z>
inline constexpr caster XVCONSTANT{
  []() noexcept -> const XVector& {
    static const XVector& v{_mm_set_ps(fat4(W), fat4(Z), fat4(Y), fat4(X))};
    return v;
  },
  []() noexcept -> const __m128i& {
    static const __m128i m{_mm_set_epi32(int4(W), int4(Z), int4(Y), int4(X))};
    return m;
  }
};

/// specialization for zero vector/matrix
template<> inline constexpr caster XVCONSTANT<0, 0, 0, 0>{
  []() noexcept -> XVector { return _mm_setzero_ps(); },
  []() noexcept -> __m128i { return _mm_setzero_si128(); },
  []() noexcept -> const XMatrix& {
    static const XMatrix m{_mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps()};
    return m;
  }
};

/// constant vector/matrix of zero
inline constexpr auto XVZERO = XVCONSTANT<0>;

/// constant vector of (-0, -0, -0, -0)
inline constexpr auto XVNEGZERO = XVCONSTANT<-0>;

/// constant vector of (1, 1, 1, 1)
inline constexpr auto XVONE = XVCONSTANT<1>;

/// constant vector of (-1, -1, -1, -1)
inline constexpr auto XVNEGONE = XVCONSTANT<-1>;

/// constant vector of (1, 0, 0, 0)
inline constexpr auto XVX = XVCONSTANT<1, 0, 0, 0>;

/// constant vector of (0, 1, 0, 0)
inline constexpr auto XVY = XVCONSTANT<0, 1, 0, 0>;

/// constant vector of (0, 0, 1, 0)
inline constexpr auto XVZ = XVCONSTANT<0, 0, 1, 0>;

/// constant vector of (0, 0, 0, 1)
inline constexpr auto XVW = XVCONSTANT<0, 0, 0, 1>;

/// constant vector of (-1, 0, 0, 0)
inline constexpr auto XVNEGX = XVCONSTANT<-1, 0, 0, 0>;

/// constant vector of (0, -1, 0, 0)
inline constexpr auto XVNEGY = XVCONSTANT<0, -1, 0, 0>;

/// constant vector of (0, 0, -1, 0)
inline constexpr auto XVNEGZ = XVCONSTANT<0, 0, -1, 0>;

/// constant vector of (0, 0, 0, -1)
inline constexpr auto XVNEGW = XVCONSTANT<0, 0, 0, -1>;

/// identity matrix
inline constexpr caster XVIDENTITY{
  []() noexcept -> const XMatrix& {
    static const XMatrix m{XVX, XVY, XVZ, XVW};
    return m;
  }
};


/// loads a vector from memory to `XVector`
inline XVector xvload(const fat4* p) noexcept { return _mm_load_ps(p); }

/// fills `XVector` with a scalar
inline XVector xvfill(const fat4 v) noexcept { return _mm_set1_ps(v); }

/// sets `XVector` with 4 scalars
inline XVector xvset(const fat4 x, const fat4 y, const fat4 z, const fat4 w) noexcept {
  return _mm_set_ps(w, z, y, x);
}

/// stores `XVector` to memory
inline void xvstore(fat4* p, const XVector& v) noexcept { _mm_store_ps(p, v); }

/// inserts a scalar to `I`-th element of `XVector`
template<nat I> inline XVector xvinsert(const XVector& v, const fat4 x) noexcept {
  return _mm_castsi128_ps(_mm_insert_epi32(_mm_castps_si128(v), bitcast<int4>(x), I));
}

/// inserts `J`-th element of `b` to `I`-th element of `a` and setzero by `Mask`
/// \param a the target `XVector`; one of this element will be replaced
/// \param b the source `XVector`; one of this element will be used to replace
/// \note `a[I] = b[J]; for (nat i = 0; i < 4; ++i) if (Mask & (1 << i)) a[i] = 0;`
template<nat I, nat J, nat Mask = 0b0000>
requires (I < 4 && J < 4 && Mask < 16)
inline XVector xvinsert(const XVector& a, const XVector& b) noexcept {
  return _mm_insert_ps(a, b, (I << 4) | (J << 6) | Mask);
}

/// extracts `I`-th element of `XVector`
template<nat I> inline fat4 xvextract(const XVector& v) noexcept {
  if constexpr (I == 0) return _mm_cvtss_f32(v);
  else return bitcast<fat4>(_mm_extract_ps(v, I << 4));
}


/// blends two `XVector`s with the specified mask
template<bool X, bool Y, bool Z, bool W>
inline XVector xvblend(const XVector& a, const XVector& b) noexcept {
  return _mm_blend_ps(a, b, (X << 3) | (Y << 2) | (Z << 1) | W);
}

/// permutes `XVector` with the specified indices
template<nat X, nat Y, nat Z, nat W>
requires (X < 4 && Y < 4 && Z < 4 && W < 4)
inline XVector xvpermute(const XVector& v) noexcept {
  if constexpr (X == 0 && Y == 1 && Z == 2 && W == 3) return v;
  else if constexpr (X == 0 && Y == 0 && Z == 2 && W == 2) return _mm_moveldup_ps(v);
  else if constexpr (X == 1 && Y == 1 && Z == 3 && W == 3) return _mm_movehdup_ps(v);
  else return _mm_permute_ps(v, (W << 6) | (Z << 4) | (Y << 2) | X);
}

/// permutes two `XVector`s with the specified indices
template<nat X, nat Y, nat Z, nat W>
requires (X < 8 && Y < 8 && Z < 8 && W < 8)
inline XVector xvpermute(const XVector& a, const XVector& b) noexcept {
  if constexpr (X < 4 && Y < 4 && Z < 4 && W < 4)
    return xvpermute<X, Y, Z, W>(a);
  else if constexpr (X >= 4 && Y >= 4 && Z >= 4 && W >= 4)
    return xvpermute<X - 4, Y - 4, Z - 4, W - 4>(b);
  else if constexpr ((X & 3) == 0 && (Y & 3) == 1 && (Z & 3) == 2 && (W & 3) == 3)
    return xvblend<X < 4, Y < 4, Z < 4, W < 4>(b, a);
  else if constexpr (X < 4 && Y < 4 && Z >= 4 && W >= 4)
    return _mm_shuffle_ps(a, b, ((W & 3) << 6) | ((Z & 3) << 4) | (Y << 2) | X);
  else if constexpr (X >= 4 && Y >= 4 && Z < 4 && W < 4)
    return _mm_shuffle_ps(b, a, (W << 6) | (Z << 4) | ((Y & 3) << 2) | (X & 3));
  else if constexpr (X == 0 && Y == 4 && X == 1 && Y == 5)
    return _mm_unpacklo_ps(a, b);
  else if constexpr (X == 4 && Y == 0 && X == 5 && Y == 1)
    return _mm_unpacklo_ps(b, a);
  else if constexpr (X == 2 && Y == 6 && X == 3 && Y == 7)
    return _mm_unpackhi_ps(a, b);
  else if constexpr (X == 6 && Y == 2 && X == 7 && Y == 3)
    return _mm_unpackhi_ps(b, a);
  else if constexpr ((X == 0) + (Y == 1) + (Z == 2) + (W == 3) == 3) {
    constexpr nat i = inspects<X != 0, Y != 1, Z != 2, W != 3>;
    constexpr nat j = select_value<i, X, Y, Z, W> - 4;
    return _mm_insert_ps(a, b, int(j << 6 | i << 4));
  } else if constexpr ((X == 4) + (Y == 5) + (Z == 6) + (W == 7) == 3) {
    constexpr nat i = inspects<X != 4, Y != 5, Z != 6, W != 7>;
    constexpr nat j = select_value<i, X, Y, Z, W>;
    return _mm_insert_ps(b, a, int(j << 6 | i << 4));
  } else if constexpr ((X < 4 || X == 4) && (Y < 4 || Y == 5) && (Z < 4 || Z == 6) && (W < 4 || W == 7))
    return xvblend<X == 4, Y == 5, Z == 6, W == 7>(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(a), b);
  else if constexpr ((X >= 4 || X == 0) && (Y >= 4 || Y == 1) && (Z >= 4 || Z == 2) && (W >= 4 || W == 3))
    return xvblend<X == 0, Y == 1, Z == 2, W == 3>(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(b), a);
  else if constexpr ((X < 4) + (Y < 4) + (Z < 4) + (W < 4) == 1) {
    constexpr nat i = inspects<X < 4, Y < 4, Z < 4, W < 4>;
    constexpr nat j = select_value<i, X, Y, Z, W>;
    return _mm_insert_ps(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(b), a, int(j << 6 | i << 4));
  } else if constexpr ((X >= 4) + (Y >= 4) + (Z >= 4) + (W >= 4) == 1) {
    constexpr nat i = inspects<X >= 4, Y >= 4, Z >= 4, W >= 4>;
    constexpr nat j = select_value<i, X, Y, Z, W> - 4;
    return _mm_insert_ps(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(a), b, int(j << 6 | i << 4));
  } else return xvblend<X < 4, Y < 4, Z < 4, W < 4>(
    xvpermute<X & 3, Y & 3, Z & 3, W & 3>(a), xvpermute<X & 3, Y & 3, Z & 3, W & 3>(b));
}

/// adds two `XVector`s
inline XVector xvadd(const XVector& a, const XVector& b) noexcept {
  return _mm_add_ps(a, b);
}

/// subtracts two `XVector`s
inline XVector xvsub(const XVector& a, const XVector& b) noexcept {
  return _mm_sub_ps(a, b);
}

/// multiplies two `XVector`s
inline XVector xvmul(const XVector& a, const XVector& b) noexcept {
  return _mm_mul_ps(a, b);
}

/// divides two `XVector`s
inline XVector xvdiv(const XVector& a, const XVector& b) noexcept {
  return _mm_div_ps(a, b);
}

/// calculates the negation of an `XVector`
inline XVector xvneg(const XVector& v) noexcept {
  return _mm_xor_ps(v, XVNEGZERO);
}

/// calculates the absolute value of an `XVector`
inline XVector xvabs(const XVector& v) noexcept {
  return _mm_andnot_ps(XVNEGZERO, v);
}

/// compares two `XVector`s for equality
inline bool xveq(const XVector& a, const XVector& b) noexcept {
  return _mm_test_all_ones(_mm_castps_si128(_mm_cmpeq_ps(a, b)));
}

/// compares two `XVector`s for inequality
inline bool xvne(const XVector& a, const XVector& b) noexcept {
  return !xveq(a, b);
}

/// compares two `XVector`s for less than
inline bool xvlt(const XVector& a, const XVector& b) noexcept {
  return _mm_movemask_ps(xvpermute<3, 2, 1, 0>(xvsub(b, a))) <
         _mm_movemask_ps(xvpermute<3, 2, 1, 0>(xvsub(a, b)));
}

/// compares two `XVector`s for less than or equal
inline bool xvle(const XVector& a, const XVector& b) noexcept {
  return _mm_movemask_ps(xvpermute<3, 2, 1, 0>(xvsub(b, a))) <=
         _mm_movemask_ps(xvpermute<3, 2, 1, 0>(xvsub(a, b)));
}

/// compares two `XVector`s for greater than
inline bool xvgt(const XVector& a, const XVector& b) noexcept {
  return xvlt(b, a);
}

/// compares two `XVector`s for greater than or equal
inline bool xvge(const XVector& a, const XVector& b) noexcept {
  return xvle(b, a);
}

/// compares two `XVector`s for three-way comparison
inline auto xvtw(const XVector& a, const XVector& b) noexcept {
  return _mm_movemask_ps(xvpermute<3, 2, 1, 0>(xvsub(a, b))) <=>
         _mm_movemask_ps(xvpermute<3, 2, 1, 0>(xvsub(b, a)));
}

/// performs floor operation on an `XVector`
inline XVector xvfloor(const XVector& v) noexcept { return _mm_floor_ps(v); }

/// performs ceil operation on an `XVector`
inline XVector xvceil(const XVector& v) noexcept { return _mm_ceil_ps(v); }

/// performs round operation on an `XVector`
inline XVector xvround(const XVector& v) noexcept { return _mm_round_ps(v, 8); }

/// performs trunc operation on an `XVector`
inline XVector xvtrunc(const XVector& v) noexcept { return _mm_trunc_ps(v); }

/// calculates the minimum of two `XVector`s
inline XVector xvmin(const XVector& a, const XVector& b) noexcept {
  return _mm_min_ps(a, b);
}

/// calculates the maximum of two `XVector`s
inline XVector xvmax(const XVector& a, const XVector& b) noexcept {
  return _mm_max_ps(a, b);
}

/// calculates cosine
inline XVector xvcos(const XVector& v) noexcept { return _mm_cos_ps(v); }

/// calculates sine
inline XVector xvsin(const XVector& v) noexcept { return _mm_sin_ps(v); }

/// calculates tangent
inline XVector xvtan(const XVector& v) noexcept { return _mm_tan_ps(v); }

/// calculates arccosine
inline XVector xvacos(const XVector& v) noexcept { return _mm_acos_ps(v); }

/// calculates arcsine
inline XVector xvasin(const XVector& v) noexcept { return _mm_asin_ps(v); }

/// calculates arctangent
inline XVector xvatan(const XVector& v) noexcept { return _mm_atan_ps(v); }

/// calculates arctangent of `y / x`
inline XVector xvatan2(const XVector& y, const XVector& x) noexcept {
  return _mm_atan2_ps(y, x);
}

/// calculates hyperbolic cosine
inline XVector xvcosh(const XVector& v) noexcept { return _mm_cosh_ps(v); }

/// calculates hyperbolic sine
inline XVector xvsinh(const XVector& v) noexcept { return _mm_sinh_ps(v); }

/// calculates hyperbolic tangent
inline XVector xvtanh(const XVector& v) noexcept { return _mm_tanh_ps(v); }

/// calculates hyperbolic arccosine
inline XVector xvacosh(const XVector& v) noexcept { return _mm_acosh_ps(v); }

/// calculates hyperbolic arcsine
inline XVector xvasinh(const XVector& v) noexcept { return _mm_asinh_ps(v); }

/// calculates hyperbolic arctangent
inline XVector xvatanh(const XVector& v) noexcept { return _mm_atanh_ps(v); }

/// performs power operation on an `XVector`
inline XVector xvpow(const XVector& a, const XVector& b) noexcept {
  return _mm_pow_ps(a, b);
}

/// calculates the exponential of an `XVector`
inline XVector xvexp(const XVector& v) noexcept { return _mm_exp_ps(v); }

/// performs `exp2` operation on an `XVector`
inline XVector xvexp2(const XVector& v) noexcept { return _mm_exp2_ps(v); }

/// performs `exp10` operation on an `XVector`
inline XVector xvexp10(const XVector& v) noexcept { return _mm_exp10_ps(v); }

/// performs `expm1` operation on an `XVector`
inline XVector xvexpm1(const XVector& v) noexcept { return _mm_expm1_ps(v); }

/// performs `ln` operation on an `XVector`
inline XVector xvln(const XVector& v) noexcept { return _mm_log_ps(v); }

/// performs `log` operation on an `XVector`
inline XVector xvlog(const XVector& v, const XVector& Base) noexcept {
  return xvdiv(_mm_log_ps(v), _mm_log_ps(Base));
}

/// performs `log2` operation on an `XVector`
inline XVector xvlog2(const XVector& v) noexcept { return _mm_log2_ps(v); }

/// performs `log10` operation on an `XVector`
inline XVector xvlog10(const XVector& v) noexcept { return _mm_log10_ps(v); }

/// performs `log1p` operation on an `XVector`
inline XVector xvlog1p(const XVector& v) noexcept { return _mm_log1p_ps(v); }

/// performs `logb` operation on an `XVector`
inline XVector xvlogb(const XVector& v) noexcept { return _mm_logb_ps(v); }

/// calculates the square root of an `XVector`
inline XVector xvsqrt(const XVector& v) noexcept { return _mm_sqrt_ps(v); }

/// calculates the inverse square root of an `XVector`
inline XVector xvsqrt_r(const XVector& v) noexcept {
  return _mm_invsqrt_ps(v);
}

/// calculates the cube root of an `XVector`
inline XVector xvcbrt(const XVector& v) noexcept { return _mm_cbrt_ps(v); }

/// calculates the inverse cube root of an `XVector`
inline XVector xvcbrt_r(const XVector& v) noexcept {
  return _mm_invcbrt_ps(v);
}

/// calculates the hypotenuse of two `XVector`s
inline XVector xvhypot(const XVector& a, const XVector& b) noexcept {
  return _mm_hypot_ps(a, b);
}

/// calculates the error function of an `XVector`
inline XVector xverf(const XVector& v) noexcept { return _mm_erf_ps(v); }

/// calculates the inverse error function of an `XVector`
inline XVector xverf_r(const XVector& v) noexcept { return _mm_erfinv_ps(v); }

/// calculates the complementary error function of an `XVector`
inline XVector xverfc(const XVector& v) noexcept { return _mm_erfc_ps(v); }

/// calculates the inverse complementary error function of an `XVector`
inline XVector xverfc_r(const XVector& v) noexcept {
  return _mm_erfcinv_ps(v);
}

/// calculates the reciprocal of an `XVector`
inline XVector xvrcp(const XVector& v) noexcept { return _mm_rcp_ps(v); }

/// calculates the horizontal sum of an `XVector`
inline XVector xvsum(const XVector& v) noexcept {
  auto a = _mm_hadd_ps(v, v);
  return _mm_hadd_ps(a, a);
}

/// calculates the dot product of two `XVector`s
/// \return `xvfill(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w)`
inline XVector xvdot(const XVector& a, const XVector& b) noexcept {
  return _mm_dp_ps(a, b, 0xff);
}

/// calculates the dot product of `XMVector` and transposed `XVector`
inline XVector xvdot(const XMatrix& m, const XVector& v) noexcept {
  auto r = _mm_dp_ps(m[0], v, 0xf1);
  r = _mm_or_ps(r, _mm_dp_ps(m[1], v, 0xf2));
  r = _mm_or_ps(r, _mm_dp_ps(m[2], v, 0xf4));
  return _mm_or_ps(r, _mm_dp_ps(m[3], v, 0xf8));
}

/// calculates the dot product of `XVector` and `XMVector`
inline XVector xvdot(const XVector& v, const XMatrix& m) noexcept {
  auto r = xvmul(m[0], xvpermute<0, 0, 0, 0>(v));
  r = xvadd(r, xvmul(m[1], xvpermute<1, 1, 1, 1>(v)));
  r = xvadd(r, xvmul(m[2], xvpermute<2, 2, 2, 2>(v)));
  return xvadd(r, xvmul(m[3], xvpermute<3, 3, 3, 3>(v)));
}

/// calculates the dot product of two `XMatrix`s
inline void xvdot(const XMatrix& a, const XMatrix& b, XMatrix& r) noexcept {
  r[0] = xvdot(a, b[0]);
  r[1] = xvdot(a, b[1]);
  r[2] = xvdot(a, b[2]);
  r[3] = xvdot(a, b[3]);
}

/// calculates the dot product of two `XMatrix`s
/// \note the result is stored in the first `XMatrix`
inline void xvdot(XMatrix& a, const XMatrix& b) noexcept {
  auto t = xvmul(b[0], xvpermute<0, 0, 0, 0>(a[0]));
  t = xvadd(t, xvmul(b[1], xvpermute<1, 1, 1, 1>(a[0])));
  t = xvadd(t, xvmul(b[2], xvpermute<2, 2, 2, 2>(a[0])));
  a[0] = xvadd(t, xvmul(b[3], xvpermute<3, 3, 3, 3>(a[0])));
  t = xvmul(b[0], xvpermute<0, 0, 0, 0>(a[1]));
  t = xvadd(t, xvmul(b[1], xvpermute<1, 1, 1, 1>(a[1])));
  t = xvadd(t, xvmul(b[2], xvpermute<2, 2, 2, 2>(a[1])));
  a[1] = xvadd(t, xvmul(b[3], xvpermute<3, 3, 3, 3>(a[1])));
  t = xvmul(b[0], xvpermute<0, 0, 0, 0>(a[2]));
  t = xvadd(t, xvmul(b[1], xvpermute<1, 1, 1, 1>(a[2])));
  t = xvadd(t, xvmul(b[2], xvpermute<2, 2, 2, 2>(a[2])));
  a[2] = xvadd(t, xvmul(b[3], xvpermute<3, 3, 3, 3>(a[2])));
  t = xvmul(b[0], xvpermute<0, 0, 0, 0>(a[3]));
  t = xvadd(t, xvmul(b[1], xvpermute<1, 1, 1, 1>(a[3])));
  t = xvadd(t, xvmul(b[2], xvpermute<2, 2, 2, 2>(a[3])));
  a[3] = xvadd(t, xvmul(b[3], xvpermute<3, 3, 3, 3>(a[3])));
}

/// calculates the cross product of two `XVector`s
/// \return `XVector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 0)`
inline XVector xvcross(const XVector& a, const XVector& b) noexcept {
  auto c = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
  auto d = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2));
  auto e = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2));
  auto f = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
  return xvsub(xvmul(c, d), xvmul(e, f));
}

/// calculates the length of an `XVector`
/// \return `xvfill(sqrt(x * x + y * y + z * z + w * w))`
inline XVector xvlength(const XVector& v) noexcept {
  return xvsqrt(xvdot(v, v));
}

/// normalizes an `XVector`
/// \return `xvdiv(v, xvlength(v))`
inline XVector xvnormalize(const XVector& v) noexcept {
  return xvdiv(v, xvlength(v));
}

/// calculates the distance between two `XVector`s
/// \return `xvlength(xvsub(a, b))`
inline XVector xvdistance(const XVector& a, const XVector& b) noexcept {
  return xvlength(xvsub(a, b));
}


/// calculates the angle between two `XVector`s
/// \return `xvfill(acos(xvdot(a, b) / (xvlength(a) * xvlength(b))))`
inline XVector xvangle(const XVector& a, const XVector& b) noexcept {
  auto c = xvdot(a, b);
  return xvacos(xvsqrt(xvdiv(xvmul(c, c), xvmul(xvdot(a, a), xvdot(b, b)))));
}

/// calculates the projection of `a` onto `b`
/// \return `xvmul(b, xvdot(a, b) / xvdot(b, b))`
inline XVector xvproject(const XVector& a, const XVector& b) noexcept {
  return xvmul(b, xvdiv(xvdot(a, b), xvdot(b, b)));
}

/// calculates the rejection of `a` from `b`
/// \return `xvsub(a, xvproject(a, b))`
inline XVector xvreject(const XVector& a, const XVector& b) noexcept {
  return xvsub(a, xvproject(a, b));
}


/// converts degrees to radians
inline XVector xvradian(const XVector& v) noexcept {
  return xvmul(v, XVCONSTANT<pi / 180>);
}

/// converts radians to degrees
inline XVector xvdegree(const XVector& v) noexcept {
  return xvmul(v, XVCONSTANT<180 / pi>);
}

/// transposes an `XMatrix`
inline void xvtranspose(const XMatrix& m, XMatrix& r) noexcept {
  auto a = _mm_unpacklo_ps(m[0], m[1]); // a0, b0, a1, b1
  auto b = _mm_unpacklo_ps(m[2], m[3]); // c0, d0, c1, d1
  r[0] = _mm_movelh_ps(a, b);           // a0, b0, c0, d0
  r[1] = _mm_movehl_ps(b, a);           // a1, b1, c1, d1
  a = _mm_unpackhi_ps(m[0], m[1]);      // a2, b2, c2, d2
  b = _mm_unpackhi_ps(m[2], m[3]);      // a3, b3, c3, d3
  r[2] = _mm_movelh_ps(a, b);           // a2, b2, c2, d2
  r[3] = _mm_movehl_ps(b, a);           // a3, b3, c3, d3
}

/// transposes an `XMatrix`
inline void xvtranspose(XMatrix& m) noexcept {
  auto a = _mm_unpacklo_ps(m[0], m[1]); // a0, b0, a1, b1
  auto b = _mm_unpacklo_ps(m[2], m[3]); // c0, d0, c1, d1
  auto c = _mm_unpackhi_ps(m[0], m[1]); // a2, b2, a3, b3
  m[0] = _mm_movelh_ps(a, b);           // a0, b0, c0, d0
  m[1] = _mm_movehl_ps(b, a);           // a1, b1, c1, d1
  a = _mm_unpackhi_ps(m[2], m[3]);      // c2, d2, c3, d3
  m[2] = _mm_movelh_ps(c, a);           // a2, b2, c2, d2
  m[3] = _mm_movehl_ps(a, c);           // a3, b3, c3, d3
}

/// rotation matrix around the x-axis
inline void xvrotation_x(numeric auto&& Radian, XMatrix& r) noexcept {
  auto t = fat4(Radian);
  r[2] = xvset(0, t, -t, 0);
  r[3] = _mm_sincos_ps(&r[0], r[2]);
  r[1] = xvpermute<0, 5, 2, 3>(r[3], r[0]); // 0, c, -s, 0
  r[2] = xvpermute<0, 1, 6, 3>(r[3], r[0]); // 0, s,  c, 0
  r[0] = XVX, r[3] = XVW;
}

/// rotation matrix around the y-axis
inline void xvrotation_y(numeric auto&& Radian, XMatrix& r) noexcept {
  auto t = fat4(Radian);
  r[0] = xvset(-t, 0, t, 0);
  r[3] = _mm_sincos_ps(&r[1], r[0]);
  r[2] = xvpermute<0, 1, 6, 3>(r[3], r[1]); // -s, 0, c, 0
  r[0] = xvpermute<4, 5, 2, 3>(r[3], r[1]); //  c, 0, s, 0
  r[1] = XVY, r[3] = XVW;
}

/// rotation matrix around the z-axis
inline void xvrotation_z(numeric auto&& Radian, XMatrix& r) noexcept {
  auto t = fat4(Radian);
  r[1] = xvset(t, -t, 0, 0);
  r[3] = _mm_sincos_ps(&r[2], r[1]);
  r[0] = xvpermute<4, 5, 2, 3>(r[3], r[2]); //  c, -s, 0, 0
  r[1] = xvpermute<0, 5, 2, 3>(r[3], r[2]); //  s,  c, 0, 0
  r[2] = XVZ, r[3] = XVW;
}

/// rotation matrix around the x-axis, y-axis, and z-axis
/// \param Radians {x, y, z, undef}
/// \param r result
inline void xvrotation(const XVector& Radians, XMatrix& r) noexcept {
  r[0] = _mm_sincos_ps(&r[1], Radians);
  r[2] = xvpermute<0, 2, 4, 6>(r[1], r[0]);
  r[3] = xvpermute<3, 0, 1, 2>(r[2]);
  r[2] = xvmul(r[2], r[3]);
  r[1] = xvmul(r[3], xvpermute<1, 1, 1, 1>(r[1]));
  r[0] = xvpermute<1, 1, 1, 1>(r[0]);
  r[3] = _mm_addsub_ps(xvpermute<2, 3, 0, 1>(xvmul(r[0], r[2])), r[2]);
  r[2] = xvinsert<0, 0, 0b1000>(xvpermute<3, 3, 1, 1>(r[1]), xvneg(r[0]));
  r[0] = xvinsert<0, 2, 0b1000>(xvpermute<0, 0, 3, 3>(r[3]), r[1]);
  r[1] = xvinsert<0, 0, 0b1000>(r[3], r[1]);
  r[3] = XVW;
}

/// inverse rotation matrix around the x-axis, y-axis, and z-axis
/// \param Radians {x, y, z, undef}
/// \param r result
inline void xvrotation_inv(const XVector& Radians, XMatrix& r) noexcept {
  r[0] = _mm_sincos_ps(&r[1], Radians);
  r[2] = xvpermute<4, 6, 0, 2>(r[0], r[1]);
  r[3] = xvpermute<3, 0, 1, 2>(r[2]);
  r[2] = xvmul(r[2], r[3]);
  r[1] = xvmul(xvpermute<1, 1, 1, 1>(r[1]), r[3]);
  r[0] = xvpermute<1, 1, 1, 1>(r[0]);
  r[3] = _mm_addsub_ps(xvpermute<2, 3, 0, 1>(xvmul(r[0], r[2])), r[2]);
  r[2] = xvinsert<2, 1, 0b1000>(xvpermute<3, 2, 3, 2>(r[3]), r[1]);
  r[0] = xvinsert<2, 2, 0b1000>(xvpermute<2, 0, 2, 0>(r[1]), xvneg(r[0]));
  r[1] = xvinsert<2, 3, 0b1000>(r[3], r[1]);
  r[3] = XVW;
}

/// scaling matrix
/// \param Scales {x, y, z, undef}
/// \param r result
inline void xvscale(const XVector& Scales, XMatrix& r) noexcept {
  r[0] = xvmul(XVX, Scales);
  r[1] = xvmul(XVY, Scales);
  r[2] = xvmul(XVZ, Scales);
  r[3] = XVW;
}

/// translation matrix
/// \param Offsets {x, y, z, undef}
/// \param r result
inline void xvtranslation(const XVector& Offsets, XMatrix& r) noexcept {
  r[0] = xvpermute<0, 1, 2, 4>(XVX, Offsets);
  r[1] = xvpermute<0, 1, 2, 5>(XVY, Offsets);
  r[2] = xvpermute<0, 1, 2, 6>(XVZ, Offsets);
  r[3] = XVW;
}

/// world matrix
inline void xvworld(const XVector& Radians, const XVector& Offsets, XMatrix& r) noexcept {
  xvrotation(Radians, r);
  r[0] = xvpermute<0, 1, 2, 4>(r[0], Offsets);
  r[1] = xvpermute<0, 1, 2, 5>(r[1], Offsets);
  r[2] = xvpermute<0, 1, 2, 6>(r[2], Offsets);
}

/// world matrix
inline void xvworld(const XVector& Scales, const XVector& Radians, const XVector& Offsets, XMatrix& r) noexcept {
  xvworld(Radians, Offsets, r);
  r[0] = xvmul(r[0], xvpermute<0, 0, 0, 0>(Scales));
  r[1] = xvmul(r[1], xvpermute<1, 1, 1, 1>(Scales));
  r[2] = xvmul(r[2], xvpermute<2, 2, 2, 2>(Scales));
}

/// view matrix
inline void xvview(const XVector& Radians, const XVector& Position, XMatrix& r) noexcept {
  xvrotation_inv(Radians, r);
  r[3] = xvneg(Position);
  r[0] = xvinsert<3, 3>(r[0], xvdot(r[0], r[3]));
  r[1] = xvinsert<3, 3>(r[1], xvdot(r[1], r[3]));
  r[2] = xvinsert<3, 3>(r[2], xvdot(r[2], r[3]));
  r[3] = XVW;
}

/// obtains view matrix
/// \param Offsets camera lens offset from the camera position
/// \param Radians rotation angles of the camera
/// \param Position camera position in the world
/// \param r result
inline void xvview(const XVector& Offsets, const XVector& Radians, const XVector& Position, XMatrix& r) {
  xvrotation_inv(Radians, r);
  r[3] = xvneg(Position);
  r[0] = xvsub(xvinsert<3, 3>(r[0], xvdot(r[0], r[3])), xvinsert<3, 0, 0b0111>(Offsets, Offsets));
  r[1] = xvsub(xvinsert<3, 3>(r[1], xvdot(r[1], r[3])), xvinsert<3, 1, 0b0111>(Offsets, Offsets));
  r[2] = xvsub(xvinsert<3, 3>(r[2], xvdot(r[2], r[3])), xvinsert<3, 2, 0b0111>(Offsets, Offsets));
  r[3] = XVW;
}

/// obtains parallel projection matrix
/// \param Width width of the view
/// \param Height height of the view
/// \param Fov field of view
/// \param r result
inline void xvprojection(numeric auto&& Width, numeric auto&& Height, numeric auto&& Fov, XMatrix& r) noexcept {
  constexpr fat4 f = 1048576, n = 0.25;
  fat4 t = std::tan(Fov * 0.5);
  r[0] = xvinsert<0>(XVZERO, -Height / (Width * t));
  r[1] = xvinsert<1>(XVZERO, 1 / t);
  r[2] = XVCONSTANT<0, 0, f / (f - n), -f * n / (f - n)>;
  r[3] = XVW;
}

/// obtains orthographic projection matrix
/// \param Width width of the view
/// \param Height height of the view
/// \param Factor scaling factor
/// \param r result
inline void xvprojection_ortho(numeric auto&& Width, numeric auto&& Height, numeric auto&& Factor, XMatrix& r) noexcept {
  constexpr fat4 f = 1048576, n = 0.25;
  r[0] = xvinsert<0>(XVZERO, -2.0f * Factor / Width);
  r[1] = xvinsert<1>(XVZERO, 2.0f * Factor / Height);
  r[2] = XVCONSTANT<0, 0, 1 / (f - n), -n / (f - n)>;
  r[3] = XVW;
}

/// inverses a transformation matrix
/// \param m matrix to invert
/// \param r result
inline void xvinverse_transformation(const XMatrix& m, XMatrix& r) noexcept {
  xvtranspose(m, r);
  r[0] = xvblend<0, 0, 0, 1>(r[0], xvneg(m[0]));
  r[1] = xvblend<0, 0, 0, 1>(r[1], xvneg(m[1]));
  r[2] = xvblend<0, 0, 0, 1>(r[2], xvneg(m[2]));
  r[3] = XVW;
}

/// obtains euler angles from a rotation matrix
/// \param m rotation matrix
/// \return euler angles
inline XVector xveuler(const XMatrix& m) noexcept {
  auto a = xvneg(xvpermute<3, 0, 3, 3>(m[2]));
  if (xveq(a, XVY)) return xvasin(xvinsert<0, 1, 0b1100>(a, m[0]));
  a = xvpermute<4, 1, 0, 3>(a, xvdiv(m[1], m[0]));
  a = xvpermute<0, 1, 6, 3>(a, xvdiv(xvpermute<0, 0, 1, 1>(m[2]), m[2]));
  return xvpermute<6, 1, 4, 3>(xvasin(a), xvatan2(xvpermute<0, 1, 5, 3>(m[1], m[2]), xvblend<0, 0, 1, 1>(m[0], m[2])));
}

} // namespace yw
