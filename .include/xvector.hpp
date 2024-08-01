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
inline constexpr caster xv_constant{
  []() noexcept -> const XVector& {
    static const const XVector& v{_mm_set_ps(W, Z, Y, X)};
    return v;
  }
};

/// specialization for zero vector/matrix
template<> inline constexpr caster xv_constant<0, 0, 0, 0>{
  []() noexcept -> XVector { return _mm_setzero_ps(); },
  []() noexcept -> const XMatrix& {
    static const XMatrix m{_mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps(), _mm_setzero_ps()};
    return m;
  }
};

/// constant vector/matrix of zero
inline constexpr auto xv_zero = xv_constant<0>;

/// constant vector of (-0, -0, -0, -0)
inline constexpr auto xv_neg_zero = xv_constant<-0>;

/// constant vector of (1, 1, 1, 1)
inline constexpr auto xv_one = xv_constant<1>;

/// constant vector of (-1, -1, -1, -1)
inline constexpr auto xv_neg_one = xv_constant<-1>;

/// constant vector of (1, 0, 0, 0)
inline constexpr auto xv_x = xv_constant<1, 0, 0, 0>;

/// constant vector of (0, 1, 0, 0)
inline constexpr auto xv_y = xv_constant<0, 1, 0, 0>;

/// constant vector of (0, 0, 1, 0)
inline constexpr auto xv_z = xv_constant<0, 0, 1, 0>;

/// constant vector of (0, 0, 0, 1)
inline constexpr auto xv_w = xv_constant<0, 0, 0, 1>;

/// constant vector of (-1, 0, 0, 0)
inline constexpr auto xv_neg_x = xv_constant<-1, 0, 0, 0>;

/// constant vector of (0, -1, 0, 0)
inline constexpr auto xv_neg_y = xv_constant<0, -1, 0, 0>;

/// constant vector of (0, 0, -1, 0)
inline constexpr auto xv_neg_z = xv_constant<0, 0, -1, 0>;

/// constant vector of (0, 0, 0, -1)
inline constexpr auto xv_neg_w = xv_constant<0, 0, 0, -1>;


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
  return _mm_castsi128_ps(_mm_insert_epi32(_mm_castps_si128(v), bitcast<int3>(x), I));
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
    return xvblend<X & 4, Y & 4, Z & 4, W & 4>(a, b);
  else if constexpr (X < 4 && Y < 4 && Z >= 4 && W >= 4)
    return _mm_shuffle_ps(a, b, ((W & 3) << 6) | ((Z & 3) << 4) | (Y << 2) | X);
  else if constexpr (X >= 4 && Y >= 4 && Z < 4 && W < 4)
    return _mm_shuffle_ps(b, a, (W << 6) | (Z << 4) | ((Y & 3) << 2) | (X & 3));
  else if constexpr ((X == 0) + (Y == 1) + (Z == 2) + (W == 3) == 3) noexcept {
    constexpr nat i = inspects<X != 0, Y != 1, Z != 2, W != 3>;
    constexpr nat j = select_value<i, X, Y, Z, W> - 4;
    return _mm_shuffle_ps(a, b, int(j << 6 | i << 4));
  } else if constexpr ((X == 4) + (Y == 5) + (Z == 6) + (W == 7) == 3) noexcept {
    constexpr nat i = inspects<X != 4, Y != 5, Z != 6, W != 7>;
    constexpr nat j = select_value<i, X, Y, Z, W>;
    return _mm_shuffle_ps(b, a, int(j << 6 | i << 4));
  } else if constexpr ((X < 4 || X == 4) && (Y < 4 || Y == 5) && (Z < 4 || Z == 6) && (W < 4 || W == 7))
    return xvblend<x == 4, y == 5, z == 6, w == 7>(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(a), b);
  else if constexpr ((X >= 4 || X == 0) && (Y >= 4 || Y == 1) && (Z >= 4 || Z == 2) && (W >= 4 || W == 3))
    return xvblend<x == 0, y == 1, z == 2, w == 3>(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(b), a);
  else if constexpr ((X < 4) + (Y < 4) + (Z < 4) + (W < 4) == 1) noexcept {
    constexpr nat i = inspects<X < 4, Y < 4, Z < 4, W < 4>;
    constexpr nat j = select_value<i, X, Y, Z, W>;
    return _mm_insert_ps(xvpermute<X & 3, Y & 3, Z & 3, W & 3>(b), a, int(j << 6 | i << 4));
  } else if constexpr ((X >= 4) + (Y >= 4) + (Z >= 4) + (W >= 4) == 1) noexcept {
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
  return _mm_xor_ps(v, xv_neg_zero);
}

/// calculates the absolute value of an `XVector`
inline XVector xvabs(const XVector& v) noexcept {
  return _mm_andnot_ps(xv_neg_zero, v);
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
  return xvmul(v, xv_constant<pi / 180>);
}

/// converts radians to degrees
inline XVector xvdegree(const XVector& v) noexcept {
  return xvmul(v, xv_constant<180 / pi>);
}

} // namespace yw
