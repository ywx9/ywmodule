#pragma once

#include "sequence.hpp"

export namespace yw {

template<typename T, typename Pj = Pass, sequence_of<nat> Sq = make_indices_for<T>> struct Projector {
  static_assert(to_sequence<Sq, nat>::count > 0);
  static_assert(!tuple<T> || indices_for<Sq, T>);
  using sequence = to_sequence<Sq, nat>;
  static constexpr nat count = sequence::count;
  add_fwref<T> ref;
  remove_ref<Pj> proj = {};
  template<typename U = T> constexpr Projector(U&& Ref, Pj p, Sq)
    noexcept(nt_constructible<Pj, Pj>) : ref(fwd<U>(Ref)), proj(mv(p)) {}
  template<typename U = T> constexpr Projector(U&& Ref, Pj p)
    noexcept(nt_constructible<Pj, Pj>) : ref(fwd<U>(Ref)), proj(mv(p)) {}
  template<typename U = T> constexpr Projector(U&& Ref, Sq) noexcept : ref(fwd<U>(Ref)) {}
  template<typename U = T> constexpr Projector(U&& Ref) noexcept : ref(fwd<U>(Ref)) {}
  template<nat I> requires(I < count && tuple<T>) constexpr decltype(auto) get() const
    noexcept(nt_gettable<T, sequence::template at<I>> && nt_invocable<Pj, element_t<T, sequence::template at<I>>>)
    requires gettable<T, sequence::template at<I>> && invocable<Pj, element_t<T, sequence::template at<I>>>
  { return invoke(proj, yw::get<sequence::template at<I>>(ref)); }
  template<nat I> requires(I < count && !tuple<T>) constexpr decltype(auto) get() const
    noexcept(nt_invocable<Pj, T>) requires invocable<Pj, T> { return invoke(proj, ref); }
};

template<typename T, typename Pj, sequence_of<nat> Sq> Projector(T&&, Pj, Sq) -> Projector<T, Pj, Sq>;
template<typename T, typename Pj> requires(!sequence_of<Pj, nat>) Projector(T&&, Pj) -> Projector<T, Pj>;
template<typename T, sequence_of<nat> Sq> Projector(T&&, Sq) -> Projector<T, Pass, Sq>;
template<typename T> Projector(T&&) -> Projector<T>;

} // namespace yw

namespace std {

template<typename T, typename Pj, yw::sequence_of<nat> Sq>
struct tuple_size<yw::Projector<T, Pj, Sq>>
  : integral_constant<size_t, yw::Projector<T, Pj, Sq>::count> {};

template<size_t I, typename T, typename Pj, yw::sequence_of<nat> Sq>
struct tuple_element<I, yw::Projector<T, Pj, Sq>> {
  using type = decltype(declval<yw::Projector<T, Pj, Sq>>().template get<I>());
};

} // namespace std
