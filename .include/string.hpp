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

namespace _ {
constexpr cat2 from_sj(add_lvref<const cat1*> s) noexcept {
  constexpr cat2 hankaku[] = {
    0x0000, 0xff61, 0xff62, 0xff63, 0xff64, 0xff65, 0xff66, 0xff67, 0xff68, 0xff69, 0xff6a, 0xff6b, 0xff6c, 0xff6d,
    0xff6e, 0xff6f, 0xff70, 0xff71, 0xff72, 0xff73, 0xff74, 0xff75, 0xff76, 0xff77, 0xff78, 0xff79, 0xff7a, 0xff7b,
    0xff7c, 0xff7d, 0xff7e, 0xff7f, 0xff80, 0xff81, 0xff82, 0xff83, 0xff84, 0xff85, 0xff86, 0xff87, 0xff88, 0xff89,
    0xff8a, 0xff8b, 0xff8c, 0xff8d, 0xff8e, 0xff8f, 0xff90, 0xff91, 0xff92, 0xff93, 0xff94, 0xff95, 0xff96, 0xff97,
    0xff98, 0xff99, 0xff9a, 0xff9b, 0xff9c, 0xff9d, 0xff9e, 0xff9f, };
  if (*s ^ 0x80) return *s;
  else if ((*s & 0x20) ^ ((*s & 0x40) >> 1)) return hankaku[*s - 160];
  else return 0;
}
}

} // namespace yw
