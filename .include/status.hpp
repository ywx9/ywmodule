/// \file status.hpp
/// \brief defines `class yw::Status`

#pragma once

#include "core.hpp"

export namespace yw {


/// class to represent a status
/// \tparam T type of the status
/// \tparam Processor callable object; `Processor(T)` is called when the status is changed
template<typename T, auto Processor = none> class Status {
protected:
  T _value{};
  mutable bool _changed{};
public:

  /// default constructor
  Status() noexcept = default;

  /// constructor with the initial value
  explicit Status(const T& Value) noexcept : _value{Value} {}

  /// constructor with the initial value
  explicit Status(T&& Value) noexcept : _value{mv(Value)} {}

  /// returns the current value of the status
  const T& value() const noexcept { return _changed = false, _value; }

  /// checks if the Status is changed after the last call of `value()`
  bool changed() const noexcept { return _changed; }

  /// function calll operator which returns the current value of the status
  const T& operator()() const noexcept { return value(); }

  /// assignment operator which changes the value of the status
  Status& operator=(const T& Value) noexcept {
    return _changed = true, _value = Value, Processor(_value), *this;
  }

  /// assignment operator which changes the value of the status
  Status& operator=(T&& Value) noexcept {
    return _changed = true, _value = mv(Value), Processor(_value), *this;
  }
};


/// class to represent a boolean status
/// \tparam Processor callable object type which is called when the status is changed
template<auto Processor> class Status<bool, Processor> {
protected:
  bool _value{};
  mutable bool _changed{};
public:

  /// default constructor
  Status() noexcept = default;

  /// constructor with the initial value
  explicit Status(bool Value) noexcept : _value{Value} {}

  /// returns the current value of the status
  bool value() const noexcept { return _changed = false, _value; }

  /// checks if the Status is changed after the last call of `value()`
  bool changed() const noexcept { return _changed; }

  /// function calll operator which returns the current value of the status
  bool operator()() const noexcept { return value(); }

  /// assignment operator which changes the value of the status
  Status& operator=(bool Value) noexcept {
    return _changed = true, _value = Value, Processor(_value), *this;
  }
};

} // namespace yw
