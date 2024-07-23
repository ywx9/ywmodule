#pragma once

#ifndef YWLIB
#include <compare>
#include <format>
#else
import std;
#endif

export namespace yw {

/// struct to represent a null value
struct None {
  constexpr None() noexcept = default;
  constexpr None(auto&&...) noexcept {}
  constexpr None& operator=(auto&&) noexcept { return *this; }
  explicit constexpr operator bool() const noexcept { return false; }
  constexpr None operator()(auto&&...) const noexcept { return {}; }
  constexpr friend bool operator==(None, None) noexcept { return false; }
  constexpr friend auto operator<=>(None, None) noexcept { return std::partial_ordering::unordered; }
  constexpr friend None operator+(None) noexcept { return {}; }
  constexpr friend None operator-(None) noexcept { return {}; }
  constexpr friend None operator+(None, None) noexcept { return {}; }
  constexpr friend None operator-(None, None) noexcept { return {}; }
  constexpr friend None operator*(None, None) noexcept { return {}; }
  constexpr friend None operator/(None, None) noexcept { return {}; }
  constexpr None& operator+=(None) noexcept { return *this; }
  constexpr None& operator-=(None) noexcept { return *this; }
  constexpr None& operator*=(None) noexcept { return *this; }
  constexpr None& operator/=(None) noexcept { return *this; }
};

inline constexpr None none;
template<typename T> concept is_None = std::same_as<std::remove_cv_t<T>, None>;

} // namespace yw

namespace std {

template<typename T> struct common_type<yw::None, T> : type_identity<yw::None> {};
template<typename T> struct common_type<T, yw::None> : type_identity<yw::None> {};

template<typename Ct> struct formatter<yw::None, Ct> : formatter<const Ct*, Ct> {
  auto format(yw::None, auto& Ctx) const {
    constexpr const Ct text[] = { 'n', 'o', 'n', 'e', '\0' };
    return formatter<const Ct*, Ct>::format(text, Ctx);
  }
};

} // namespace std
