#pragma once

#ifndef YWLIB
#include <string>
#include <string_view>
#else
import std;
#endif

#include "core.hpp"

export namespace yw {

template<character Ct> using String = std::basic_string<Ct>;
template<character Ct> using StringView = std::basic_string_view<Ct>;
using str1 = String<cat1>;
using str2 = String<cat2>;
using stv1 = StringView<cat1>;
using stv2 = StringView<cat2>;

/// checks if `T` is a string-like type.
template<typename T, typename Ct = iter_value<T>>
concept stringable = nt_convertible_to<T&, StringView<Ct>>;

/// returns the length of the string.
inline constexpr Overload strlen{
  []<character Ct>(const Ct* const Str) { return std::char_traits<Ct>::length(Str); },
  []<range Rg>(Rg&& Str) requires character<iter_value<Rg>> { return std::ranges::distance(Str); }
};

} // namespace yw
