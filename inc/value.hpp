#pragma once

#ifndef YWLIB
#include <limits>
#include <numbers>
#else
import std;
#endif

#include "core.hpp"

export namespace yw {

using fat8 = double;

/// struct to represent a numeric value
struct Value {
  fat8 value{};
  constexpr Value() noexcept = default;
  constexpr Value(None) noexcept : value(std::numeric_limits<fat8>::quiet_NaN()) {}
  template<numeric T> requires(!same_as<T, Value>) constexpr Value(T&& v) noexcept : value(fat8(static_cast<T&&>(v))) {}
  constexpr operator fat8&() & noexcept { return value; }
  constexpr operator const fat8&() const& noexcept { return value; }
  constexpr operator fat8&&() && noexcept { return std::move(value); }
  constexpr operator const fat8&&() const&& noexcept { return std::move(value); }
  template<numeric T> explicit constexpr operator T() const
    noexcept(noexcept(T(value))) requires requires { T(value); } { return T(value); }
  template<numeric T> constexpr Value& operator+=(T&& v) noexcept { return _ += fat8(static_cast<T&&>(v)), *this; }
  template<numeric T> constexpr Value& operator-=(T&& v) noexcept { return _ -= fat8(static_cast<T&&>(v)), *this; }
  template<numeric T> constexpr Value& operator*=(T&& v) noexcept { return _ *= fat8(static_cast<T&&>(v)), *this; }
  template<numeric T> constexpr Value& operator/=(T&& v) noexcept { return _ /= fat8(static_cast<T&&>(v)), *this; }
};

inline constexpr Value e = std::numbers::e_v<fat8>;
inline constexpr Value pi = std::numbers::pi_v<fat8>;
inline constexpr Value phi = std::numbers::phi_v<fat8>;
inline constexpr Value gamma = std::numbers::egamma_v<fat8>;
inline constexpr Value ln2 = std::numbers::ln2_v<fat8>;
inline constexpr Value ln10 = std::numbers::ln10_v<fat8>;
inline constexpr Value log2_10 = 3.3219280948873623478703194294894;
inline constexpr Value log10_2 = 0.30102999566398119521373889472449;
inline constexpr Value sqrt2 = std::numbers::sqrt2_v<fat8>;
inline constexpr Value sqrt3 = std::numbers::sqrt3_v<fat8>;
inline constexpr Value inf = std::numeric_limits<fat8>::infinity();
inline constexpr Value nan = std::numeric_limits<fat8>::quiet_NaN();

} // namespace yw

namespace std {

template<typename T> struct common_type<yw::Value, T> : common_type<double, T> {};
template<typename T> struct common_type<T, yw::Value> : common_type<T, double> {};

template<typename Ct> struct formatter<yw::Value, Ct> : formatter<double, Ct> {
  auto format(yw::Value v, auto& Ctx) const {
    return formatter<double, Ct>::format(static_cast<double>(v), Ctx);
  }
};

} // namespace std
