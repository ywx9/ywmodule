#pragma once

#ifndef YWLIB
#include <filesystem>
#else
import std;
#endif

#include "string.hpp"

export namespace yw {


/// class to represent a path
using Path = std::filesystem::path;


/// class to represent a logger
class Logger {
protected:
  String<cat1> text{};
public:
  Path file{};
};


/// namespace for file operations
namespace file {

/// checks if a file exists
inline constexpr auto exists =
  [](convertible_to<Path> auto&& p) noexcept -> bool {
  try { return std::filesystem::exists(std::forward<decltype(p)>(p)); }
  catch (...) { return false; }
};

/// checks if a file is a regular file
inline constexpr auto is_regular_file =
  [](convertible_to<Path> auto&& p) noexcept -> bool {
  try { return std::filesystem::is_regular_file(std::forward<decltype(p)>(p)); }
  catch (...) { return false; }
};

/// checks if a file is a directory
inline constexpr auto is_directory =
  [](convertible_to<Path> auto&& p) noexcept -> bool {
  try { return std::filesystem::is_directory(std::forward<decltype(p)>(p)); }
  catch (...) { return false; }
};

/// obtains the size of a file
inline constexpr auto file_size =
  [](convertible_to<Path> auto&& p) noexcept -> nat {
  try { return std::filesystem::file_size(std::forward<decltype(p)>(p)); }
  catch (...) { return 0; }
};

/// creates a directory
inline constexpr auto create_directory =
  [](convertible_to<Path> auto&& p) noexcept -> bool {
  try { return std::filesystem::create_directories(std::forward<decltype(p)>(p)); }
  catch (...) { return false; }
};

/// removes a file
inline constexpr auto remove_file =
  [](convertible_to<Path> auto&& p) noexcept -> bool {
  try { return std::filesystem::remove(std::forward<decltype(p)>(p)); }
  catch (...) { return false; }
};

} // namespace file

} // namespace yw
