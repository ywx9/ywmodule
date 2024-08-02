/// \file vector.hpp
/// \brief defines `yw::Vector`

#pragma once

#include "xvector.hpp"

export namespace yw {

/// \brief 4D vector
struct Vector {
  fat4 x{}, y{}, z{}, w{};
  static constexpr nat count = 4;

  constexpr Vector() noexcept = default;

  explicit constexpr Vector(const fat4& Fill) noexcept : x(Fill), y(Fill), z(Fill), w(Fill) {}

  explicit constexpr Vector(const numeric auto&& Fill) noexcept : Vector(fat4(Fill)) {}

  constexpr Vector(const numeric auto&& X, const numeric auto&& Y,
                   const numeric auto&& Z, const numeric auto&& W) noexcept
    : x(fat4(X)), y(fat4(Y)), z(fat4(Z)), w(fat4(W)) {}

  constexpr Vector(const numeric auto&& X, const numeric auto&& Y,  const numeric auto&& Z) noexcept
    : x(fat4(X)), y(fat4(Y)), z(fat4(Z)) {}

  constexpr Vector(const numeric auto&& X, const numeric auto&& Y) noexcept
    : x(fat4(X)), y(fat4(Y)) {}

  Vector(const XVector& v) noexcept { _mm_store_ps(&x, v); }

  operator XVector() const noexcept { return _mm_load_ps(&x); }

  constexpr nat size() const noexcept { return count; }

  fat4* data() noexcept { return &x; }

  const fat4* data() const noexcept { return &x; }

  fat4& operator[](nat i) noexcept { return data()[i]; }

  fat4 operator[](nat i) const noexcept { return data()[i]; }

  fat4* begin() noexcept { return data(); }

  const fat4* begin() const noexcept { return data(); }

  fat4* end() noexcept { return data() + count; }

  const fat4* end() const noexcept { return data() + count; }

  template<nat I> requires (I < count)
  constexpr fat4& get() & noexcept {
    return select_parameter<I>(x, y, z, w);
  }

  template<nat I> requires (I < count)
  constexpr const fat4& get() const & noexcept {
    return select_parameter<I>(x, y, z, w);
  }

  template<nat I> requires (I < count)
  constexpr fat4&& get() && noexcept {
    return std::move(select_parameter<I>(x, y, z, w));
  }

  template<nat I> requires (I < count)
  constexpr const fat4&& get() const && noexcept {
    return std::move(select_parameter<I>(x, y, z, w));
  }

  friend constexpr bool operator==(const Vector& a, const Vector& b) noexcept {
    if (IS_CEV) return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    else return xveq(a, b);
  }

  friend constexpr po_t operator<=>(const Vector& a, const Vector& b) noexcept {
    if (IS_CEV) {
      if (auto c = a.x <=> b.x; c != 0) return c;
      else if (c = a.y <=> b.y; c != 0) return c;
      else if (c = a.z <=> b.z; c != 0) return c;
      else return a.w <=> b.w;
    } else return xvtw(a, b);
  }

  friend constexpr Vector operator+(const Vector& a) noexcept { return a; }

  friend constexpr Vector operator-(const Vector& a) noexcept {
    if (IS_CEV) return {-a.x, -a.y, -a.z, -a.w};
    else return xvneg(a);
  }

  friend constexpr Vector operator+(const Vector& a, const Vector& b) noexcept {
    if (IS_CEV) return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    else return xvadd(a, b);
  }

  friend constexpr Vector operator-(const Vector& a, const Vector& b) noexcept {
    if (IS_CEV) return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    else return xvsub(a, b);
  }

  friend constexpr Vector operator*(const Vector& a, const Vector& b) noexcept {
    if (IS_CEV) return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
    else return xvmul(a, b);
  }

  friend constexpr Vector operator/(const Vector& a, const Vector& b) noexcept {
    if (IS_CEV) return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
    else return xvdiv(a, b);
  }

  constexpr Vector& operator+=(const Vector& b) noexcept {
    if (IS_CEV) { x += b.x; y += b.y; z += b.z; w += b.w; return *this; }
    else return *this = xvadd(*this, b);
  }

  constexpr Vector& operator-=(const Vector& b) noexcept {
    if (IS_CEV) { x -= b.x; y -= b.y; z -= b.z; w -= b.w; return *this; }
    else return *this = xvsub(*this, b);
  }

  constexpr Vector& operator*=(const Vector& b) noexcept {
    if (IS_CEV) { x *= b.x; y *= b.y; z *= b.z; w *= b.w; return *this; }
    else return *this = xvmul(*this, b);
  }

  constexpr Vector& operator/=(const Vector& b) noexcept {
    if (IS_CEV) { x /= b.x; y /= b.y; z /= b.z; w /= b.w; return *this; }
    else return *this = xvdiv(*this, b);
  }

  friend constexpr Vector operator*(const Vector& a, numeric auto&& b) noexcept {
    if (IS_CEV) {
      auto f = fat4(b);
      return {a.x * f, a.y * f, a.z * f, a.w * f};
    } else return xvmul(a, xvfill(fat4(b)));
  }

  friend constexpr Vector operator*(numeric auto&& a, const Vector& b) noexcept { return b * a; }

  friend constexpr Vector operator/(const Vector& a, numeric auto&& b) noexcept {
    if (IS_CEV) {
      auto f = fat4(b);
      return {a.x / f, a.y / f, a.z / f, a.w / f};
    } else return xvdiv(a, xvfill(fat4(b)));
  }

  friend constexpr Vector operator/(numeric auto&& a, const Vector& b) noexcept {
    if (IS_CEV) {
      auto f = fat4(a);
      return {f / b.x, f / b.y, f / b.z, f / b.w};
    } else return xvdiv(xvfill(fat4(a)), b);
  }

  constexpr Vector& operator*=(numeric auto&& b) noexcept {
    if (IS_CEV) {
      auto f = fat4(b);
      x *= f; y *= f; z *= f; w *= f;
      return *this;
    } else return *this = xvmul(*this, xvfill(fat4(b)));
  }

  constexpr Vector& operator/=(numeric auto&& b) noexcept {
    if (IS_CEV) {
      auto f = fat4(b);
      x /= f; y /= f; z /= f; w /= f;
      return *this;
    } else return *this = xvdiv(*this, xvfill(fat4(b)));
  }

  constexpr fat4 power() const noexcept {
    if (IS_CEV) return x * x + y * y + z * z + w * w;
    else return [](XVector v) noexcept { return xvextract<0>(xvdot(v, v)); }(*this);
  }

  /// calculates the length of the vector
  fat4 length() const noexcept { return xvextract<0>(xvlength(*this)); }

  /// normalizes the vector
  Vector normalize() const noexcept { return xvnormalize(*this); }
};

} // namespace yw


namespace std {

template<> struct tuple_size<yw::Vector> : integral_constant<size_t, yw::Vector::count> {};

template<size_t I> requires (I < yw::Vector::count)
struct tuple_element<I, yw::Vector> : type_identity<float> {};

} // namespace std
