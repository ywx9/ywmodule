#pragma once

#include "none.hpp"

#define ywlib_wrapper(...) noexcept(noexcept(__VA_ARGS__)) \
  requires requires { __VA_ARGS__; } { return __VA_ARGS__; }

export namespace yw {

using cat1 = char;
using cat2 = wchar_t;
using uct1 = char8_t;
using uct2 = char16_t;
using uct4 = char32_t;
using int1 = signed char;
using int2 = signed short;
using int4 = signed int;
using int8 = signed long long;
using nat1 = unsigned char;
using nat2 = unsigned short;
using nat4 = unsigned int;
using nat8 = unsigned long long;
using fat4 = float;
using fat8 = double;

using sl_t = long;
using ul_t = unsigned long;
using ld_t = long double;
using np_t = decltype(nullptr);
using po_t = std::partial_ordering;
using wo_t = std::weak_ordering;
using so_t = std::strong_ordering;

using nat = nat8;
inline constexpr nat npos = nat(-1);
inline constexpr po_t unordered = std::partial_ordering::unordered;

consteval cat1 operator""_c1(nat8 n) { return static_cast<cat1>(n); }
consteval cat2 operator""_c2(nat8 n) { return static_cast<cat2>(n); }
consteval uct1 operator""_u1(nat8 n) { return static_cast<uct1>(n); }
consteval uct2 operator""_u2(nat8 n) { return static_cast<uct2>(n); }
consteval uct4 operator""_u4(nat8 n) { return static_cast<uct4>(n); }
consteval int1 operator""_i1(nat8 n) { return static_cast<int1>(n); }
consteval int2 operator""_i2(nat8 n) { return static_cast<int2>(n); }
consteval int4 operator""_i4(nat8 n) { return static_cast<int4>(n); }
consteval int8 operator""_i8(nat8 n) { return static_cast<int8>(n); }
consteval nat1 operator""_n1(nat8 n) { return static_cast<nat1>(n); }
consteval nat2 operator""_n2(nat8 n) { return static_cast<nat2>(n); }
consteval nat4 operator""_n4(nat8 n) { return static_cast<nat4>(n); }
consteval nat8 operator""_n8(nat8 n) { return static_cast<nat8>(n); }
consteval fat4 operator""_f4(nat8 n) { return static_cast<fat4>(n); }
consteval fat8 operator""_f8(nat8 n) { return static_cast<fat8>(n); }
consteval fat4 operator""_f4(ld_t n) { return static_cast<fat4>(n); }
consteval fat8 operator""_f8(ld_t n) { return static_cast<fat8>(n); }

/// Helper of `makenat`
template<typename T> struct MakeNat { using type = None; };
template<typename T> requires (sizeof(T) == 1) struct MakeNat<T> { using type = nat1; };
template<typename T> requires (sizeof(T) == 2) struct MakeNat<T> { using type = nat2; };
template<typename T> requires (sizeof(T) == 4) struct MakeNat<T> { using type = nat4; };
template<typename T> requires (sizeof(T) == 8) struct MakeNat<T> { using type = nat8; };

/// natural number type, whose size is the same as `T`
template<typename T> using makenat = typename MakeNat<T>::type;

template<typename T, typename... Ts> concept same_as = requires { requires (std::same_as<T, Ts> && ...); };
template<typename T, typename... Ts> concept included_in = requires { requires (std::same_as<T, Ts> || ...); };
template<typename T, typename... Ts> concept derived_from = requires { requires (std::derived_from<T, Ts> && ...); };
template<typename T, typename... Ts> concept castable_to = requires(T (&f)()) { ((static_cast<Ts>(f())), ...); };
template<typename T, typename... Ts> concept nt_castable_to = castable_to<T, Ts...> && requires(T (&f)() noexcept) { { ((static_cast<Ts>(f())), ...) } noexcept; };
template<typename T, typename... Ts> concept convertible_to = requires { requires (std::convertible_to<T, Ts> && ...); };
template<typename T, typename... Ts> concept nt_convertible_to = convertible_to<T, Ts...> && requires { requires (std::is_nothrow_convertible_v<T, Ts> && ...); };
template<typename T> concept numeric = nt_castable_to<T, int4, int8, nat4, nat8, fat4, fat8>;

template<auto V, typename T = decltype(V)> requires convertible_to<decltype(V), T> struct constant {
  using type = T;
  static constexpr type value = V;
  consteval operator type() const noexcept { return value; }
  consteval type operator()() const noexcept { return value; }
};

template<typename T> concept is_const = std::is_const_v<T>;
template<typename T> concept is_volatile = std::is_volatile_v<T>;
template<typename T> concept is_cv = is_const<T> && is_volatile<T>;
template<typename T> concept is_lvref = std::is_lvalue_reference_v<T>;
template<typename T> concept is_rvref = std::is_rvalue_reference_v<T>;
template<typename T> concept is_reference = is_lvref<T> || is_rvref<T>;
template<typename T> concept is_pointer = std::is_pointer_v<T>;
template<typename T> concept is_bounded_array = std::is_bounded_array_v<T>;
template<typename T> concept is_unbounded_array = std::is_unbounded_array_v<T>;
template<typename T> concept is_array = is_bounded_array<T> || is_unbounded_array<T>;
template<typename T> concept is_function = std::is_function_v<T>;

namespace _ {
template<typename T> struct _type { using type = T; };
template<typename T> struct _remove_all_pointers : _type<T> {};
template<is_pointer T> struct _remove_all_pointers<T> : _remove_all_pointers<std::remove_pointer<T>> {};
template<typename T> struct _member_type;
template<typename M, typename C> struct _member_type<M C::*> { using m = M; using c = C; };
template<typename T> struct _add_cv { using cv = const volatile T; using c = const T; using v = volatile T; };
template<typename T> struct _add_cv<T&> { using cv = const volatile T&; using c = const T&; using v = volatile T&; };
template<typename T> struct _add_cv<T&&> { using cv = const volatile T&&; using c = const T&&; using v = volatile T&&; };
template<typename T> struct _add_pointer : _type<T> {};
template<typename T> requires requires { typename std::void_t<T*>; } struct _add_pointer<T> : _type<T*> {};
template<typename T> requires requires { typename std::void_t<T*>; } struct _add_pointer<T&> : _type<T*&> {};
template<typename T> requires requires { typename std::void_t<T*>; } struct _add_pointer<T&&> : _type<T*&&> {};
template<typename T, nat N> struct _add_extent : _type<T> {};
template<typename T, nat N> requires requires { typename std::void_t<T[N]>; } struct _add_extent<T, N> : _type<T[N]> {};
template<typename T, nat N> requires requires { typename std::void_t<T[N]>; } struct _add_extent<T&, N> : _type<T(&)[N]> {};
template<typename T, nat N> requires requires { typename std::void_t<T[N]>; } struct _add_extent<T&&, N> : _type<T(&&)[N]> {};
template<typename T> requires requires { typename std::void_t<T[]>; } struct _add_extent<T, 0> : _type<T[]> {};
template<typename T> requires requires { typename std::void_t<T[]>; } struct _add_extent<T&, 0> : _type<T(&)[]> {};
template<typename T> requires requires { typename std::void_t<T[]>; } struct _add_extent<T&&, 0> : _type<T(&&)[]> {};
template<typename T, T I, typename... Ts> struct _select_type : _select_type<nat, I, Ts...> {};
template<bool B, typename Tt, typename Tf> struct _select_type<bool, B, Tt, Tf> : std::conditional<B, Tt, Tf> {};
template<nat I, typename T, typename... Ts> struct _select_type<nat, I, T, Ts...> : _select_type<nat, I - 1, Ts...> {};
template<typename T, typename... Ts> struct _select_type<nat, 0, T, Ts...> : _type<T> {};
}

template<typename T> using remove_cv = std::remove_cv_t<T>;
template<typename T> using remove_const = std::remove_const_t<T>;
template<typename T> using remove_volatile = std::remove_volatile_t<T>;
template<typename T> using remove_ref = std::remove_reference_t<T>;
template<typename T> using remove_cvref = std::remove_cvref_t<T>;
template<typename T> using remove_extent = std::remove_extent_t<T>;
template<typename T> using remove_all_extents = std::remove_all_extents_t<T>;
template<typename T> using remove_pointer = std::remove_pointer_t<T>;
template<typename T> using remove_all_pointers = typename _::_remove_all_pointers<T>::type;
template<typename T> concept is_memptr = is_pointer<T> && std::is_member_pointer_v<T>;

template<is_memptr Mp> using member_type = typename _::_member_type<remove_cv<Mp>>::m;
template<is_memptr Mp> using class_type = typename _::_member_type<remove_cv<Mp>>::c;
template<typename T> concept is_member_function_pointer = is_memptr<T> && is_function<member_type<T>>;
template<typename T> concept is_member_object_pointer = is_memptr<T> && !is_member_function_pointer<T>;

template<typename T> concept is_enum = std::is_enum_v<T>;
template<typename T> concept is_class = std::is_class_v<T>;
template<typename T> concept is_union = std::is_union_v<T>;
template<typename T> concept is_abstract = is_class<T> && std::is_abstract_v<T>;
template<typename T> concept is_aggregate = is_class<T> && std::is_aggregate_v<T>;
template<typename T> concept is_empty = is_class<T> && std::is_empty_v<T>;
template<typename T> concept is_final = is_class<T> && std::is_final_v<T>;
template<typename T> concept is_polymorphic = is_class<T> && std::is_polymorphic_v<T>;
template<typename T> concept has_virtual_destructor = is_class<T> && std::has_virtual_destructor_v<T>;
template<typename T> concept is_standard_layout = std::is_standard_layout_v<T>;
template<typename T> concept trivial = std::is_trivially_copyable_v<T>;

template<typename T> concept is_void = same_as<remove_cv<T>, void>;
template<typename T> concept is_bool = same_as<remove_cv<T>, bool>;
template<typename T> concept character = included_in<remove_cv<T>, cat1, cat2, uct1, uct2, uct4>;
template<typename T> concept integral = std::integral<remove_cv<T>>;
template<typename T> concept floating_point = std::floating_point<remove_cv<T>>;
template<typename T> concept arithmetic = integral<T> || floating_point<T>;

template<typename T> using add_cv = typename _::_add_cv<T>::cv;
template<typename T> using add_const = typename _::_add_cv<T>::c;
template<typename T> using add_volatile = typename _::_add_cv<T>::v;
template<typename T> using add_lvref = std::add_lvalue_reference_t<T>;
template<typename T> using add_rvref = std::add_rvalue_reference_t<remove_ref<T>>;
template<typename T> using add_fwref = std::add_rvalue_reference_t<T>;
template<typename T> using add_pointer = typename _::_add_pointer<T>::type;
template<typename T, nat N> using add_extent = typename _::_add_extent<T, N>::type;

inline consteval auto cev(auto a) noexcept { return a; }
inline consteval auto arraysize(const is_bounded_array auto& Array) noexcept { return std::extent_v<remove_ref<decltype(Array)>>; }
template<typename T> inline constexpr auto declval = []() noexcept -> add_fwref<T> {};

template<typename T> concept destructible = requires { requires std::is_destructible_v<T>; };
template<typename T> concept nt_destructible = destructible<T> && std::destructible<T>;
template<typename T, typename A> concept assignable = requires { requires std::is_assignable_v<T, A>; };
template<typename T, typename A> concept nt_assignable = assignable<T, A> && requires { requires std::is_nothrow_assignable_v<T, A>; };
template<typename T, typename... As> concept constructible = requires { T{declval<As>()...}; };
template<typename T, typename... As> concept nt_constructible = constructible<T, As...> && requires { { T{declval<As>()...} } noexcept; };
template<typename T, typename U = T> concept exchangeable = constructible<remove_cvref<T>, add_rvref<T>> && assignable<T, U>;
template<typename T, typename U = T> concept nt_exchangeable = nt_constructible<remove_cvref<T>, add_rvref<T>> && nt_assignable<T, U>;

inline constexpr auto addressof = []<is_lvref T>(T&& Ref) noexcept { return __builtin_addressof(Ref); };
inline constexpr auto asconst = []<typename T>(T&& Ref) noexcept -> add_const<T&&> { return Ref; };
inline constexpr auto mv = []<typename T>(T&& Ref) noexcept -> decltype(auto) { return static_cast<add_rvref<T>>(Ref); };
template<typename T> inline constexpr auto fwd = [](auto&& Ref) noexcept -> decltype(auto) { return static_cast<T&&>(Ref); };
template<typename T> inline constexpr auto construct = []<typename... Ts>(Ts&&... Args) noexcept(nt_constructible<T, Ts...>) -> T requires constructible<T, Ts...> { return T{fwd<Ts>(Args)...}; };
inline constexpr auto assign = []<typename L, typename R>(L&& Lhs, R&& Rhs) noexcept(nt_assignable<L, R>) -> decltype(auto) requires assignable<L, R> { return fwd<L>(Lhs) = fwd<R>(Rhs); };
inline constexpr auto exchange = []<typename T, typename U = T>(T& Ref, U&& Value) noexcept(nt_exchangeable<T&, U>) -> T requires exchangeable<T&, U> { auto a = mv(Ref); Ref = fwd<U>(Value); return a; };

/// decays the type
template<typename T> using decay = std::decay_t<T>;

/// performs `decay-copy` operation
inline constexpr auto decay_copy = []<typename T>(T&& Ref)
   noexcept(nt_convertible_to<T, decay<T>>) -> decay<T> { return fwd<T>(Ref); };

template<convertible_to<nat> auto I, nat N> inline constexpr bool selectable = (is_bool<decltype(I)> && N == 2) || (!is_bool<decltype(I)> && static_cast<nat>(I) < N);
template<convertible_to<nat> auto I, typename... Ts> requires selectable<I, sizeof...(Ts)> using select_type = typename _::_select_type<decltype(I), I, Ts...>::type;
template<convertible_to<nat> auto I, auto... Vs> requires selectable<I, sizeof...(Vs)> inline constexpr auto select_value = select_type<I, constant<Vs>...>::value;
template<convertible_to<nat> auto I> inline constexpr auto select_parameter = []<typename A, typename... As>(A&& Arg, As&&... Args) noexcept -> decltype(auto) requires selectable<I, 1 + sizeof...(As)> {
  if constexpr (same_as<decltype(I), nat>) { if constexpr (I == 0) return fwd<A>(Arg); else return select_parameter<I - 1>(fwd<As>(Args)...); }
  else if constexpr (is_bool<decltype(I)>) return select_parameter<nat(!I)>(fwd<A>(Arg), fwd<As>(Args)...); else return select_parameter<nat(I)>(fwd<A>(Arg), fwd<As>(Args)...); };

template<typename... Fs> struct Overload : public Fs... { using Fs::operator()...; };
struct Pass { template<typename T> constexpr T&& operator()(T&& a) const noexcept { return static_cast<T&&>(a); } };
inline constexpr Pass pass;
template<bool... Bs> inline constexpr nat counts = (nat(0) + ... + Bs);
template<bool... Bs> inline constexpr nat inspects = 0;
template<bool B, bool... Bs> inline constexpr nat inspects<B, Bs...> = B ? 0 : 1 + inspects<Bs...>;

namespace _ {
template<typename T, typename U> struct _copy_reference : _type<remove_ref<T>> {};
template<typename T, typename U> struct _copy_reference<T, U&> : _type<add_lvref<T>> {};
template<typename T, typename U> struct _copy_reference<T, U&&> : _type<add_rvref<T>> {};
template<typename T, typename Src> struct _copy_extent : _type<T> {};
template<typename T, is_array U> struct _copy_extent<T, U> : _type<add_extent<T, std::extent_v<U>>> {};
template<typename... Ts> struct _common_type : _type<None> {};
template<typename... Ts> requires requires { typename std::void_t<std::common_reference_t<Ts...>>; } struct _common_type<Ts...> : std::common_reference<Ts...> {};
template<typename T, template<typename...> typename Tm> struct _specialization_of : constant<false> {};
template<template<typename...> typename Tm, typename... Ts> struct _specialization_of<Tm<Ts...>, Tm> : constant<true> {};
template<typename T, typename U> struct _variation_of : constant<false> {};
template<template<typename...> typename Tm, typename... Ts, typename... Us> struct _variation_of<Tm<Ts...>, Tm<Us...>> : constant<true> {};
template<template<auto...> typename Tm, auto... Vs, auto... Ws> struct _variation_of<Tm<Vs...>, Tm<Ws...>> : constant<true> {};
template<template<typename, auto...> typename Tm, typename T, typename U, auto... Vs, auto... Ws> struct _variation_of<Tm<T, Vs...>, Tm<U, Ws...>> : constant<true> {};
template<template<auto, typename...> typename Tm, auto V, auto W, typename... Ts, typename... Us> struct _variation_of<Tm<V, Ts...>, Tm<W, Us...>> : constant<true> {};
}

template<typename T, typename Src> using copy_const = select_type<is_const<Src>, add_const<T>, T>;
template<typename T, typename Src> using copy_volatile = select_type<is_volatile<Src>, add_volatile<T>, T>;
template<typename T, typename Src> using copy_cv = copy_const<copy_volatile<T, Src>, Src>;
template<typename T, typename Src> using copy_reference = typename _::_copy_reference<T, Src>::type;
template<typename T, typename Src> using copy_cvref = copy_reference<copy_cv<T, Src>, Src>;
template<typename T, typename Src> using copy_extent = typename _::_copy_extent<T, Src>::type;
template<typename T, typename Src> using copy_pointer = select_type<is_pointer<Src>, copy_cv<add_pointer<T>, Src>, T>;

template<typename F, typename... As> concept invocable = std::invocable<F, As...>;
template<typename F, typename... As> concept nt_invocable = invocable<F, As...> && requires { requires std::is_nothrow_invocable_v<F, As...>; };
template<typename F, typename... As> using invoke_result = select_type<is_void<std::invoke_result_t<F, As...>>, None, std::invoke_result_t<F, As...>>;
template<typename F, typename R, typename... As> concept invocable_r = invocable<F, As...> && convertible_to<invoke_result<F, As...>, R>;
template<typename F, typename R, typename... As> concept nt_invocable_r = nt_invocable<F, As...> && nt_convertible_to<invoke_result<F, As...>, R>;
template<typename F, typename... As> concept predicate = invocable_r<F, bool, As...>;
inline constexpr auto invoke = []<typename F, typename... As>(F&& Func, As&&... Args) noexcept(nt_invocable<F, As...>) -> invoke_result<F, As...> requires invocable<F, As...> {
  if constexpr (is_void<std::invoke_result_t<F, As...>>) return std::invoke(fwd<F>(Func), fwd<As>(Args)...), None{}; else return std::invoke(fwd<F>(Func), fwd<As>(Args)...); };
template<typename R> inline constexpr auto invoke_r = []<typename F, typename... As>(F&& f, As&&... as) noexcept(nt_invocable_r<F, R, As...>) -> R requires invocable_r<F, R, As...> {
  return static_cast<R>(invoke(fwd<F>(f), fwd<As>(as)...)); };

template<typename... Ts> using common_type = typename _::_common_type<Ts...>::type;
template<typename... Ts> concept common_with = requires {
  requires !same_as<remove_cv<common_type<Ts...>>, None>;
  requires (std::common_reference_with<common_type<Ts...>, Ts> && ...); };

template<typename T, template<typename...> typename Tm> concept specialization_of = _::_specialization_of<T, Tm>::value;
template<typename T, typename U> concept variation_of = _::_variation_of<T, U>::value;

struct Eq { constexpr bool operator()(auto&& a, auto&& b) const noexcept(noexcept(a == b)) requires requires { { a == b } -> convertible_to<bool>; } { return a == b; } };
struct Ne { constexpr bool operator()(auto&& a, auto&& b) const noexcept(noexcept(a != b)) requires requires { { a != b } -> convertible_to<bool>; } { return a != b; } };
struct Lt { constexpr bool operator()(auto&& a, auto&& b) const noexcept(noexcept(a < b)) requires requires { { a < b } -> convertible_to<bool>; } { return a < b; } };
struct Le { constexpr bool operator()(auto&& a, auto&& b) const noexcept(noexcept(a <= b)) requires requires { { a <= b } -> convertible_to<bool>; } { return a <= b; } };
struct Gt { constexpr bool operator()(auto&& a, auto&& b) const noexcept(noexcept(a > b)) requires requires { { a > b } -> convertible_to<bool>; } { return a > b; } };
struct Ge { constexpr bool operator()(auto&& a, auto&& b) const noexcept(noexcept(a >= b)) requires requires { { a >= b } -> convertible_to<bool>; } { return a >= b; } };

inline constexpr Eq eq;
inline constexpr Ne ne;
inline constexpr Lt lt;
inline constexpr Le le;
inline constexpr Gt gt;
inline constexpr Ge ge;

template<trivial T> constexpr T bitcast(const trivial auto& Ref) noexcept requires (sizeof(T) == sizeof(Ref)) { return __builtin_bit_cast(T, Ref); };
inline constexpr auto bitcount = [](const nat Value) noexcept -> nat { return std::popcount(Value); };
inline constexpr auto bitfloor = [](const nat Value) noexcept -> nat { return std::bit_floor(Value); };
inline constexpr auto bitceil = [](const nat Value) noexcept -> nat { return std::bit_ceil(Value); };
inline constexpr auto bitwidth = [](const nat Value) noexcept -> nat { return std::bit_width(Value); };

template<typename... Fs> struct caster : public Fs... {
private:
  template<typename T> static constexpr nat i = inspects<same_as<T, invoke_result<Fs>>...>;
  template<typename T> static constexpr nat j = i<T> < sizeof...(Fs) ? i<T> : inspects<convertible_to<T, invoke_result<Fs>>...>;
  template<typename... As> static constexpr nat k = inspects<invocable<Fs, As...>...>;
public:
  using Fs::operator()...;
  template<typename T> requires (j<T> < sizeof...(Fs)) constexpr operator T() const noexcept(nt_convertible_to<invoke_result<select_type<j<T>, Fs...>>, T>)
    requires convertible_to<invoke_result<select_type<j<T>, Fs...>>, T> { return select_type<j<T>, Fs...>::operator()(); }
  template<typename... As> requires (k<As...> < sizeof...(Fs)) constexpr auto operator()(As&&... Args) const noexcept(nt_invocable<select_type<k<As...>, Fs...>, As...>)
    requires invocable<select_type<k<As...>, Fs...>, As...> { return select_type<k<As...>, Fs...>::operator()(fwd<As>(Args)...); }
};


/// checks if the code is in the constant evaluation
/// \note Used as the condition of `if`.
inline constexpr caster IS_CEV{
[]() noexcept { return std::is_constant_evaluated(); }
};


/// checks if the code is not in the constant evaluation
/// \note Put this anywhere in the code, and if constant evaluation is detected, the program will not compile.
constexpr void cannot_be_constant_evaluated(auto&&...) {
  if (IS_CEV) throw "This code cannot be constant evaluated.";
}

namespace _ {
template<typename T> struct _iter_t {};
template<std::ranges::range Rg> struct _iter_t<Rg> : _iter_t<std::ranges::iterator_t<Rg>> {};
template<std::input_or_output_iterator It> struct _iter_t<It> {
  using v = std::iter_value_t<It>; using d = std::iter_difference_t<It>;
  using r = std::iter_reference_t<It>; using rr = std::iter_rvalue_reference_t<It>; };
}

template<typename It> concept iterator = std::input_or_output_iterator<It>;
template<typename Se, typename It> concept sentinel_for = std::sentinel_for<Se, It>;
template<typename Se, typename It> concept sized_sentinel_for = std::sized_sentinel_for<Se, It>;
template<typename Rg> concept range = std::ranges::range<Rg>;
template<typename Rg> concept borrowed_range = std::ranges::borrowed_range<Rg>;
template<typename Rg> concept sized_range = std::ranges::sized_range<Rg>;
template<range Rg> using iterator_t = std::ranges::iterator_t<Rg>;
template<range Rg> using borrowed_iterator_t = std::ranges::borrowed_iterator_t<Rg>;
template<range Rg> using sentinel_t = std::ranges::sentinel_t<Rg>;

/// value type of iterator/range
template<typename T> requires iterator<remove_ref<T>> || range<remove_ref<T>>
using iter_value = typename _::_iter_t<remove_ref<T>>::v;

/// difference type of iterator/range
template<typename T> requires iterator<remove_ref<T>> || range<remove_ref<T>>
using iter_difference = typename _::_iter_t<remove_ref<T>>::d;

/// reference type of iterator/range
template<typename T> requires iterator<remove_ref<T>> || range<remove_ref<T>>
using iter_reference = typename _::_iter_t<remove_ref<T>>::r;

/// rvalue reference type of iterator/range
template<typename T> requires iterator<remove_ref<T>> || range<remove_ref<T>>
using iter_rvref = typename _::_iter_t<remove_ref<T>>::rr;

/// common type of `iter_reference` and `iter_value`
template<typename T> requires iterator<remove_ref<T>> || range<remove_ref<T>>
using iter_common = common_type<iter_reference<T>, iter_value<T>>;

template<typename It, typename T> concept output_iterator = std::output_iterator<It, T>;
template<typename It> concept input_iterator = std::input_iterator<It>;
template<typename It> concept fwd_iterator = std::forward_iterator<It>;
template<typename It> concept bid_iterator = std::bidirectional_iterator<It>;
template<typename It> concept rnd_iterator = std::random_access_iterator<It>;
template<typename It> concept cnt_iterator = std::contiguous_iterator<It>;
template<typename It, typename U> concept iterator_for = iterator<It> && convertible_to<iter_reference<It>, U>;
template<typename It, typename U> concept cnt_iterator_of = cnt_iterator<It> && same_as<remove_ref<iter_reference<It>>, U>;
template<typename Rg, typename T> concept output_range = std::ranges::output_range<Rg, T>;
template<typename Rg> concept input_range = std::ranges::input_range<Rg>;
template<typename Rg> concept fwd_range = std::ranges::forward_range<Rg>;
template<typename Rg> concept bid_range = std::ranges::bidirectional_range<Rg>;
template<typename Rg> concept rnd_range = std::ranges::random_access_range<Rg>;
template<typename Rg> concept cnt_range = std::ranges::contiguous_range<Rg>;
template<typename Rg, typename U> concept range_for = iterator_for<iterator_t<Rg>, U>;
template<typename Rg, typename U> concept cnt_range_of = cnt_range<Rg> && same_as<iter_value<iterator_t<Rg>>, U>;
template<typename Rg> concept cnt_sized_range = cnt_range<Rg> && sized_range<Rg>;
template<typename Rg, typename U> concept cnt_sized_range_of = cnt_sized_range<Rg> && cnt_range_of<Rg, U>;
template<typename It, typename In> concept iter_copyable = iterator<It> && iterator<In> && std::indirectly_copyable<In, It>;
template<typename It, typename In> concept iter_movable = iterator<It> && iterator<In> && std::indirectly_movable<In, It>;
template<typename Fn, typename It> concept iter_unary_invocable = iterator<It> && std::indirectly_unary_invocable<Fn, It>;
template<typename Fn, typename It> concept iter_unary_predicate = iterator<It> && std::indirect_unary_predicate<Fn, It>;
inline constexpr auto begin = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::begin(fwd<Rg>(r)));
inline constexpr auto end = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::end(fwd<Rg>(r)));
inline constexpr auto rbegin = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::rbegin(fwd<Rg>(r)));
inline constexpr auto rend = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::rend(fwd<Rg>(r)));
inline constexpr auto size = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::size(fwd<Rg>(r)));
inline constexpr auto empty = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::empty(fwd<Rg>(r)));
inline constexpr auto data = []<range Rg>(Rg&& r) ywlib_wrapper(std::ranges::data(fwd<Rg>(r)));
inline constexpr auto iter_move = []<iterator It>(It&& i) ywlib_wrapper(std::ranges::iter_move(fwd<It>(i)));
inline constexpr auto iter_swap = []<iterator It, iterator Jt>(It&& i, Jt&& j) ywlib_wrapper(std::ranges::iter_swap(fwd<It>(i), fwd<Jt>(j)));

namespace _::_get {
template<nat I> void get(auto&&) = delete;
template<nat I, typename T> inline constexpr auto pattern = []() -> int {
  if constexpr (is_bounded_array<remove_ref<T>>) return I < std::extent_v<remove_ref<T>> ? 16 : 8;
  else if constexpr (requires { get<I>(declval<T>()); }) return 1 | noexcept(get<I>(declval<T>())) * 16;
  else if constexpr (requires { declval<T>().template get<I>(); }) return 2 | noexcept(declval<T>().template get<I>()) * 16;
  else return I == 0 ? 20 : 8; }();
template<nat I, typename T, nat P = pattern<I, T>> requires(P != 8) constexpr decltype(auto) call(T&& A) noexcept(bool(P & 16)) {
  if constexpr ((P & 7) == 0) return static_cast<T&&>(A)[I];
  else if constexpr ((P & 7) == 1) return get<I>(static_cast<T&&>(A));
  else if constexpr ((P & 7) == 2) return static_cast<T&&>(A).template get<I>();
  else if constexpr ((P & 7) == 4) return static_cast<T&&>(A); }
template<typename T, nat I = (pattern<0, T> & 15)> struct extent : std::tuple_size<T> {};
template<typename T> struct extent<T, 0> : std::extent<remove_ref<T>> {};
template<typename T> struct extent<T, 4> : constant<1> {};
}

template<typename T> inline constexpr nat extent = _::_get::extent<remove_ref<T>>::value;
template<typename T, typename... Ts> concept same_extent = ((extent<T> == extent<Ts>) && ...);
template<typename T> concept tuple = requires { requires (_::_get::pattern<0, T> & 15) < 3; };
template<typename T, nat I> concept gettable = requires { _::_get::call<I>(declval<T>()); };
template<typename T, nat I> concept nt_gettable = gettable<T, I> && requires { { _::_get::call<I>(declval<T>()) } noexcept; };
template<typename T, nat I> requires gettable<T, I> using element_t = decltype(_::_get::call<I>(declval<T>()));
template<nat I> inline constexpr auto get = []<gettable<I> T>(T&& Ref) noexcept(nt_gettable<T, I>) -> element_t<T, I> { return _::_get::call<I>(fwd<T>(Ref)); };



} // namespace yw
