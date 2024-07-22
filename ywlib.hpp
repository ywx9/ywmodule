#pragma once


#ifndef YWLIB
#define YWLIB
#include <xstring>
#include <string>
#include <string_view>
#include <format>
#include <print>
#else
import std;
#endif

namespace yw {

inline void hello_world() {
  std::print("Hello, World from {}!\n", __FILE__);
}

} // namespace yw
