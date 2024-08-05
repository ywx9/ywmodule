#pragma once

#ifndef YWLIB
#include <filesystem>
#include <fstream>
#include <iostream>
#else
import std;
#endif

#include "chrono.hpp"
#include "source.hpp"

export namespace yw {


/// class to represent a path
using Path = std::filesystem::path;


/// class to represent a logger
class Logger {
protected:
  String<cat1> text{};
public:

  /// enum to represent the log level
  enum class Level {
    DEBUG   = 10,
    INFO    = 20,
    WARNING = 30,
    ERROR   = 40,
    FATAL   = 50, };
  friend constexpr auto operator==(Level a, Level b) noexcept { return int(a) == int(b); }
  friend constexpr auto operator<=>(Level a, Level b) noexcept { return int(a) <=> int(b); }

  /// file path to write the log
  Path file{};

  /// log level to write
  Level level{Level::INFO};

  /// flag to output the log to the console
  bool console{true};

  /// destructs the logger and writes the log to the file
  virtual ~Logger() {
    try {
      // if `text` is empty, do nothing
      if (text.empty()) return;
      // if there is not the directory to write the log file, create it
      if (const auto dir = file.parent_path(); !std::filesystem::exists(dir)) std::filesystem::create_directories(dir);
      // write the log to the file
      if (std::ofstream ofs(file, std::ios::app); ofs) ofs << text, text.clear();
      else { // if the file cannot be opened, write the log to another file; `[parent]\[stem].[ext]` -> `[parent]\[stem].[no].[ext]`
        auto parent = file.parent_path().string<cat1>(), stem = file.stem().string<cat1>(), ext = file.extension().string<cat1>();
        for (nat i{1}; i < 1000; ++i) { // try 1000 times
          ofs.open(std::format("{}\\{}.{:03d}{}", parent, stem, i, ext), std::ios::app);
          if (ofs) { ofs << text, text.clear(); return; }
        }
        std::cerr << "Failed to write the log to the file: " << file << std::endl;
      }
    } catch (...) { std::cerr << "Failed to write the log to the file: " << file << std::endl; }
  }

  /// constructs the logger with the file path
  /// \param File file path to write the log
  explicit Logger(convertible_to<Path> auto&& File) : file(static_cast<decltype(File)>(File)) {}

  /// appends the log text after formatting
  /// \param Level log level; the text length must be less than 7
  /// \param Text text to log
  /// \note output format: `YYYY-MM-DD HH:MM:SS [Level] Text`
  void write(stv1 Level, stv1 Text) {
    text.reserve(text.size() + 38 + Text.size());
    auto t = std::format("{} [{:^7}] {}\n", now(), Level, Text);
    if (console) std::cout << t;
    text += t;
  }

  /// appends the log text after formatting
  /// \param Level log level; the text length must be less than 7
  /// \param Text text to log
  /// \param _ source location
  /// \note output format: `YYYY-MM-DD HH:MM:SS [Level] Text ([func] at [file]([line],[column]))`
  void write(stv1 Level, stv1 Text, const Source& _) {
    auto src = std::format("({} at {}({},{}))", _.func, _.file, _.line, _.column);
    text.reserve(text.size() + 39 + Text.size() + src.size());
    auto t = std::format("{} [{:^7}] {} {}\n", now(), Level, Text, src);
    if (console) std::cout << t;
    text += t;
  }

  /// appends the dubug log
  /// \param Text text to log
  void debug(stv1 Text, const Source& _ = {}) { if (level <= Level::DEBUG) write("DEBUG", Text, _); }

  /// appends the info log
  /// \param Text text to log
  void info(stv1 Text) { if (level <= Level::INFO) write("INFO", Text); }

  /// appends the warning log
  /// \param Text text to log
  void warning(stv1 Text) { if (level <= Level::WARNING) write("WARNING", Text); }

  /// appends the error log
  /// \param Text text to log
  void error(stv1 Text) { if (level <= Level::ERROR) write("ERROR", Text); }

  /// appends the fatal log
  /// \param Text text to log
  void fatal(stv1 Text) { if (level <= Level::FATAL) write("FATAL", Text); }
};


/// global logger
// \note The file path is replaced if `"windows.hpp"` is included.
inline Logger logger("log.txt");

} // namespace yw
