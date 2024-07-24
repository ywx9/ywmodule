#pragma once

#include "none.hpp"

export namespace yw {

template<auto...Vs> struct Sequence;

namespace _ {
template<typename S, typename T> struct _to_sequence;
template<template<auto...> typename Tm, typename T, auto... Vs> struct _to_sequence<Tm<Vs...>, T> : _type<Sequence<T(Vs)...>> {};
template<template<typename, auto...> typename Tm, typename T, typename U, auto... Vs> struct _to_sequence<Tm<U, Vs...>, T> : _type<Sequence<T(Vs)...>> {};
template<template<auto...> typename Tm, auto... Vs> struct _to_sequence<Tm<Vs...>, None> : _type<Sequence<Vs...>> {};
template<template<typename, auto...> typename Tm, typename U, auto... Vs> struct _to_sequence<Tm<U, Vs...>, None> : _type<Sequence<Vs...>> {};
template<typename S, nat N> struct _indices_for : constant<false> {};
template<nat... Is, nat N> struct _indices_for<Sequence<Is...>, N> : constant<(lt(Is, N) && ...)> {};
template<nat I, nat N, auto F, nat... Vs> struct _make_sequence : _make_sequence<I + 1, N, F, Vs..., F(I)> {};
template<nat N, auto F, nat... Vs> struct _make_sequence<N, N, F, Vs...> : _type<Sequence<Vs...>> {};
template<nat I, nat N, typename S, nat... Is> struct _extracting_indices;
template<nat I, nat N, bool... Bs, nat... Is> struct _extracting_indices<I, N, Sequence<Bs...>, Is...>
  : select_type<select_value<I, Bs...>, _extracting_indices<I + 1, N, Sequence<Bs...>, Is..., I>, _extracting_indices<I + 1, N, Sequence<Bs...>, Is...>> {};
template<nat N, bool... B, nat... Is> struct _extracting_indices<N, N, Sequence<B...>, Is...> : _type<Sequence<Is...>> {};
template<typename T, typename S> struct _common_element;
template<typename T, nat... Is> struct _common_element<T, Sequence<Is...>> : _common_type<element_t<T, Is>...> {};
template<typename T, typename U, typename S> struct _tuple_for : constant<0> {};
template<typename T, typename U, nat... Is> struct _tuple_for<T, U, Sequence<Is...>>
  : constant<int((convertible_to<element_t<T, Is>, U> && ...)) + 2 * int((nt_convertible_to<element_t<T, Is>, U> && ...))> {};
}

template<typename S, typename T = None> using to_sequence = typename _::_to_sequence<S, T>::type;
template<typename S, typename T = None> concept sequence_of = variation_of<to_sequence<S, T>, Sequence<>>;
template<typename S, typename T> concept indices_for = _::_indices_for<to_sequence<S>, extent<T>>::value;
template<nat Begin, nat End, invocable<nat> auto Proj = pass> requires (Begin <= End) using make_sequence = typename _::_make_sequence<Begin, End, Proj>::type;
template<typename Tp> using make_indices_for = make_sequence<0, extent<Tp>>;
template<sequence_of<bool> Sq> using extracting_indices = typename _::_extracting_indices<0, extent<Sq>, Sq>::type;
template<typename T> using common_element = typename _::_common_element<T, make_indices_for<T>>::type;
template<typename T, typename U> concept tuple_for = requires { requires bool(_::_tuple_for<T, U, make_indices_for<T>>::value & 1); };
template<typename T, typename U> concept nt_tuple_for = tuple_for<T, U> && requires { requires bool(_::_tuple_for<T, U, make_indices_for<T>>::value & 2); };

template<auto... Vs> struct Sequence {
private:
  template<typename S> struct _append : _append<to_sequence<S, None>> {};
  template<auto... Ws> struct _append<Sequence<Ws...>> : _::_type<Sequence<Vs..., Ws...>> {};
  template<> struct _append<None> : _::_type<None> {};
  template<typename S> struct _extract : _extract<to_sequence<S, None>> {};
  template<nat... Is> struct _extract<Sequence<Is...>> : _::_type<Sequence<select_value<Is, Vs...>...>> {};
  template<> struct _extract<None> : _::_type<None> {};
public:

  /// number of values in the sequence
  static constexpr nat count = sizeof...(Vs);

  /// value at index `I`
  template<nat I> requires(lt(I, sizeof...(Vs)))
  static constexpr auto at = select_value<I, Vs...>;

  /// type at index `I`
  template<nat I> requires(lt(I, sizeof...(Vs)))
  using type_at = select_type<I, decltype(Vs)...>;

  /// appends values of another sequence
  template<sequence_of Sq>
  using append = typename _append<to_sequence<Sq>>::type;

  /// extracts a subsequence
  template<indices_for<Sequence> Ind>
  using extract = typename _extract<to_sequence<Ind, nat>>::type;

  /// extracts the first `N` values
  template<nat N> requires(N <= sizeof...(Vs))
  using fore = extract<make_sequence<0, N>>;

  /// extracts the last `N` values
  template<nat N> requires(N <= sizeof...(Vs))
  using back = extract<make_sequence<sizeof...(Vs), sizeof...(Vs) - N>>;

  /// inserts another sequence at index `I`
  template<nat I, sequence_of Sq> requires(I < sizeof...(Vs))
  using insert = typename fore<I>::template append<Sq>::template append<back<sizeof...(Vs) - I>>;

  /// expands the sequence into a template
  template<template<auto...> typename Tm>
  using expand = Tm<Vs...>;

  /// for `yw::get`
  template<nat I> requires(I < sizeof...(Vs))
  constexpr const auto&& get() const noexcept { return mv(at<I>); }
};

namespace _ {
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
}

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

} // namespace yw

namespace std {

template<auto... Vs>
struct tuple_size<yw::Sequence<Vs...>>
  : integral_constant<size_t, yw::Sequence<Vs...>::count> {};

template<size_t I, auto... Vs>
struct tuple_element<I, yw::Sequence<Vs...>>
  : type_identity<yw::Sequence<Vs...>::template type_at<I>> {};

} // namespace std
