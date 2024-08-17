#pragma once

#include "string.hpp"

export namespace yw {

/// struct to represent a source location
struct Source {
  Source(const char*) = delete;

  const cat1* const file;

  const cat1* const func;

  const int4 line;

  const int4 column;

  /// default constructor to initialize the information
  constexpr Source(
    const cat1* file = __builtin_FILE(),
    const cat1* func = __builtin_FUNCTION(),
    const int4 line = __builtin_LINE(),
    const int4 column = __builtin_COLUMN())
    : file(file), func(func), line(line), column(column) {}
};

}
