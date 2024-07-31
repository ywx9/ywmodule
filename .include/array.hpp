#pragma once

#ifndef YWLIB
#include <initializer_list>
#include <vector>
#else
import std;
#endif

#include "core.hpp"

export namespace yw {

/// class to represent a fixed-size array
template<typename T, nat N = npos> class Array {
public:
  T array[N] = {{}};
  static constexpr nat count = N;
  using value_type = T;
  constexpr Array& operator=(const T (&a)[N]) { for (nat i{}; i < N; ++i) array[i] = a[i]; return *this; }
  constexpr operator add_lvref<T[N]>() noexcept { return array; }
  constexpr operator add_lvref<const T[N]>() const noexcept { return array; }
  constexpr T& operator[](const nat I) { return array[I]; }
  constexpr const T& operator[](const nat I) const { return array[I]; }
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

/// class to represent a null array
template<typename T> class Array<T, 0> {
public:
  static constexpr nat count = 0;
  using value_type = T;
  constexpr nat size() const noexcept { return 0; }
  constexpr bool empty() const noexcept { return true; }
  constexpr np_t data() noexcept { return nullptr; }
  constexpr const np_t data() const noexcept { return nullptr; }
  constexpr np_t begin() noexcept { return nullptr; }
  constexpr const np_t begin() const noexcept { return nullptr; }
  constexpr np_t end() noexcept { return nullptr; }
  constexpr const np_t end() const noexcept { return nullptr; }
};

/// class to represent a dynamic array
template<typename T> class Array<T, npos> : public std::vector<T> {
public:
  constexpr Array() noexcept = default;
  constexpr Array(std::vector<T>&& v) : std::vector<T>(mv(v)) {}
  constexpr explicit Array(nat n) : std::vector<T>(n) {}
  constexpr Array(nat n, const T& v) : std::vector<T>(n, v) {}
  template<iterator_for<T> It> constexpr Array(It i, It s) : std::vector<T>(i, s) {}
  template<iterator_for<T> It, sentinel_for<It> Se> requires(!same_as<It, Se>)
  constexpr Array(It i, Se s) : std::vector<T>(std::common_iterator<It, Se>(i), std::common_iterator<It, Se>(s)) {}
  template<range_for<T> Rg> constexpr Array(Rg&& r) : std::vector<T>(yw::begin(r), yw::end(r)) {}
  explicit constexpr Array(std::initializer_list<T> il) : std::vector<T>(il) {}
};

// deduction guides
template<typename T, convertible_to<T>... Ts> Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;
template<typename T> Array(nat, const T&) -> Array<T, npos>;
template<iterator It, sentinel_for<It> Se> Array(It, Se) -> Array<iter_value<It>, npos>;
template<range Rg> Array(Rg&&) -> Array<iter_value<Rg>, npos>;

/// converts a range to a fixed-size array
/// \tparam N the size of the array
/// \param r (in) input range to convert
/// \return `Array<iter_value<decltype(r)>, N>`
template<nat N> constexpr auto to_array(input_range auto&& r)
   -> Array<iter_value<decltype(r)>, N> {
  Array<iter_value<decltype(r)>, N> a;
  auto b = yw::begin(r);
  for (nat i{}; i < N || b != r.end(); ++i, ++b) a[i] = *b;
  return a;
}

} // namespace yw

namespace std {

template<typename T, size_t N> requires (N != yw::npos)
struct tuple_size<yw::Array<T, N>> : integral_constant<size_t, N> {};

template<size_t I, typename T, size_t N> requires (N != yw::npos)
struct tuple_element<I, yw::Array<T, N>> { using type = T; };

} // namespace std

#ifndef YWLIB_SKIP_TEST
namespace test {
using namespace yw;
inline constexpr bool array_n = []() {
  Array<int, 3> a = {1, 2, 3};
  static_assert(a.count == 3);
  static_assert(same_as<decltype(a)::value_type, int>);
  static_assert(std::tuple_size_v<decltype(a)> == 3);
  static_assert(same_as<std::tuple_element_t<0, decltype(a)>, int>);
  if (a[0] != 1) return false;
  if (a[1] != 2) return false;
  if (a[2] != 3) return false;
  if (a.size() != 3) return false;
  if (a.empty()) return false;
  if (a.data() != a.begin()) return false;
  if (a.data() != &a[0]) return false;
  if (yw::get<0>(a) != 1) return false;
  int b[] = {4, 5, 6};
  a = b;
  if (a[0] != 4) return false;
  if (a[1] != 5) return false;
  if (a[2] != 6) return false;
  auto c = std::vector{7, 8, 9};
  a = to_array<3>(c);
  if (a[0] != 7) return false;
  if (a[1] != 8) return false;
  if (a[2] != 9) return false;
  return true;
}();
static_assert(array_n);

inline constexpr bool array_0 = []() {
  Array<int, 0> a;
  static_assert(a.count == 0);
  static_assert(std::tuple_size_v<decltype(a)> == 0);
  if (a.size() != 0) return false;
  if (!a.empty()) return false;
  if (a.data() != a.begin()) return false;
  if (a.begin() != a.end()) return false;
  return true;
}();
static_assert(array_0);

inline constexpr bool array_npos = []() {
  Array<int> a{1, 2, 3};
  if (a[0] != 1) return false;
  if (a[1] != 2) return false;
  if (a[2] != 3) return false;
  if (a.size() != 3) return false;
  if (a.empty()) return false;
  if (a.data() != &(*a.begin())) return false;
  if (a.data() != &a[0]) return false;
  int b[] = {4, 5, 6};
  a = b;
  if (a[0] != 4) return false;
  if (a[1] != 5) return false;
  if (a[2] != 6) return false;
  auto c = to_array<3>(a);
  if (c[0] != 4) return false;
  if (c[1] != 5) return false;
  if (c[2] != 6) return false;
  return true;
}();
} // namespace test
#endif
