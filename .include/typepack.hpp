#pragma once

#include "sequence.hpp"

export namespace yw {

template<typename... Ts> struct Typepack;

namespace _ {
template<typename T, typename S> struct _to_typepack;
template<typename T, nat... Is> struct _to_typepack<T, Sequence<Is...>> : _type<Typepack<element_t<T, Is>...>> {};
}

template<typename T> using to_typepack = typename _::_to_typepack<T, make_indices_for<T>>::type;

template<typename... Ts> struct Typepack {
private:
  template<typename T, typename U> struct t_append {};
  template<typename... Us, typename... Vs> struct t_append<Typepack<Us...>, Typepack<Vs...>> : _::_type<Typepack<Us..., Vs...>> {};
  template<typename T, typename S> struct t_extract {};
  template<typename... Us, nat... Is> struct t_extract<Typepack<Us...>, Sequence<Is...>> : _::_type<Typepack<select_type<Is, Us...>...>> {};
public:

  /// number of types in the typepack
  /// \note the word "count" cannot be used for coding `Typepack` because of a bug in MSVC (2024.07.24)
  static constexpr nat8 count = sizeof...(Ts);

  using common = common_type<Ts...>;

  template<nat I> requires(I < (sizeof...(Ts)))
  using at = select_type<I, Ts...>;

  template<tuple Tp>
  using append = typename t_append<Typepack, to_typepack<Tp>>::type;

  template<indices_for<Typepack> Sq>
  using extract = typename t_extract<Typepack, to_sequence<Sq, nat>>::type;

  template<nat N> requires(N <= (sizeof...(Ts)))
  using fore = extract<make_sequence<0, N>>;

  template<nat N> requires(N <= (sizeof...(Ts)))
  using back = extract<make_sequence<(sizeof...(Ts)), (sizeof...(Ts)) - N>>;

  template<nat I, tuple T> requires(I < (sizeof...(Ts)))
  using insert = typename fore<I>::template append<T>::template append<back<(sizeof...(Ts)) - I>>;

  template<template<typename...> typename Tm>
  using expand = Tm<Ts...>;

  template<nat I> requires(I < (sizeof...(Ts)))
  constexpr at<I> get() const noexcept;
};

} // namespace yw

namespace std {

template<typename... Ts> struct tuple_size<yw::Typepack<Ts...>> : integral_constant<size_t, sizeof...(Ts)> {};
template<size_t I, typename... Ts> struct tuple_element<I, yw::Typepack<Ts...>> { using type = typename yw::Typepack<Ts...>::template at<I>; };

} // namespace std
