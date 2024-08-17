/// \file color.hpp
/// \brief defines `yw::Color`

#pragma once

#include "vector.hpp"
#include "utility.hpp"

export namespace yw {

/// struct to represent a color
struct Color {

  struct Rgb;
  struct Hsv;

  static const Color BLACK, DIMGRAY, GRAY, DARKGRAY, SILVER, LIGHTGRAY, GAINSBORO, WHITESMOKE, WHITE, SNOW, GHOSTWHITE,
    FLORALWHITE, LINEN, ANTIQUEWHITE, PAPAYAWHIP, BLANCHEDALMOND, BISQUE, MOCCASIN, NAVAJOWHITE, PEACHPUFF, MISTYROSE,
    LAVENDERBLUSH, SEASHELL, OLDLACE, IVORY, HONEYDEW, MINTCREAM, AZURE, ALICEBLUE, LAVENDER, LIGHTSTEELBLUE,
    LIGHTSLATEGRAY, SLATEGRAY, STEELBLUE, ROYALBLUE, MIDNIGHTBLUE, NAVY, DARKBLUE, MEDIUMBLUE, BLUE, DODGERBLUE,
    CORNFLOWERBLUE, DEEPSKYBLUE, LIGHTSKYBLUE, SKYBLUE, LIGHTBLUE, POWDERBLUE, PALETURQUOISE, LIGHTCYAN, CYAN, AQUA,
    TURQUOISE, MEDIUMTURQUOISE, DARKTURQUOISE, LIGHTSEAGREEN, CADETBLUE, DARKCYAN, TEAL, DARKSLATEGRAY, DARKGREEN,
    GREEN, FORESTGREEN, SEAGREEN, MEDIUMSEAGREEN, MEDIUMAQUAMARINE, DARKSEAGREEN, AQUAMARINE, PALEGREEN, LIGHTGREEN,
    SPRINGGREEN, MEDIUMSPRINGGREEN, LAWNGREEN, CHARTREUSE, GREENYELLOW, LIME, LIMEGREEN, YELLOWGREEN, DARKOLIVEGREEN,
    OLIVEDRAB, OLIVE, DARKKHAKI, PALEGOLDENROD, CORNSILK, BEIGE, LIGHTYELLOW, LIGHTGOLDENRODYELLOW, LEMONCHIFFON, WHEAT,
    BURLYWOOD, TAN, KHAKI, YELLOW, GOLD, ORANGE, SANDYBROWN, DARKORANGE, GOLDENROD, PERU, DARKGOLDENROD, CHOCOLATE,
    SIENNA, SADDLEBROWN, MAROON, DARKRED, BROWN, FIREBRICK, INDIANRED, ROSYBROWN, DARKSALMON, LIGHTCORAL, SALMON,
    LIGHTSALMON, CORAL, TOMATO, ORANGERED, RED, CRIMSON, MEDIUMVIOLETRED, DEEPPINK, HOTPINK, PALEVIOLETRED, PINK,
    LIGHTPINK, THISTLE, MAGENTA, FUCHSIA, VIOLET, PLUM, ORCHID, MEDIUMORCHID, DARKORCHID, DARKVIOLET, DARKMAGENTA,
    PURPLE, INDIGO, DARKSLATEBLUE, BLUEVIOLET, MEDIUMPURPLE, SLATEBLUE, MEDIUMSLATEBLUE, TRANSPARENT, UNDEFINED, YW;

  fat4 r = 0.f; // Red [0..1]
  fat4 g = 0.f; // Blue [0..1]
  fat4 b = 0.f; // Green [0..1]
  fat4 a = 1.f; // Alpha [0..1]

  /// default constructor
  constexpr Color() noexcept = default;

  /// constructor with RGB values
  constexpr Color(numeric auto&& R, numeric auto&& G, numeric auto&& B)
    noexcept : r(fat4(R)), g(fat4(G)), b(fat4(B)) {}

  /// constructor with RGBA values
  constexpr Color(numeric auto&& R, numeric auto&& G, numeric auto&& B, numeric auto&& A)
    noexcept : r(fat4(R)), g(fat4(G)), b(fat4(B)), a(fat4(A)) {}

  /// for `yw::get`
  template<nat I> requires (I < 4) constexpr fat4& get()
    noexcept { return select_parameter<I>(r, g, b, a); }

  /// for `yw::get`
  template<nat I> requires (I < 4) constexpr fat4 get() const
    noexcept { return select_parameter<I>(r, g, b, a); }
};


/// struct to repreesnt a color in RGB format
struct Color::Rgb {

  fat4 r = 0.f; // Red [0..1]
  fat4 g = 0.f; // Blue [0..1]
  fat4 b = 0.f; // Green [0..1]
  fat4 a = 1.f; // Alpha [0..1]

  /// default constructor
  constexpr Rgb() noexcept = default;

  /// constructor with `yw::Color`
  constexpr Rgb(const Color& c) noexcept : r(c.r), g(c.g), b(c.b), a(c.a) {}

  /// conversion operator to `yw::Color`
  constexpr operator Color() const noexcept { return {r, g, b, a}; }

  /// constructor with RGB values
  constexpr Rgb(numeric auto&& R, numeric auto&& G, numeric auto&& B)
    noexcept : r(fat4(R)), g(fat4(G)), b(fat4(B)) {}

  /// constructor with RGBA values
  constexpr Rgb(numeric auto&& R, numeric auto&& G, numeric auto&& B, numeric auto&& A)
    noexcept : r(fat4(R)), g(fat4(G)), b(fat4(B)), a(fat4(A)) {}

  /// constructor with hex value
  explicit constexpr Rgb(nat4 xRRGGBB, fat4 A = 1.f)
    noexcept : Rgb(from_hex(xRRGGBB, A)) {}

  /// conversion operator to hex value
  explicit constexpr operator unsigned() const
    noexcept { return to_hex(*this); }

  /// constructor with `yw::XVector`
  explicit Rgb(const XVector& Xv) noexcept { _mm_storeu_ps(&r, Xv); }

  /// conversion operator to `yw::XVector`
  operator XVector() const noexcept { return xvload(&r); }

  /// for `yw::get`
  template<nat Ix> requires(Ix < 4) constexpr fat4& get()
    noexcept { return select_parameter<Ix>(r, g, b, a); }

  /// for `yw::get`
  template<nat Ix> requires(Ix < 4) constexpr fat4 get() const
    noexcept { return select_parameter<Ix>(r, g, b, a); }

private:

  static constexpr Rgb from_hex(const nat4 x, const fat4 A) noexcept {
    if constexpr (IS_CEV) return {fat4((0xff0000 & x) >> 16) / 255.f, fat4((0xff00 & x) >> 8) / 255.f, fat4(0xff & x) / 255.f, A};
    else return Rgb(xvinsert<3>(xvmul(XVCONSTANT<1.0 / 255.0>, _mm_cvtepi32_ps(_mm_cvtepu8_epi32(_mm_castps_si128(_mm_broadcast_ss((const fat4*)&x))))), A));
  }

  static constexpr nat4 to_hex(const Rgb& a) noexcept {
    if constexpr (IS_CEV) return (nat4)(a.r * 255.f) << 16 | (nat4)(a.g * 255.f) << 6 | (nat4)(a.b * 255.f) | (nat4)(a.a * 255.f) << 24;
    else return bitcast<nat4>(_mm_cvtsi128_si32(_mm_shuffle_epi8(_mm_cvtps_epi32(xvmul(xvload(&a.r), XVCONSTANT<255>)), XVCONSTANT<0, 0, 0, 0x03020100>)));
  }
};


/// struct to represent a color in HSV format
struct Color::Hsv {

  fat4 h = 0.f; // Hue [0..1]
  fat4 s = 0.f; // Saturation [0..1]
  fat4 v = 0.f; // Chroma [0..1]
  fat4 a = 1.f; // Alpha [0..1]

  /// default constructor
  constexpr Hsv() noexcept = default;

  /// constructor with `yw::Color`
  constexpr Hsv(const Color& c) noexcept : Hsv(from_rgb(c)) {}

  /// conversion operator to `yw::Color`
  constexpr operator Color() const noexcept { return to_rgb(*this); }

  /// constructor with `yw::Rgb`
  explicit constexpr Hsv(const Rgb& RGB) noexcept : Hsv(from_rgb(RGB)) {}

  /// conversion operator to `yw::Rgb`
  explicit constexpr operator Rgb() const noexcept { return to_rgb(*this); }

  /// constructor with HSV values
  constexpr Hsv(numeric auto&& H, numeric auto&& S, numeric auto&& V)
    noexcept : h(fat4(H)), s(fat4(S)), v(fat4(V)) {}

  /// constructor with HSVA values
  constexpr Hsv(numeric auto&& H, numeric auto&& S, numeric auto&& V, numeric auto&& A)
    noexcept : h(fat4(H)), s(fat4(S)), v(fat4(V)), a(fat4(A)) {}

private:

  /// conversion from `yw::Hsv` to `yw::Rgb`
  static constexpr Rgb to_rgb(const Hsv& HSV) noexcept {
    if (HSV.s == 0.f) return Rgb(HSV.v, HSV.v, HSV.v, HSV.a);
    else if (HSV.h < 0.5f) {
      if      (cev(2 / 6.f) < HSV.h) return Rgb(HSV.v - HSV.s, HSV.v, HSV.v - 3 * HSV.s + 6 * HSV.h * HSV.s, HSV.a);
      else if (cev(1 / 6.f) < HSV.h) return Rgb(HSV.v + HSV.s - 6 * HSV.h * HSV.s, HSV.v, HSV.v - HSV.s, HSV.a);
      else if (cev(0 / 6.f) < HSV.h) return Rgb(HSV.v, HSV.v - HSV.s + 6 * HSV.h * HSV.s, HSV.v - HSV.s, HSV.a);
      else return Rgb(0.f, 0.f, 0.f, HSV.a);
    } else {
      if      (HSV.h < cev(4.f / 6.f)) return Rgb(HSV.v - HSV.s, HSV.v + 3 * HSV.s - 6 * HSV.h * HSV.s, HSV.v, HSV.a);
      else if (HSV.h < cev(5.f / 6.f)) return Rgb(HSV.v - 5 * HSV.s + 6 * HSV.h * HSV.s, HSV.v - HSV.s, HSV.v, HSV.a);
      else if (HSV.h < 1.f)            return Rgb(HSV.v, HSV.v - HSV.s, HSV.v + 5 * HSV.s - 6 * HSV.h * HSV.s, HSV.a);
      else return Rgb(0.f, 0.f, 0.f, HSV.a);
    }
  }
  static constexpr Hsv from_rgb(const Rgb& RGB) noexcept {
    auto min = yw::mini(RGB.r, RGB.g, RGB.b);
    if (fat4 v = yw::max(RGB.r, RGB.g, RGB.b); v == min.second) return {0.f, 0.f, v, RGB.a};
    else if (fat4 s = v - min.second; min.first == 0) return {cev(1.0f / 6.0f) * ((RGB.b - RGB.g) / s + 3.f), s, v, RGB.a};
    else if (min.first == 1) return {cev(1.0f / 6.0f) * ((RGB.r - RGB.b) / s + 5.f), s, v, RGB.a};
    else return {cev(1.0f / 6.0f) * ((RGB.g - RGB.r) / s + 1.f), s, v, RGB.a};
  }
};

inline constexpr Color Color::BLACK = Color::Rgb(0x000000);
inline constexpr Color Color::DIMGRAY = Color::Rgb(0x696969);
inline constexpr Color Color::GRAY = Color::Rgb(0x808080);
inline constexpr Color Color::DARKGRAY = Color::Rgb(0xa9a9a9);
inline constexpr Color Color::SILVER = Color::Rgb(0xc0c0c0);
inline constexpr Color Color::LIGHTGRAY = Color::Rgb(0xd3d3d3);
inline constexpr Color Color::GAINSBORO = Color::Rgb(0xdcdcdc);
inline constexpr Color Color::WHITESMOKE = Color::Rgb(0xf5f5f5);
inline constexpr Color Color::WHITE = Color::Rgb(0xffffff);
inline constexpr Color Color::SNOW = Color::Rgb(0xfffafa);
inline constexpr Color Color::GHOSTWHITE = Color::Rgb(0xf8f8ff);
inline constexpr Color Color::FLORALWHITE = Color::Rgb(0xfffaf0);
inline constexpr Color Color::LINEN = Color::Rgb(0xfaf0e6);
inline constexpr Color Color::ANTIQUEWHITE = Color::Rgb(0xfaebd7);
inline constexpr Color Color::PAPAYAWHIP = Color::Rgb(0xffefd5);
inline constexpr Color Color::BLANCHEDALMOND = Color::Rgb(0xffebcd);
inline constexpr Color Color::BISQUE = Color::Rgb(0xffe4c4);
inline constexpr Color Color::MOCCASIN = Color::Rgb(0xffe4b5);
inline constexpr Color Color::NAVAJOWHITE = Color::Rgb(0xffdead);
inline constexpr Color Color::PEACHPUFF = Color::Rgb(0xffdab9);
inline constexpr Color Color::MISTYROSE = Color::Rgb(0xffe4e1);
inline constexpr Color Color::LAVENDERBLUSH = Color::Rgb(0xfff0f5);
inline constexpr Color Color::SEASHELL = Color::Rgb(0xfff5ee);
inline constexpr Color Color::OLDLACE = Color::Rgb(0xfdf5e6);
inline constexpr Color Color::IVORY = Color::Rgb(0xfffff0);
inline constexpr Color Color::HONEYDEW = Color::Rgb(0xf0fff0);
inline constexpr Color Color::MINTCREAM = Color::Rgb(0xf5fffa);
inline constexpr Color Color::AZURE = Color::Rgb(0xf0ffff);
inline constexpr Color Color::ALICEBLUE = Color::Rgb(0xf0f8ff);
inline constexpr Color Color::LAVENDER = Color::Rgb(0xe6e6fa);
inline constexpr Color Color::LIGHTSTEELBLUE = Color::Rgb(0xb0c4de);
inline constexpr Color Color::LIGHTSLATEGRAY = Color::Rgb(0x778899);
inline constexpr Color Color::SLATEGRAY = Color::Rgb(0x708090);
inline constexpr Color Color::STEELBLUE = Color::Rgb(0x4682b4);
inline constexpr Color Color::ROYALBLUE = Color::Rgb(0x4169e1);
inline constexpr Color Color::MIDNIGHTBLUE = Color::Rgb(0x191970);
inline constexpr Color Color::NAVY = Color::Rgb(0x000080);
inline constexpr Color Color::DARKBLUE = Color::Rgb(0x00008b);
inline constexpr Color Color::MEDIUMBLUE = Color::Rgb(0x0000cd);
inline constexpr Color Color::BLUE = Color::Rgb(0x0000ff);
inline constexpr Color Color::DODGERBLUE = Color::Rgb(0x1e90ff);
inline constexpr Color Color::CORNFLOWERBLUE = Color::Rgb(0x6495ed);
inline constexpr Color Color::DEEPSKYBLUE = Color::Rgb(0x00bfff);
inline constexpr Color Color::LIGHTSKYBLUE = Color::Rgb(0x87cefa);
inline constexpr Color Color::SKYBLUE = Color::Rgb(0x87ceeb);
inline constexpr Color Color::LIGHTBLUE = Color::Rgb(0xadd8e6);
inline constexpr Color Color::POWDERBLUE = Color::Rgb(0xb0e0e6);
inline constexpr Color Color::PALETURQUOISE = Color::Rgb(0xafeeee);
inline constexpr Color Color::LIGHTCYAN = Color::Rgb(0xe0ffff);
inline constexpr Color Color::CYAN = Color::Rgb(0x00ffff);
inline constexpr Color Color::AQUA = Color::Rgb(0x00ffff);
inline constexpr Color Color::TURQUOISE = Color::Rgb(0x40e0d0);
inline constexpr Color Color::MEDIUMTURQUOISE = Color::Rgb(0x48d1cc);
inline constexpr Color Color::DARKTURQUOISE = Color::Rgb(0x00ced1);
inline constexpr Color Color::LIGHTSEAGREEN = Color::Rgb(0x20b2aa);
inline constexpr Color Color::CADETBLUE = Color::Rgb(0x5f9ea0);
inline constexpr Color Color::DARKCYAN = Color::Rgb(0x008b8b);
inline constexpr Color Color::TEAL = Color::Rgb(0x008080);
inline constexpr Color Color::DARKSLATEGRAY = Color::Rgb(0x2f4f4f);
inline constexpr Color Color::DARKGREEN = Color::Rgb(0x006400);
inline constexpr Color Color::GREEN = Color::Rgb(0x008000);
inline constexpr Color Color::FORESTGREEN = Color::Rgb(0x228b22);
inline constexpr Color Color::SEAGREEN = Color::Rgb(0x2e8b57);
inline constexpr Color Color::MEDIUMSEAGREEN = Color::Rgb(0x3cb371);
inline constexpr Color Color::MEDIUMAQUAMARINE = Color::Rgb(0x66cdaa);
inline constexpr Color Color::DARKSEAGREEN = Color::Rgb(0x8fbc8f);
inline constexpr Color Color::AQUAMARINE = Color::Rgb(0x7fffd4);
inline constexpr Color Color::PALEGREEN = Color::Rgb(0x98fb98);
inline constexpr Color Color::LIGHTGREEN = Color::Rgb(0x90ee90);
inline constexpr Color Color::SPRINGGREEN = Color::Rgb(0x00ff7f);
inline constexpr Color Color::MEDIUMSPRINGGREEN = Color::Rgb(0x00fa9a);
inline constexpr Color Color::LAWNGREEN = Color::Rgb(0x7cfc00);
inline constexpr Color Color::CHARTREUSE = Color::Rgb(0x7fff00);
inline constexpr Color Color::GREENYELLOW = Color::Rgb(0xadff2f);
inline constexpr Color Color::LIME = Color::Rgb(0x00ff00);
inline constexpr Color Color::LIMEGREEN = Color::Rgb(0x32cd32);
inline constexpr Color Color::YELLOWGREEN = Color::Rgb(0x9acd32);
inline constexpr Color Color::DARKOLIVEGREEN = Color::Rgb(0x556b2f);
inline constexpr Color Color::OLIVEDRAB = Color::Rgb(0x6b8e23);
inline constexpr Color Color::OLIVE = Color::Rgb(0x808000);
inline constexpr Color Color::DARKKHAKI = Color::Rgb(0xbdb76b);
inline constexpr Color Color::PALEGOLDENROD = Color::Rgb(0xeee8aa);
inline constexpr Color Color::CORNSILK = Color::Rgb(0xfff8dc);
inline constexpr Color Color::BEIGE = Color::Rgb(0xf5f5dc);
inline constexpr Color Color::LIGHTYELLOW = Color::Rgb(0xffffe0);
inline constexpr Color Color::LIGHTGOLDENRODYELLOW = Color::Rgb(0xfafad2);
inline constexpr Color Color::LEMONCHIFFON = Color::Rgb(0xfffacd);
inline constexpr Color Color::WHEAT = Color::Rgb(0xf5deb3);
inline constexpr Color Color::BURLYWOOD = Color::Rgb(0xdeb887);
inline constexpr Color Color::TAN = Color::Rgb(0xd2b48c);
inline constexpr Color Color::KHAKI = Color::Rgb(0xf0e68c);
inline constexpr Color Color::YELLOW = Color::Rgb(0xffff00);
inline constexpr Color Color::GOLD = Color::Rgb(0xffd700);
inline constexpr Color Color::ORANGE = Color::Rgb(0xffa500);
inline constexpr Color Color::SANDYBROWN = Color::Rgb(0xf4a460);
inline constexpr Color Color::DARKORANGE = Color::Rgb(0xff8c00);
inline constexpr Color Color::GOLDENROD = Color::Rgb(0xdaa520);
inline constexpr Color Color::PERU = Color::Rgb(0xcd853f);
inline constexpr Color Color::DARKGOLDENROD = Color::Rgb(0xb8860b);
inline constexpr Color Color::CHOCOLATE = Color::Rgb(0xd2691e);
inline constexpr Color Color::SIENNA = Color::Rgb(0xa0522d);
inline constexpr Color Color::SADDLEBROWN = Color::Rgb(0x8b4513);
inline constexpr Color Color::MAROON = Color::Rgb(0x800000);
inline constexpr Color Color::DARKRED = Color::Rgb(0x8b0000);
inline constexpr Color Color::BROWN = Color::Rgb(0xa52a2a);
inline constexpr Color Color::FIREBRICK = Color::Rgb(0xb22222);
inline constexpr Color Color::INDIANRED = Color::Rgb(0xcd5c5c);
inline constexpr Color Color::ROSYBROWN = Color::Rgb(0xbc8f8f);
inline constexpr Color Color::DARKSALMON = Color::Rgb(0xe9967a);
inline constexpr Color Color::LIGHTCORAL = Color::Rgb(0xf08080);
inline constexpr Color Color::SALMON = Color::Rgb(0xfa8072);
inline constexpr Color Color::LIGHTSALMON = Color::Rgb(0xffa07a);
inline constexpr Color Color::CORAL = Color::Rgb(0xff7f50);
inline constexpr Color Color::TOMATO = Color::Rgb(0xff6347);
inline constexpr Color Color::ORANGERED = Color::Rgb(0xff4500);
inline constexpr Color Color::RED = Color::Rgb(0xff0000);
inline constexpr Color Color::CRIMSON = Color::Rgb(0xdc143c);
inline constexpr Color Color::MEDIUMVIOLETRED = Color::Rgb(0xc71585);
inline constexpr Color Color::DEEPPINK = Color::Rgb(0xff1493);
inline constexpr Color Color::HOTPINK = Color::Rgb(0xff69b4);
inline constexpr Color Color::PALEVIOLETRED = Color::Rgb(0xdb7093);
inline constexpr Color Color::PINK = Color::Rgb(0xffc0cb);
inline constexpr Color Color::LIGHTPINK = Color::Rgb(0xffb6c1);
inline constexpr Color Color::THISTLE = Color::Rgb(0xd8bfd8);
inline constexpr Color Color::MAGENTA = Color::Rgb(0xff00ff);
inline constexpr Color Color::FUCHSIA = Color::Rgb(0xff00ff);
inline constexpr Color Color::VIOLET = Color::Rgb(0xee82ee);
inline constexpr Color Color::PLUM = Color::Rgb(0xdda0dd);
inline constexpr Color Color::ORCHID = Color::Rgb(0xda70d6);
inline constexpr Color Color::MEDIUMORCHID = Color::Rgb(0xba55d3);
inline constexpr Color Color::DARKORCHID = Color::Rgb(0x9932cc);
inline constexpr Color Color::DARKVIOLET = Color::Rgb(0x9400d3);
inline constexpr Color Color::DARKMAGENTA = Color::Rgb(0x8b008b);
inline constexpr Color Color::PURPLE = Color::Rgb(0x800080);
inline constexpr Color Color::INDIGO = Color::Rgb(0x4b0082);
inline constexpr Color Color::DARKSLATEBLUE = Color::Rgb(0x483d8b);
inline constexpr Color Color::BLUEVIOLET = Color::Rgb(0x8a2be2);
inline constexpr Color Color::MEDIUMPURPLE = Color::Rgb(0x9370db);
inline constexpr Color Color::SLATEBLUE = Color::Rgb(0x6a5acd);
inline constexpr Color Color::MEDIUMSLATEBLUE = Color::Rgb(0x7b68ee);
inline constexpr Color Color::TRANSPARENT = Color::Rgb(0x00000000);
inline constexpr Color Color::UNDEFINED = Color(0, 0, 0, -1);
inline constexpr Color Color::YW = Color::Rgb(0x081020);
} // namespace yw


namespace std {

// tuple

template<> struct tuple_size<yw::Color> : integral_constant<size_t, 4> {};
template<> struct tuple_size<yw::Color::Rgb> : integral_constant<size_t, 4> {};
template<> struct tuple_size<yw::Color::Hsv> : integral_constant<size_t, 4> {};

template<size_t I> requires (I < 4) struct tuple_element<I, yw::Color> : type_identity<float> {};
template<size_t I> requires (I < 4) struct tuple_element<I, yw::Color::Rgb> : type_identity<float> {};
template<size_t I> requires (I < 4) struct tuple_element<I, yw::Color::Hsv> : type_identity<float> {};

} // namespace std
