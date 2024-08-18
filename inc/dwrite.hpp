/// \file dwrite.hpp
/// \brief DirectWrite header file

#pragma once

// #ifndef YWLIB_DEFAULT_TEXT_FORMAT
// #define YWLIB_DEFAULT_TEXT_FORMAT L"Yu Gothic UI"
// #endif

#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

// #include "comptr.hpp"
// #include "windows.hpp"

export namespace yw::win {


// enum ////////////////////////////////////////////////////////////////////////


using DWRITE_FACTORY_TYPE = ::DWRITE_FACTORY_TYPE;
using DWRITE_FONT_WEIGHT = ::DWRITE_FONT_WEIGHT;
using DWRITE_FONT_STYLE = ::DWRITE_FONT_STYLE;
using DWRITE_FONT_STRETCH = ::DWRITE_FONT_STRETCH;


//////////////////////////////////////////////////////////////////////// enum //
// class ///////////////////////////////////////////////////////////////////////


using IDWriteFactory = ::IDWriteFactory;
using IDWriteTextFormat = ::IDWriteTextFormat;


/////////////////////////////////////////////////////////////////////// class //
// function ////////////////////////////////////////////////////////////////////


template<typename Factory>
inline sl_t DWriteCreateFactory(DWRITE_FACTORY_TYPE type, Factory** factory) {
  return ::DWriteCreateFactory(type, __uuidof(Factory), (IUnknown**)factory);
}


//////////////////////////////////////////////////////////////////// function //

} ////////////////////////////////////////////////////////////////////////////// namespace yw::win

// export namespace yw {


// /// DirectWrite factory
// inline ComPtr<win::IDWriteFactory> dwrite_factory = []{
//   win::IDWriteFactory* factory;
//   auto hr = ::DWriteCreateFactory(
//     DWRITE_FACTORY_TYPE_SHARED, __uuidof(win::IDWriteFactory), (IUnknown**)&factory);
//   if (FAILED(hr)) {
//     auto text = cvt<cat1>(hresult_message(hr));
//     text = std::format("Failed to create DirectWrite factory: {}", text);
//     logger.error(text);
//     std::cerr << text << std::endl;
//     return ComPtr<win::IDWriteFactory>();
//   } else return ComPtr<win::IDWriteFactory>(factory);
// }();


// /// default text format
// inline ComPtr<win::IDWriteTextFormat> default_text_format = []{
//   if (!dwrite_factory) return ComPtr<win::IDWriteTextFormat>();
//   win::IDWriteTextFormat* format;
//   auto hr = dwrite_factory->CreateTextFormat(
//     YWLIB_DEFAULT_TEXT_FORMAT, nullptr, DWRITE_FONT_WEIGHT_NORMAL,
//     DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"", &format);
//   if (FAILED(hr)) {
//     auto text = cvt<cat1>(hresult_message(hr));
//     text = std::format("Failed to create default text format: {}", text);
//     logger.error(text);
//     std::cerr << text << std::endl;
//     return ComPtr<win::IDWriteTextFormat>();
//   } else return ComPtr<win::IDWriteTextFormat>(format);
// }();


// /// obtains the font family name of the text format
// inline str2 get_font_family_name(win::IDWriteTextFormat* format) {
//   if (!format) return str2{};
//   auto length = format->GetFontFamilyNameLength() + 1;
//   auto buffer = str2(length, 0);
//   auto hr = format->GetFontFamilyName(buffer.data(), length);
//   if (FAILED(hr)) {
//     auto text = cvt<cat1>(hresult_message(hr));
//     text = std::format("Failed to get font family name: {}", text);
//     logger.error(text);
//     std::cerr << text << std::endl;
//     return str2{};
//   } else return buffer;
// }

// } // namespace yw
