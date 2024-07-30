#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef max
#undef min

#include "string.hpp"

export namespace yw::win {

inline str2 from_shiftjis(const stv1 sj) {
  str2 s(::MultiByteToWideChar(932, 0, sj.data(), int(sj.size()), nullptr, 0), L'0');
  ::MultiByteToWideChar(932, 0, sj.data(), int(sj.size()), s.data(), int(s.size()));
  return s;
}

inline str1 to_shiftjis(const stv2 w) {
  str1 s(::WideCharToMultiByte(932, 0, w.data(), int(w.size()), nullptr, 0, nullptr, nullptr), '0');
  ::WideCharToMultiByte(932, 0, w.data(), int(w.size()), s.data(), int(s.size() + 1), nullptr, nullptr);
  return s;
}

inline int4 WideCharToMultiByte(nat4 CodePage, nat4 dwFlags, const cat2* lpWideCharStr, int4 cchWideChar, cat1* lpMultiByteStr, int4 cbMultiByte, const cat1* lpDefaultChar, int4* lpUsedDefaultChar) {
  return ::WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte, lpDefaultChar, lpUsedDefaultChar);
}

} // namespace yw::win
