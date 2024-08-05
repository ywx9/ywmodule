/// \file database.hpp
/// \brief defines classes for database operations

#pragma once

// #ifndef YWLIB
// #include <unordered_map>
// #else
// import std;
// #endif

#include "array.hpp"
#include "string.hpp"

namespace yw {

template<typename... Ts> class Table {
public:

  ///

  /// value type
  enum class Type {
    NULL_,
    BOOL,
    INT,
    REAL,
    TEXT,
    BLOB,
  };

  /// class to represent a value
  class Value {
    union {

    };
  }

protected:

public:

};

}
