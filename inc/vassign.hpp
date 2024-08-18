/// \file vassign.hpp
/// \brief defines `function object vassign` and concepts

#pragma once

#include "sequence.hpp"



namespace yw::_ {

template<typename Sq, typename Lt, typename Rt>
struct _vassignable : Sequence<false, false> {};

template<nat... Is, typename Lt, typename Rt>
struct _vassignable<Sequence<Is...>, Lt, Rt>
  : Sequence<(assignable<element_t<Lt, Is>, element_t<Rt, Is>> && ...),
             (nt_assignable<element_t<Lt, Is>, element_t<Rt, Is>> && ...)> {};

} ////////////////////////////////////////////////////////////////////////////// namespace yw::_


export namespace yw {


/// checks if each element of `Rt` can be assigned to the corresponding element of `Lt`
template<typename Lt, typename Rt> concept vassignable = requires {
  requires same_extent<Lt, Rt>;
  requires _::_vassignable<make_indices_for<Lt>, Lt, Rt>::template at<0>;
};


/// checks if each element of `Rt` can be assigned to the corresponding element of `Lt` without throwing exceptions
template<typename Lt, typename Rt> concept nt_vassignable = requires {
  requires vassignable<Lt, Rt>;
  requires _::_vassignable<make_indices_for<Lt>, Lt, Rt>::template at<1>;
};


/// helper for `vassign`
struct Vassign {

  /// assigns each element of `Rt` to the corresponding element of `Lt`
  template<typename Lt, typename Rt> requires vassignable<Lt, Rt>
  constexpr void operator()(Lt&& L, Rt&& R) const
    noexcept(nt_vassignable<Lt, Rt>) {
    [&]<nat... Is>(Sequence<Is...>) {
      ((yw::get<Is>(fwd<Lt>(L)) = yw::get<Is>(fwd<Rt>(R))), ...);
    }(make_indices_for<Lt>{});
  }
};


/// function object to assign each element of `Rt` to the corresponding element of `Lt`
/// \note `void vassign(Lt&& l, Rt&& r) noexcept(nt_vassignable<Lt, Rt>) requires vassignable<Lt, Rt>`
inline constexpr Vassign vassign;


} ////////////////////////////////////////////////////////////////////////////// namespace yw
