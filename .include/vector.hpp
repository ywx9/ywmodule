/// \file vector.hpp
/// \brief defines `Vector` and `Vector2`

#pragma once

#include "xvector.hpp"

export namespace yw {


/// struct to represent a 2D vector
struct Vector2 {

  /// x-coordinate
  fat4 x{};

  /// y-coordinate
  fat4 y{};

  /// number of elements
  static constexpr nat count = 2;

  /// default constructor
  constexpr Vector2() noexcept = default;

  /// constructor with the same value
  explicit constexpr Vector2(const fat4 Fill)
    noexcept : x(Fill), y(Fill) {}

  /// constructor with the same value
  explicit constexpr Vector2(numeric auto&& Fill)
    noexcept : Vector2(fat4(Fill)) {}

  /// constructor with the x and y coordinates
  constexpr Vector2(numeric auto&& X, numeric auto&& Y)
    noexcept : x(fat4(X)), y(fat4(Y)) {}

  /// number of elements
  constexpr nat size() const noexcept { return count; }

  /// pointer to the data
  fat4* data() noexcept { return &x; }

  /// pointer to the data (const)
  const fat4* data() const noexcept { return &x; }

  /// reference to the element
  fat4& operator[](nat i) noexcept { return data()[i]; }

  /// reference to the element (const)
  fat4 operator[](nat i) const noexcept { return data()[i]; }

  /// iterator to the first element
  fat4* begin() noexcept { return data(); }

  /// iterator to the first element (const)
  const fat4* begin() const noexcept { return data(); }

  /// iterator to the last element
  fat4* end() noexcept { return data() + count; }

  /// iterator to the last element (const)
  const fat4* end() const noexcept { return data() + count; }

  /// returns the element at the specified index
  template<nat I> requires (I < count)
  constexpr fat4& get() & noexcept {
    return select_parameter<I>(x, y);
  }

  /// returns the element at the specified index (const)
  template<nat I> requires (I < count)
  constexpr const fat4& get() const & noexcept {
    return select_parameter<I>(x, y);
  }

  /// returns the element at the specified index (rvalue)
  template<nat I> requires (I < count)
  constexpr fat4&& get() && noexcept {
    return std::move(select_parameter<I>(x, y));
  }

  /// returns the element at the specified index (const rvalue)
  template<nat I> requires (I < count)
  constexpr const fat4&& get() const && noexcept {
    return std::move(select_parameter<I>(x, y));
  }

  /// equality operator
  friend constexpr bool operator==(const Vector2& a, const Vector2& b)
    noexcept { return a.x == b.x && a.y == b.y; }

  /// comparison operator
  friend constexpr auto operator<=>(
    const Vector2& a, const Vector2& b) noexcept {
    if (auto c = a.x <=> b.x; c != 0) return c;
    else return a.y <=> b.y;
  }

  /// unary plus operator
  friend constexpr Vector2 operator+(const Vector2& a) noexcept { return a; }

  /// unary minus operator
  friend constexpr Vector2 operator-(const Vector2& a)
    noexcept { return {-a.x, -a.y}; }

  /// addition operator
  friend constexpr Vector2 operator+(const Vector2& a, const Vector2& b)
    noexcept { return {a.x + b.x, a.y + b.y}; }

  /// subtraction operator
  friend constexpr Vector2 operator-(const Vector2& a, const Vector2& b)
    noexcept { return {a.x - b.x, a.y - b.y}; }

  /// multiplication operator
  friend constexpr Vector2 operator*(const Vector2& a, const Vector2& b)
    noexcept { return {a.x * b.x, a.y * b.y}; }

  /// division operator
  friend constexpr Vector2 operator/(const Vector2& a, const Vector2& b)
    noexcept { return {a.x / b.x, a.y / b.y}; }

  /// addition assignment operator
  constexpr Vector2& operator+=(const Vector2& b) noexcept {
    x += b.x;
    y += b.y;
    return *this;
  }

  /// subtraction assignment operator
  constexpr Vector2& operator-=(const Vector2& b) noexcept {
    x -= b.x;
    y -= b.y;
    return *this;
  }

  /// multiplication assignment operator
  constexpr Vector2& operator*=(const Vector2& b) noexcept {
    x *= b.x;
    y *= b.y;
    return *this;
  }

  /// division assignment operator
  constexpr Vector2& operator/=(const Vector2& b) noexcept {
    x /= b.x;
    y /= b.y;
    return *this;
  }

  /// multiplication operator
  friend constexpr Vector2 operator*(
    const Vector2& a, numeric auto&& b) noexcept {
    auto f = fat4(b);
    return {a.x * f, a.y * f};
  }

  /// division operator
  friend constexpr Vector2 operator/(const Vector2& a, numeric auto&& b)
    noexcept {
    auto f = fat4(b);
    return {a.x / f, a.y / f};
  }

  /// multiplication assignment operator
  constexpr Vector2& operator*=(numeric auto&& b) noexcept {
    auto f = fat4(b);
    x *= f;
    y *= f;
    return *this;
  }

  /// division assignment operator
  constexpr Vector2& operator/=(numeric auto&& b) noexcept {
    auto f = fat4(b);
    x /= f;
    y /= f;
    return *this;
  }

  /// calculates the power of the vector
  constexpr fat4 power() const noexcept {
    return x * x + y * y;
  }

  /// calculates the length of the vector
  fat4 length() const noexcept {
    return std::sqrt(power());
  }

  /// normalizes the vector
  Vector2 normalize() const noexcept {
    return *this / length();
  }

}; ///////////////////////////////////////////////////////////////////////////// Vector2


/// \brief 4D vector
struct Vector {
  fat4 x{}, y{}, z{}, w{};
  static constexpr nat count = 4;

  constexpr Vector() noexcept = default;

  explicit constexpr Vector(const fat4& Fill) noexcept : x(Fill), y(Fill), z(Fill), w(Fill) {}

  explicit constexpr Vector(numeric auto&& Fill) noexcept : Vector(fat4(Fill)) {}

  constexpr Vector(numeric auto&& X, numeric auto&& Y,
                   numeric auto&& Z, numeric auto&& W) noexcept
    : x(fat4(X)), y(fat4(Y)), z(fat4(Z)), w(fat4(W)) {}

  constexpr Vector(numeric auto&& X, numeric auto&& Y,  numeric auto&& Z) noexcept
    : x(fat4(X)), y(fat4(Y)), z(fat4(Z)) {}

  constexpr Vector(numeric auto&& X, numeric auto&& Y) noexcept
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

template<> struct tuple_size<yw::Vector2>
  : integral_constant<size_t, yw::Vector2::count> {};
template<> struct tuple_size<yw::Vector>
  : integral_constant<size_t, yw::Vector::count> {};

template<size_t I> requires (I < yw::Vector2::count)
struct tuple_element<I, yw::Vector2> : type_identity<float> {};
template<size_t I> requires (I < yw::Vector::count)
struct tuple_element<I, yw::Vector> : type_identity<float> {};

} // namespace std
