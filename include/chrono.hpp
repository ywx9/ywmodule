#pragma once

#ifndef YWLIB
#include <chrono>
#else
import std;
#endif

#include "string.hpp"

export namespace yw {

/// struct to represent a date
struct Date {

  int4 year{}, month{}, day{};

  template<typename Clock, typename Duration>
  Date(const std::chrono::time_point<Clock, Duration>& tp) {
    const auto d = std::chrono::floor<std::chrono::days>(tp);
    const auto ymd = std::chrono::year_month_day(d);
    year = int4(ymd.year());
    month = int4(nat4(ymd.month()));
    day = int4(nat4(ymd.day()));
  }

  Date() : Date(std::chrono::zoned_time(
    std::chrono::current_zone(), std::chrono::system_clock::now()
    ).get_local_time()) {}

  Date(const numeric auto Y, const numeric auto M, const numeric auto D)
    : year(int4(Y)), month(int4(M)), day(int4(D)) {}

  operator str1() const {
    return std::format("{:04d}-{:02d}-{:02d}", year, month, day);
  }

  explicit operator str2() const {
    return std::format(L"{:04d}-{:02d}-{:02d}", year, month, day);
  }

  explicit operator String<uct1>() const {
    auto r = String<uct1>(10, 0);
    std::format_to(r.data(), "{:04d}-{:02d}-{:02d}", year, month, day);
    return r;
  }

  explicit operator String<uct2>() const {
    auto r = String<uct2>(10, 0);
    std::format_to(r.data(), L"{:04d}-{:02d}-{:02d}", year, month, day);
    return r;
  }

  explicit operator String<uct4>() const {
    auto r = String<uct4>(10, 0);
    std::ranges::copy(std::format("{:04d}-{:02d}-{:02d}", year, month, day), r.begin());
    return r;
  }
};


/// struct to represent a time
struct Time {

  int4 hour{}, minute{}, second{};

  template<typename Clock, typename Duration>
  Time(const std::chrono::time_point<Clock, Duration>& tp) {
    const auto tod = std::chrono::floor<std::chrono::milliseconds>(tp - std::chrono::floor<std::chrono::days>(tp));
    const auto hms = std::chrono::hh_mm_ss(tod);
    hour = int4(hms.hours().count());
    minute = int4(hms.minutes().count());
    second = int4(hms.seconds().count());
  }

  Time() : Time(std::chrono::zoned_time(
    std::chrono::current_zone(), std::chrono::system_clock::now()
    ).get_local_time()) {}

  Time(const numeric auto H, const numeric auto M, const numeric auto S)
    : hour(int4(H)), minute(int4(M)), second(int4(S)) {}

  operator str1() const {
    return std::format("{:02d}:{:02d}:{:02d}", hour, minute, second);
  }

  explicit operator str2() const {
    return std::format(L"{:02d}:{:02d}:{:02d}", hour, minute, second);
  }

  explicit operator String<uct1>() const {
    auto r = String<uct1>(8, 0);
    std::format_to(r.data(), "{:02d}:{:02d}:{:02d}", hour, minute, second);
    return r;
  }

  explicit operator String<uct2>() const {
    auto r = String<uct2>(8, 0);
    std::format_to(r.data(), L"{:02d}:{:02d}:{:02d}", hour, minute, second);
    return r;
  }

  explicit operator String<uct4>() const {
    auto r = String<uct4>(8, 0);
    std::ranges::copy(std::format("{:02d}:{:02d}:{:02d}", hour, minute, second), r.begin());
    return r;
  }
};


/// struct to represent a date and time
struct Now {

  Date date;
  Time time;

  template<typename Clock, typename Duration>
  Now(const std::chrono::time_point<Clock, Duration>& tp)
    : date(tp), time(tp) {}

  Now() : Now(std::chrono::zoned_time(
    std::chrono::current_zone(), std::chrono::system_clock::now()
    ).get_local_time()) {}

  Now(const Date& d, const Time& t) : date(d), time(t) {}

  operator const Date&() const { return date; }

  operator const Time&() const { return time; }

  operator str1() const {
    return std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
      date.year, date.month, date.day, time.hour, time.minute, time.second);
  }

  explicit operator str2() const {
    return std::format(L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
      date.year, date.month, date.day, time.hour, time.minute, time.second);
  }

  explicit operator String<uct1>() const {
    auto r = String<uct1>(19, 0);
    std::format_to(r.data(), "{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
      date.year, date.month, date.day, time.hour, time.minute, time.second);
    return r;
  }

  explicit operator String<uct2>() const {
    auto r = String<uct2>(19, 0);
    std::format_to(r.data(), L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
      date.year, date.month, date.day, time.hour, time.minute, time.second);
    return r;
  }

  explicit operator String<uct4>() const {
    auto r = String<uct4>(19, 0);
    std::ranges::copy(std::format("{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}",
      date.year, date.month, date.day, time.hour, time.minute, time.second), r.begin());
    return r;
  }
};


/// caster to return the current date and time
inline constexpr caster now{[]{ return Now{}; }};

} // namespace yw


namespace std {

template<typename Ct> struct formatter<yw::Date, Ct> : formatter<basic_string<Ct>, Ct> {
  auto format(const yw::Date& d, auto& Ctx) const {
    return formatter<basic_string<Ct>, Ct>::format(basic_string<Ct>(d), Ctx);
  }
};

template<typename Ct> struct formatter<yw::Time, Ct> : formatter<basic_string<Ct>, Ct> {
  auto format(const yw::Time& t, auto& Ctx) const {
    return formatter<basic_string<Ct>, Ct>::format(basic_string<Ct>(t), Ctx);
  }
};

template<typename Ct> struct formatter<yw::Now, Ct> : formatter<basic_string<Ct>, Ct> {
  auto format(const yw::Now& n, auto& Ctx) const {
    return formatter<basic_string<Ct>, Ct>::format(basic_string<Ct>(n), Ctx);
  }
};

} // namespace std
