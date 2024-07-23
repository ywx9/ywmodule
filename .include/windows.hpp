#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "string.hpp"

export namespace yw::win {

inline str2 from_shiftjis(const stv1 sj) {
  str2 s(::MultiByteToWideChar(932, 0, sj.data(), int(sj.size()), nullptr, 0), L'0');
  ::MultiByteToWideChar(932, 0, sj.data(), int(sj.size()), s.data(), int(s.size()));
  return s;
}

}
