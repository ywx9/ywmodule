/// \file list.hpp
/// \brief defines `struct List`


#pragma once

#include "typepack.hpp"
#include "vassign.hpp"


export namespace yw {


template<typename... Ts> struct List;


/// alias for base class of `List`
template<typename... Ts> using ListBase =
  Typepack<Ts...>::template fore<sizeof...(Ts) - 1>::template expand<List>;


/// struct to represent a standard tuple-like object
template<typename... Ts>
struct List : ListBase<Ts...> {

  /// number of elements
  static constexpr nat count = sizeof...(Ts);

  /// last element type
  using last_type = select_type<count - 1, Ts...>;

  /// last element
  select_type<count - 1, Ts...> last;

  /// returns the `I`-th element of `this`
  template<nat I> constexpr auto get() & noexcept
    -> select_type<I, Ts...>& {
    if constexpr (I == count - 1) return last;
    else return ListBase<Ts...>::template get<I>();
  }

  /// returns the `I`-th element of `this` (const& version)
  template<nat I> constexpr auto get() const& noexcept
    -> add_const<select_type<I, Ts...>&> {
    if constexpr (I == count - 1) return last;
    else return ListBase<Ts...>::template get<I>();
  }

  /// returns the `I`-th element of `this` (&& version)
  template<nat I> constexpr auto get() && noexcept
    -> select_type<I, Ts...>&& {
    if constexpr (I == count - 1) return fwd<last_type>(last);
    else return static_cast<ListBase<Ts...>&&>(*this).template get<I>();
    // else return fwd<select_type<I, Ts...>>(ListBase<Ts...>::template get<I>());
  }

  /// returns the `I`-th element of `this` (const&& version)
  template<nat I> constexpr auto get() const&& noexcept
    -> add_const<select_type<I, Ts...>&&> {
    if constexpr (I == count - 1) return fwd<add_const<last_type>>(last);
    else return static_cast<const ListBase<Ts...>&&>(*this).template get<I>();
  }

  /// assigns corresponding elements of `Rt` to `this`
  template<typename Rt> constexpr void operator=(Rt&& R) &
    noexcept(nt_vassignable<List&, Rt>) requires vassignable<List&, Rt> {
    vassign(*this, fwd<Rt>(R));
  }

  /// assigns corresponding elements of `Rt` to `this` (const& version)
  template<typename Rt> constexpr void operator=(Rt&& R) const&
    noexcept(nt_vassignable<const List&, Rt>)
    requires vassignable<const List&, Rt> {
    vassign(*this, fwd<Rt>(R));
  }

  /// assigns corresponding elements of `Rt` to `this` (&& version)
  template<typename Rt> constexpr void operator=(Rt&& R) &&
    noexcept(nt_vassignable<List&&, Rt>) requires vassignable<List&&, Rt> {
    vassign(mv(*this), fwd<Rt>(R));
  }

  /// assigns corresponding elements of `Rt` to `this` (const&& version)
  template<typename Rt> constexpr void operator=(Rt&& R) const&&
    noexcept(nt_vassignable<const List&&, Rt>)
    requires vassignable<const List&&, Rt> {
    vassign(mv(*this), fwd<Rt>(R));
  }
};


/// struct to represent a standard tuple-like object
template<typename T1, typename T2, typename T3>
struct List<T1, T2, T3> : List<T1, T2> {

  /// number of elements
  static constexpr nat count = 3;

  /// third element type
  using third_type = T3;

  /// third element
  third_type third;

  /// returns the `I`-th element of `this`
  template<nat I> constexpr auto get() & noexcept
    -> select_type<I, T1, T2, T3>& {
    if constexpr (I == 2) return third;
    else return static_cast<List<T1, T2>&>(*this).template get<I>();
  }
  template<nat I> constexpr auto get() const& noexcept
    -> add_const<select_type<I, T1, T2, T3>&> {
    if constexpr (I == 2) return third;
    else return static_cast<const List<T1, T2>&>(*this).template get<I>();
  }
  template<nat I> constexpr auto get() && noexcept
    -> select_type<I, T1, T2, T3>&& {
    if constexpr (I == 2) return fwd<T3>(third);
    else return static_cast<List<T1, T2>&&>(*this).template get<I>();
  }
  template<nat I> constexpr auto get() const&& noexcept
    -> add_const<select_type<I, T1, T2, T3>&&> {
    if constexpr (I == 2) return fwd<add_const<T3>>(third);
    else return static_cast<const List<T1, T2>&&>(*this).template get<I>();
  }
};

template<typename T1, typename T2> struct List<T1, T2> : public List<T1> {
  using base = List<T1>;
  static constexpr nat count = 2;
  using second_type = T2;
  second_type second;

  template<nat I> constexpr auto get() & noexcept -> select_type<I, T1, T2>& {
    if constexpr (I == 1) return second;
    else return base::first;
  }
  template<nat I> constexpr auto get() const& noexcept -> add_const<select_type<I, T1, T2>&> {
    if constexpr (I == 1) return second;
    else return base::first;
  }
  template<nat I> constexpr auto get() && noexcept -> select_type<I, T1, T2>&& {
    if constexpr (I == 1) return fwd<T2>(second);
    else return fwd<T1>(base::first);
  }
  template<nat I> constexpr auto get() const&& noexcept -> add_const<select_type<I, T1, T2>&&> {
    if constexpr (I == 1) return fwd<add_const<T2>>(second);
    else return fwd<add_const<T1>>(base::first);
  }
};

template<typename T> struct List<T> {
  static constexpr nat count = 1;
  using first_type = T;
  first_type first;
  template<nat I> requires(I == 0) constexpr T& get() & noexcept { return first; }
  template<nat I> requires(I == 0) constexpr add_const<T&> get() const& noexcept { return first; }
  template<nat I> requires(I == 0) constexpr T&& get() && noexcept { return fwd<T>(first); }
  template<nat I> requires(I == 0) constexpr add_const<T&&> get() const&& noexcept { return fwd<add_const<T>>(first); }
};

template<> struct List<> {
private:
  template<typename T, typename U, typename V> struct _from_typepack {};
  template<typename... Ts, typename U, template<typename...> typename Tm, typename... Vs>
  struct _from_typepack<Typepack<Ts...>, U, Tm<Vs...>> : _::_type<List<copy_cvref<Tm<Ts>, U>...>> {};
  template<typename... Ts, typename U, template<typename, auto...> typename Tm, typename V, auto... Vs>
  struct _from_typepack<Typepack<Ts...>, U, Tm<V, Vs...>> : _::_type<List<copy_cvref<Tm<Ts, Vs...>, U>...>> {};
  template<typename... Ts, typename U, typename V> struct _from_typepack<Typepack<Ts...>, U, V> : _::_type<List<copy_cvref<Ts, U>...>> {};
public:
  static constexpr nat count = 0;
  template<typename... Ts> static constexpr auto asref(Ts&&... Args) noexcept { return List<Ts&&...>{fwd<Ts>(Args)...}; }
  template<specialization_of<Typepack> Tp, typename Qualifier = None> using from_typepack = _from_typepack<Tp, Qualifier, remove_cvref<Qualifier>>::type;
};

template<typename... Ts> List(Ts...) -> List<Ts...>;

namespace _ {

template<nat I, typename F, typename... Ts>
constexpr decltype(auto) _vapply_i(F&& f, Ts&&... ts)
  noexcept(nt_invocable<F, element_t<Ts, I>...>)
  requires invocable<F, element_t<Ts, I>...>
{ return invoke(fwd<F>(f), get<I>(fwd<Ts>(ts))...); }

template<nat... Is, typename F, typename... Ts>
constexpr decltype(auto) _vapply_is(Sequence<Is...>, F&& f, Ts&&... ts)
  ywlib_wrapper(List{_vapply_i<Is>(fwd<F>(f), fwd<Ts>(ts)...)...});

template<typename F, typename T, typename... Ts>
requires same_extent<T, Ts...>
constexpr decltype(auto) _vapply(F&& f, T&& t, Ts&&... ts)
  ywlib_wrapper(_vapply_is(make_indices_for<T>(), fwd<F>(f), fwd<T>(t), fwd<Ts>(ts)...));

template<typename F, typename T, typename... Ts>
requires (!same_extent<T, Ts...> && tuple<T> && (tuple<Ts> && ...))
void _vapply(F&& f, T&& t, Ts&&... ts) = delete;

template<typename F, typename... Ts>
requires (!(tuple<Ts> || ...))
void _vapply(F&& f, Ts&&... ts) = delete;

template<nat I, nat N, nat... Is, nat... Js, typename F, typename... Ts>
constexpr decltype(auto) _vapply_b(Sequence<Is...>, Sequence<Js...>, F&& f, Ts&&... ts)
  ywlib_wrapper(_vapply(fwd<F>(f), select_parameter<Is>(fwd<Ts>(ts)...)...,
    projector(select_parameter<I>(fwd<Ts>(ts)...), make_sequence<0, N>{}),
    select_parameter<Js>(fwd<Ts>(ts)...)...));

template<nat I, nat J, typename F, typename... Ts>
constexpr decltype(auto) _vapply_a(F&& f, Ts&&... ts)
  ywlib_wrapper(_vapply_b<I, extent<select_type<J, Ts...>>>(
    make_sequence<0, I>{}, make_sequence<I + 1, sizeof...(Ts)>{}, fwd<F>(f), fwd<Ts>(ts)...));

template<typename F, typename... Ts>
requires (!(tuple<Ts> && ...) && (tuple<Ts> || ...))
constexpr decltype(auto) _vapply(F&& f, Ts&&... ts)
  ywlib_wrapper(_vapply_a<inspects<!tuple<Ts>...>,
    inspects<tuple<Ts>...>>(fwd<F>(f), fwd<Ts>(ts)...));
}

template<typename T, typename... Ts> concept vapplyable =
  requires(T&& t, Ts&&... ts) { { _::_vapply(fwd<T>(t), fwd<Ts>(ts)...) }; };

template<typename T, typename... Ts> concept nt_vapplyable = requires(T&& t, Ts&&... ts) {
  requires vapplyable<T, Ts...>;
  { _::_vapply(fwd<T>(t), fwd<Ts>(ts)...) } noexcept; };

template<typename T, typename... Ts> using vapply_result =
  decltype(_::_vapply(declval<T>(), declval<Ts>()...));

inline constexpr auto vapply = []<typename F, typename... Ts>(F&& f, Ts&&... ts)
  noexcept(nt_vapplyable<F, Ts...>) requires vapplyable<F, Ts...>
{ return _::_vapply(fwd<F>(f), fwd<Ts>(ts)...); };

// template<typename Lt, typename Rt> concept vassignable =
//   tuple<Lt> && vapplyable<decltype(assign), Lt, Rt>;

// template<typename Lt, typename Rt> concept nt_vassignable =
//   tuple<Lt> && nt_vapplyable<decltype(assign), Lt, Rt>;

// inline constexpr auto vassign = []<typename Lt, typename Rt>(Lt&& L, Rt&& R)
//   noexcept(nt_vassignable<Lt, Rt>) requires vassignable<Lt, Rt>
// { return vapply(assign, fwd<Lt>(L), fwd<Rt>(R)); };

} // namespace yw

namespace std {

template<typename... Ts> struct tuple_size<yw::List<Ts...>> : integral_constant<size_t, sizeof...(Ts)> {};
template<size_t I, typename... Ts> struct tuple_element<I, yw::List<Ts...>> { using type = yw::select_type<I, Ts...>; };

} // namespace std
