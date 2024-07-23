#pragma once

#ifndef YWLIB
#include <vector>
#else
import std;
#endif

#include "core.hpp"

export namespace yw {

export template<typename T, nat N = npos> class Array {
public:
  T array[N] = {{}};
  static constexpr nat count = N;
  using value_type = T;
  template<convertible_to<T>> constexpr Array& operator=(const T (&a)[N]) { for (nat i{}; i < N; ++i) array[i] = a[i]; return *this; }
  constexpr operator add_lvref<T[N]>() noexcept { return array; }
  constexpr operator add_lvref<const T[N]>() const noexcept { return array; }
  constexpr T& operator[](nat I) { return array[I]; }
  constexpr const T& operator[](nat I) const { return array[I]; }
  constexpr nat size() const noexcept { return N; }
  constexpr bool empty() const noexcept { return false; }
  constexpr T* data() noexcept { return array; }
  constexpr const T* data() const noexcept { return array; }
  constexpr T* begin() noexcept { return array; }
  constexpr const T* begin() const noexcept { return array; }
  constexpr T* end() noexcept { return array + N; }
  constexpr const T* end() const noexcept { return array + N; }
  constexpr T& front() noexcept { return *array; }
  constexpr const T& front() const noexcept { return *array; }
  constexpr T& back() noexcept { return array[N - 1]; }
  constexpr const T& back() const noexcept { return array[N - 1]; }
  template<nat I> requires(I < N) constexpr T& get() & noexcept { return array[I]; }
  template<nat I> requires(I < N) constexpr T&& get() && noexcept { return mv(array[I]); }
  template<nat I> requires(I < N) constexpr const T& get() const& noexcept { return array[I]; }
  template<nat I> requires(I < N) constexpr const T&& get() const&& noexcept { return mv(array[I]); }
};

export template<typename T> class Array<T, 0> {
public:
  static constexpr nat count = 0;
  using value_type = T;
  constexpr nat size() const noexcept { return 0; }
  constexpr bool empty() const noexcept { return true; }
  T* data() noexcept { return reinterpret_cast<T*>(this); }
  const T* data() const noexcept { return reinterpret_cast<const T*>(this); }
  T* begin() noexcept { return reinterpret_cast<T*>(this); }
  const T* begin() const noexcept { return reinterpret_cast<const T*>(this); }
  T* end() noexcept { return reinterpret_cast<T*>(this); }
  const T* end() const noexcept { return reinterpret_cast<const T*>(this); }
};

export template<typename T> class Array<T, npos> : public std::vector<T> {
public:
  constexpr Array() noexcept = default;
  constexpr Array(std::vector<T>&& v) : std::vector<T>(mv(v)) {}
  constexpr explicit Array(nat n) : std::vector<T>(n) {}
  constexpr Array(nat n, const T& v) : std::vector<T>(n, v) {}
  template<iterator_for<T> It> constexpr Array(It i, It s) : std::vector<T>(i, s) {}
  template<iterator_for<T> It, sentinel_for<It> Se> requires(!same_as<It, Se>)
  constexpr Array(It i, Se s) : std::vector<T>(std::common_iterator<It, Se>(i), std::common_iterator<It, Se>(s)) {}
  template<range_for<T> Rg> constexpr Array(Rg&& r) : std::vector<T>(yw::begin(r), yw::end(r)) {}
};

export template<typename T, convertible_to<T>... Ts> Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;
export template<typename T> Array(nat, const T&) -> Array<T, npos>;
export template<iterator It, sentinel_for<It> Se> Array(It, Se) -> Array<iter_value<It>, npos>;
export template<range Rg> Array(Rg&&) -> Array<iter_value<Rg>, npos>;

} // namespace yw

namespace std {

export template<typename T, size_t N> requires (N != yw::npos) struct tuple_size<yw::Array<T, N>> : integral_constant<size_t, N> {};
export template<size_t I, typename T, size_t N> requires (N != yw::npos) struct tuple_element<I, yw::Array<T, N>> { using type = T; };

} // namespace std
