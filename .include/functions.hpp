#pragma once

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

} // namespace yw
