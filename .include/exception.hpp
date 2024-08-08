/// \file exception.hpp
/// \brief defines exception classes

#pragma once

#ifndef YWLIB
#include <exception>
#else
import std;
#endif

#include "logger.hpp"

export namespace yw {


/// class to represent a exception with source location
class Exception : public std::runtime_error {
public:

  /// constructs an exception with a message
  Exception(stringable auto&& Str, const Source& _ = {})
    : std::runtime_error(std::format(
      "{}\n ↑ thrown at {}({},{})",
      cvt<cat1>(Str), _.file, _.line, _.column)) {}

  /// constructs an exception with a message
  Exception(const Exception& e, const Source& _ = {})
    : Exception(e.what(), _) {}

  /// constructs an exception with a `std::exception`
  /// \note the message is converted from SJIS to UTF-8
  Exception(const std::exception& e, const Source& _ = {})
    : Exception(from_sjis(e.what()), _) {}
};

} // namespace yw
