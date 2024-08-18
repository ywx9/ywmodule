/// \file comptr.hpp
/// \brief COM smart pointer header file

#pragma once

#ifndef YWLIB
#include <memory>
#else
import std;
#endif

#include "exception.hpp"

export namespace yw {

/// deleter for COM smart pointer
template<typename Com> struct ComDeleter {

  /// default constructor
  constexpr ComDeleter() noexcept = default;

  /// constructor from other deleter
  template<typename Other>
  constexpr ComDeleter(const ComDeleter<Other>&) noexcept {}

  /// function call operator to release COM object
  constexpr void operator()(Com* ptr) const noexcept {
    if (ptr) ptr->Release();
  }
};

/// smart pointer for COM object
template<typename Com>
class ComPtr : public std::unique_ptr<Com, ComDeleter<Com>> {

  /// base class
  using base = std::unique_ptr<Com, ComDeleter<Com>>;

public:
  using base::unique_ptr;
  using base::operator=;
  using base::operator bool;

  /// implicit conversion to raw pointer
  constexpr operator Com*() const noexcept {
    return this->get();
  }

  /// obtains the address of the pointer for initialization
  template<invocable<Com*&> Fn> constexpr invoke_result<Fn, Com*&>
  init(Fn&& initializer, const Source& _ = {}) & {
    if (*this) throw Exception("COM object already initialized", _);
    Com* ptr = nullptr;
    auto r = initializer(ptr);
    if (!ptr) throw Exception("Failed to initialize COM object", _);
    this->reset(ptr);
    return r;
  }

  /// reinterprets the pointer type to another COM type
  template<typename Other> sl_t as(ComPtr<Other>& other) const {
    return this->get()->QueryInterface(__uuidof(Other), (void**)other.init());
  }
};

}
