#pragma once

#include "logger.hpp"

export namespace yw {

/// namespace for file operations
namespace file {

/// checks if a file exists
inline constexpr auto exists =
  [](convertible_to<Path> auto&& p) noexcept -> bool {
  try { return std::filesystem::exists(std::forward<decltype(p)>(p)); }
  catch (const std::exception& e) { logger.warning(e.what()); return false; }
  catch (...) { logger.fatal("Failed to check if the file exists."); return false; }
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

/// returns the file list of a directory
inline constexpr auto list = [](convertible_to<Path> auto&& p, bool Recursive = false) noexcept -> Array<Path> {
  try {
    Array<Path> r;
    if (!exists(p)) { logger.warning(std::format("The directory does not exist: {}", p)); return {}; }
    else if (!is_directory(p)) { logger.warning(std::format("The path is not a directory: {}", p)); return {}; }
    if (Recursive) for (const auto& e : std::filesystem::recursive_directory_iterator(p)) r.push_back(e.path());
    else for (const auto& e : std::filesystem::directory_iterator(p)) r.push_back(e.path());
    return r;
  } catch (const std::exception& e) { return logger.error(e.what()), Array<Path>{}; }
  catch (...) { return logger.error(std::format("Failed to list the directory by unknown error: {}", p)), Array<Path>{}; }
};

/// reads a file as a string
inline constexpr auto read = [](convertible_to<Path> auto&& p) noexcept {
  try {
    if (!exists(p)) return logger.warning(std::format("The file does not exist: {}", p)), std::string{};
    if (!is_regular_file(p)) return logger.warning(std::format("The path is not a file: {}", p)), std::string{};
    std::ifstream ifs(p);
    if (!ifs) return logger.error(std::format("Failed to open the file: {}", p)), std::string{};
    return std::string{std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{}};
  } catch (const std::exception& e) { return logger.error(e.what()), std::string{}; }
  catch (...) { return logger.error(std::format("Failed to read the file by unknown error: {}", p)), std::string{}; }
};

/// writes data to a file
inline constexpr auto write = [](convertible_to<Path> auto&& p, cnt_range auto&& d) noexcept -> bool {
  try {
    std::ofstream ofs(p);
    if (!ofs) return logger.error(std::format("Failed to open the file: {}", p)), false;
    std::copy(std::begin(d), std::end(d), std::ostreambuf_iterator<char>{ofs});
    return true;
  } catch (const std::exception& e) { return logger.error(e.what()), false; }
  catch (...) { return logger.error(std::format("Failed to write the file by unknown error: {}", p)), false; }
};

} // namespace file

} // namespace yw
