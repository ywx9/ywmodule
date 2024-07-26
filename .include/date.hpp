#pragma once

#ifndef YWLIB
#include <chrono>
#else
import std;
#endif

#include "string.hpp"

export namespace yw {

struct date {

  int year{};

  int month{};

  int day{};

  /// initializes the information by the given date.
  template<typename Clock, typename Duration>
  date(const std::chrono::time_point<Clock, Duration>& tp) noexcept {
    auto d = std::chrono::floor<std::chrono::days>(tp);
    auto ymd = std::chrono::year_month_day(d);
    year = int(ymd.year());
    month = int(ymd.month());
    day = int(ymd.day());
  }

  /// default constructor to initialize the information by the current date.
  date() noexcept
    : date(std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time()) {}

  /// initializes the information by the given date.
  date(numeric auto Y, numeric auto M, numeric auto D) noexcept
    : year(Y), month(M), day(D) {}

  ///
};
}
