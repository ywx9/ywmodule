/// \file get.hpp
/// \brief defines `function object get` and other utilities

#pragma once

#include "core.hpp"

namespace yw::_::_get {

template<nat I> void get() = delete;

template<nat I, typename T> inline constexpr auto pattern = []() -> int {
  if constexpr (is_bounded_array<remove_ref<T>>)
    return I < std::extent_v<remove_ref<T>> ? 16 : 8;
  else if constexpr (requires { get<I>(declval<T>()); })
    return 1 | noexcept(get<I>(declval<T>())) * 16;
  else if constexpr (requires { declval<T>().template get<I>(); })
    return 2 | noexcept(declval<T>().template get<I>()) * 16;
  else return I == 0 ? 20 : 8;
}();

template<nat I, typename T, nat P = pattern<I, T>> requires(P != 8)
constexpr decltype(auto) call(T&& A) noexcept(bool(P & 16)) {
  if constexpr ((P & 7) == 0) return static_cast<T&&>(A)[I];
  else if constexpr ((P & 7) == 1) return get<I>(static_cast<T&&>(A));
  else if constexpr ((P & 7) == 2) return static_cast<T&&>(A).template get<I>();
  else if constexpr ((P & 7) == 4) return static_cast<T&&>(A);
}

template<typename T, nat I = (pattern<0, T> & 15)> struct extent : std::tuple_size<T> {};
template<typename T> struct extent<T, 0> : std::extent<remove_ref<T>> {};
template<typename T> struct extent<T, 4> : constant<1> {};

} ////////////////////////////////////////////////////////////////////////////// namespace yw::_::_get


export namespace yw {


/// number of elements in tuple-like object
template<typename T> inline constexpr nat extent =
  _::_get::extent<remove_ref<T>>::value;


/// checks if the extents of `T` and `Ts...` are the same
template<typename T, typename... Ts>
concept same_extent = requires {
  requires ((extent<T> == extent<Ts>) && ...);
};


/// checks if `T` is a tuple-like object
template<typename T> concept tuple = requires {
  requires (_::_get::pattern<0, T> & 15) < 3;
};


/// checks if `get<I>(A)` is well-formed
template<typename T, nat I> concept gettable = requires {
  _::_get::call<I>(declval<T>());
};


/// checks if `get<I>(A)` is well-formed and noexcept
template<typename T, nat I> concept nt_gettable = requires {
  requires gettable<T, I>;
  { _::_get::call<I>(declval<T>()) } noexcept;
};


/// type of the `I`-th element of tuple-like object
template<typename T, nat I> requires gettable<T, I>
using element_t = decltype(_::_get::call<I>(declval<T>()));


/// obtains the element of tuple-like object
template<nat I> inline constexpr auto get =
[]<typename T>(T&& Ref) noexcept(nt_gettable<T, I>)
  -> element_t<T, I> { return _::_get::call<I>(fwd<T>(Ref)); };


} ////////////////////////////////////////////////////////////////////////////// namespace yw
