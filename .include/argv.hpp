#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#undef max
#undef min
#undef ERROR

#include "array.hpp"
#include "string.hpp"

export namespace yw {

/// command line arguments; argv[0] is the program name
inline Array<str1> argv = [] {
  int argc;
  auto a = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  Array<str1> r(argc);
  for (; 0 <= --argc;) r[argc] = cvt<cat1>(a[argc]);
  return r;
}();

} // namespace yw
