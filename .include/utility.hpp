#pragma once

#include "array.hpp"
#include "list.hpp"

export namespace yw {


/// helper for min/max functions
template<bool Max> struct t_max {

  constexpr None operator()() const noexcept { return {}; }

  template<typename T> constexpr T operator()(T&& t) const noexcept { return t; }

  template<typename T0, typename T1, typename... Ts>
  constexpr auto operator()(T0&& t0, T1&& t1, Ts&&... ts) const -> common_type<T0, T1, Ts...> {
    return (*this)(call<common_type<T0, T1, Ts...>>(t0, t1), ts...);
  }

  template<typename F, typename T, typename... Ts>
  constexpr auto operator()(F&& f, T&& t, Ts&&... ts) const
    -> common_type<invoke_result<F&, T>, invoke_result<F&, Ts>...> {
    return (*this)(invoke(f, t), invoke(f, ts)...);
  }

private:

  template<typename R> static constexpr auto call(auto&& t, auto&& u) -> R {
    if constexpr (Max) return t < u ? u : t;
    else return u < t ? u : t;
  }
};

/// function object to get the maximum value
inline constexpr t_max<1> max;

/// function object to get the minimum value
inline constexpr t_max<0> min;


/// helper for mini/maxi functions
template<bool Max> struct t_maxi {

  constexpr List<nat, None> operator()() const noexcept { return List<nat, None>{}; }

  template<typename T> constexpr List<nat, remove_ref<T>>
  operator()(T&& t) const noexcept { return {0, fwd<T>(t)}; }

  template<typename T0, typename T1, typename... Ts>
  constexpr List<nat, remove_ref<common_type<T0, T1, Ts...>>>
  operator()(T0&& t0, T1&& t1, Ts&&... ts) const {
    return call<1>(List<>::asref(t0, t1, ts...),
      List<nat, remove_ref<common_type<T0, T1, Ts...>>>{0, fwd<T0>(t0)});
  }

private:

  template<nat I, typename Tp, typename R>
  static constexpr auto call(Tp&& L, R M) {
    if constexpr (I == extent<Tp>) return mv(M);
    else {
      if constexpr (Max) {
        if (M.second < get<I>(L)) return call<I + 1>(fwd<Tp>(L), R{I, get<I>(L)});
        else return call<I + 1>(fwd<Tp>(L), mv(M));
      } else {
        if (M.second > get<I>(L)) return call<I + 1>(fwd<Tp>(L), R{I, get<I>(L)});
        else return call<I + 1>(fwd<Tp>(L), mv(M));
      }
    }
  }
};

/// function object to get the index and value of the maximum value
inline constexpr t_maxi<1> maxi;

/// function object to get the index and value of the minimum value
inline constexpr t_maxi<0> mini;


/// bitwide left rotation
inline constexpr auto rotl =
[]<std::unsigned_integral T>(T x, nat s) noexcept -> T {
  return static_cast<T>((x << s) | (x >> (cev(sizeof(T) * 8) - s)));
};

/// bitwide right rotation
inline constexpr auto rotr =
[]<std::unsigned_integral T>(T x, nat s) noexcept -> T {
  return static_cast<T>((x >> s) | (x << (cev(sizeof(T) * 8) - s)));
};


/// bitwise reverse
inline constexpr auto reverse_bits =
[]<std::unsigned_integral T>(T x) noexcept -> T {
  // std::cout << typeid(T).name() << std::endl;
  // std::cout << sizeof(T) << std::endl;
  // std::cout << x << std::endl;
  if constexpr (is_bool<T>) return x;
  else if constexpr (sizeof(T) == 1) {
    x = ((x & 0x55) << 1) | ((x & 0xAA) >> 1);
    x = ((x & 0x33) << 2) | ((x & 0xCC) >> 2);
    return (x << 4) | (x >> 4);
  } else if constexpr (sizeof(T) == 2) {
    x = ((x & 0x5555) << 1) | ((x & 0xAAAA) >> 1);
    x = ((x & 0x3333) << 2) | ((x & 0xCCCC) >> 2);
    x = ((x & 0x0F0F) << 4) | ((x & 0xF0F0) >> 4);
    return (x << 8) | (x >> 8);
  } else if constexpr (sizeof(T) == 4) {
    x = ((x & 0x55555555) << 1) | ((x & 0xAAAAAAAA) >> 1);
    x = ((x & 0x33333333) << 2) | ((x & 0xCCCCCCCC) >> 2);
    x = ((x & 0x0F0F0F0F) << 4) | ((x & 0xF0F0F0F0) >> 4);
    x = ((x & 0x00FF00FF) << 8) | ((x & 0xFF00FF00) >> 8);
    return (x << 16) | (x >> 16);
  } else {
    x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
    x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);
    x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);
    x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);
    x = ((x & 0x0000FFFF0000FFFF) << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);
    // return (x << 32) | (x >> 32);
    x = (x << 32) | (x >> 32);
    // std::cout << std::format("{:064b}", x) << std::endl;
    return x;
  }
};


/// reverses the endianness of an integer
inline constexpr auto reverse_endian =
[]<trivial T>(T&& a) noexcept -> T {
  if constexpr (sizeof(T) == 1) return a;
  else if constexpr (sizeof(T) == 2) return (a >> 8) | (a << 8);
  else if constexpr (sizeof(T) == 4)
    return (a >> 24) | ((a >> 8) & 0x0000FF00) | ((a << 8) & 0x00FF0000) | (a << 24);
  else if constexpr (sizeof(T) == 8)
    return (a >> 56) | ((a >> 40) & 0x000000000000FF00) | ((a >> 24) & 0x0000000000FF0000) |
                       ((a >>  8) & 0x00000000FF000000) | ((a <<  8) & 0x000000FF00000000) |
                       ((a << 24) & 0x0000FF0000000000) | ((a << 40) & 0x00FF000000000000) | (a << 56);
  else {
    auto b = bitcast<Array<nat1, sizeof(T)>>(a);
    for (nat i{}; i < sizeof(T) / 2; ++i) swap(b[i], b[sizeof(T) - i - 1]);
    return bitcast<T>(b);
  }
};


} // namespace yw
