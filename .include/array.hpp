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

  /// base array
  T array[N] = {{}};

  /// number of elements in the array
  static constexpr nat COUNT = N;

  /// type of the elements in the array
  using value_type = T;

  /// assignment operator which copies the elements from the array `a`
  constexpr Array& operator=(const T (&a)[N]) {
    for (nat i{}; i < N; ++i) array[i] = a[i];
    return *this;
  }

  /// conversion operator to lvalue reference to the base array
  constexpr operator add_lvref<T[N]>() noexcept { return array; }

  /// conversion operator to lvalue reference to the base array (const)
  constexpr operator add_lvref<const T[N]>() const noexcept { return array; }

  /// index operator to access the element at index `I`
  constexpr T& operator[](const nat I) { return array[I]; }

  /// index operator to access the element at index `I` (const)
  constexpr const T& operator[](const nat I) const { return array[I]; }

  /// returns the number of elements in the array
  constexpr nat size() const noexcept { return N; }

  /// checks if the array is empty
  constexpr bool empty() const noexcept { return false; }

  /// returns a pointer to the base array
  constexpr T* data() noexcept { return array; }

  /// returns a pointer to the base array (const)
  constexpr const T* data() const noexcept { return array; }

  /// returns an iterator to the first element in the array
  constexpr T* begin() noexcept { return array; }

  /// returns an iterator to the first element in the array (const)
  constexpr const T* begin() const noexcept { return array; }

  /// returns an iterator to the end of the array
  constexpr T* end() noexcept { return array + N; }

  /// returns an iterator to the end of the array (const)
  constexpr const T* end() const noexcept { return array + N; }

  /// returns a reference to the first element in the array
  constexpr T& front() noexcept { return *array; }

  /// returns a reference to the first element in the array (const)
  constexpr const T& front() const noexcept { return *array; }

  /// returns a reference to the last element in the array
  constexpr T& back() noexcept { return array[N - 1]; }

  /// returns a reference to the last element in the array (const)
  constexpr const T& back() const noexcept { return array[N - 1]; }

  /// returns a reference to the element at index `I`
  template<nat I> requires(I < N)
  constexpr T& get() & noexcept { return array[I]; }

  /// returns a reference to the element at index `I` (rvalue)
  template<nat I> requires(I < N)
  constexpr T&& get() && noexcept { return mv(array[I]); }

  /// returns a reference to the element at index `I` (const)
  template<nat I> requires(I < N)
  constexpr const T& get() const& noexcept { return array[I]; }

  /// returns a reference to the element at index `I` (const rvalue)
  template<nat I> requires(I < N)
  constexpr const T&& get() const&& noexcept { return mv(array[I]); }
};


/// class to represent a null array
template<typename T> class Array<T, 0> {
public:

  /// number of elements in the array
  static constexpr nat COUNT = 0;

  /// type of the elements in the array
  using value_type = T;

  /// returns number of elements in the array
  constexpr nat size() const noexcept { return 0; }

  /// checks if the array is empty
  constexpr bool empty() const noexcept { return true; }

  /// returns a pointer to the base array
  constexpr np_t data() noexcept { return nullptr; }

  /// returns a pointer to the base array (const)
  constexpr const np_t data() const noexcept { return nullptr; }

  /// returns an iterator to the first element in the array
  constexpr np_t begin() noexcept { return nullptr; }

  /// returns an iterator to the first element in the array (const)
  constexpr const np_t begin() const noexcept { return nullptr; }

  /// returns an iterator to the end of the array
  constexpr np_t end() noexcept { return nullptr; }

  /// returns an iterator to the end of the array (const)
  constexpr const np_t end() const noexcept { return nullptr; }
};


/// class to represent a dynamic array
template<typename T> class Array<T, npos> : public std::vector<T> {
public:

  /// default constructor
  constexpr Array() noexcept = default;

  /// constructor with the rvalue reference to `std::vector`
  constexpr Array(std::vector<T>&& v) : std::vector<T>(mv(v)) {}

  /// constructor with the number of elements
  constexpr explicit Array(nat n) : std::vector<T>(n) {}

  /// constructor with the number of elements and the value to initialize
  constexpr Array(nat n, const T& v) : std::vector<T>(n, v) {}

  /// constructor with the iterators
  template<iterator_for<T> It>
  constexpr Array(It i, It s) : std::vector<T>(i, s) {}

  /// constructor with the iterators and the sentinel
  template<iterator_for<T> It, sentinel_for<It> Se>
  requires (!same_as<It, Se>) constexpr Array(It i, Se s)
    : std::vector<T>(std::common_iterator<It, Se>(i),
                     std::common_iterator<It, Se>(s)) {}

  /// constructor with the range
  template<range_for<T> Rg> constexpr Array(Rg&& r)
    : std::vector<T>(yw::begin(r), yw::end(r)) {}

  /// constructor with the initializer list
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
