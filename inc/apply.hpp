/// \file apply.hpp
/// \brief defines `apply` and `build`

#pragma once

#include "sequence.hpp"

namespace yw::_ {

template<typename F, typename... Ts> requires (!(tuple<Ts> || ...))
constexpr decltype(auto) _apply(F&& f, Ts&&... ts) noexcept(nt_invocable<F, Ts...>)
  requires invocable<F, Ts...> { return invoke(fwd<F>(f), fwd<Ts>(ts)...); }

template<nat I, nat... Is, nat... Js, nat... Ks, typename F, typename... Ts>
constexpr decltype(auto) _apply_b(Sequence<Is...>, Sequence<Js...>, Sequence<Ks...>, F&& f, Ts&&... ts)
  ywlib_wrapper(_apply(fwd<F>(f), select_parameter<Is>(fwd<Ts>(ts)...)...,
    get<Js>(fwd<select_type<I, Ts...>>(select_parameter<I>(fwd<Ts>(ts)...)))..., select_parameter<Ks>(fwd<Ts>(ts)...)...));

template<nat I, typename F, typename... Ts>
constexpr decltype(auto) _apply_a(F&& f, Ts&&... ts)
  ywlib_wrapper(_apply_b<I>(make_sequence<0, I>{}, make_indices_for<select_type<I, Ts...>>{},
    make_sequence<I + 1, sizeof...(Ts)>{}, fwd<F>(f), fwd<Ts>(ts)...));

template<typename F, typename... Ts> requires (tuple<Ts> || ...)
constexpr decltype(auto) _apply(F&& f, Ts&&... ts)
  ywlib_wrapper(_apply_a<inspects<tuple<Ts>...>>(fwd<F>(f), fwd<Ts>(ts)...));

} ////////////////////////////////////////////////////////////////////////////// namespace yw::_


export namespace yw {


/// checks if `apply(F, Ts...)` is well-formed
template<typename F, typename... Ts>
concept applyable = requires(F&& f, Ts&&... ts) {
  { _::_apply(fwd<F>(f), fwd<Ts>(ts)...) };
};


/// checks if `apply(F, Ts...)` is well-formed and noexcept
template<typename F, typename... Ts>
concept nt_applyable = requires(F&& f, Ts&&... ts) {
  applyable<F, Ts...>;
  { _::_apply(fwd<F>(f), fwd<Ts>(ts)...) } noexcept;
};


/// result type of `apply(F, Ts...)`
template<typename F, typename... Ts>
using apply_result = decltype(_::_apply(declval<F>(), declval<Ts>()...));


/// applies a function to arguments with tuples expanded
inline constexpr auto apply =
[]<typename F, typename... Ts>(F&& Func, Ts&&... Args)
  noexcept(nt_applyable<F, Ts...>) requires applyable<F, Ts...> {
  return _::_apply(fwd<F>(Func), fwd<Ts>(Args)...);
};


/// checks if `build<T>(Tp)` is well-formed
template<typename T, typename Tp>
concept buildable = applyable<decltype(construct<T>), Tp>;


/// checks if `build<T>(Tp)` is well-formed and noexcept
template<typename T, typename Tp>
concept nt_buildable = nt_applyable<decltype(construct<T>), Tp>;


/// constructs an object of type `T` from elements of `Tp`
template<typename T> inline constexpr auto build =
[]<typename Tp>(Tp&& Tuple)
  noexcept(nt_buildable<T, Tp>) requires buildable<T, Tp> {
  return apply(construct<T>, fwd<Tp>(Tuple));
};


} ////////////////////////////////////////////////////////////////////////////// namespace yw
