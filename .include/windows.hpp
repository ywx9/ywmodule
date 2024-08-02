#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#undef max
#undef min
#undef ERROR

#include "array.hpp"
#include "string.hpp"
#include "logger.hpp"

export namespace yw {

/// window handle
using HWND = ::HWND;

/// command line arguments; argv[0] is the program name
inline const Array<str1> argv = [] {
  int argc;
  auto a = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  Array<str1> r(argc);
  for (; 0 <= --argc;) r[argc] = cvt<cat1>(a[argc]);
  return r;
}();


/// default logger
inline Logger logger{Path(argv[0]).replace_extension(".log")};


/// stopwatch class
class Stopwatch {

  inline static const fat8 FREQ = [](LARGE_INTEGER _) {
    return ::QueryPerformanceFrequency(&_), fat8(_.QuadPart); }({});

  mutable LARGE_INTEGER li;
  int8 last;

public:

  /// default constructor; initializes the stopwatch
  Stopwatch() noexcept : li{}, last{(::QueryPerformanceCounter(&li), li.QuadPart)} {}

  /// function call operator which returns the elapsed time in seconds
  fat8 operator()() const noexcept { return *this; }

  /// conversion operator which returns the elapsed time in seconds
  operator fat8() const noexcept {
    return ::QueryPerformanceCounter(&li), (li.QuadPart - last) / FREQ;
  }

  /// conversion operator to `fat4` which is the elapsed time in seconds
  explicit operator fat4() const noexcept { return fat4(operator fat8()); }

  /// resets the stopwatch to zero
  void reset() noexcept { ::QueryPerformanceCounter(&li), last = li.QuadPart; }

  /// returns the elapsed time in seconds and resets the stopwatch to zero
  fat8 split() noexcept {
    ::QueryPerformanceCounter(&li);
    return (li.QuadPart - exchange(last, li.QuadPart)) / FREQ;
  }
};


/// displays a message box with a `OK` button
/// \param Window (optional) window handle
/// \param Text message text
/// \param Caption (optional) message caption
/// \return `true` if the user clicks the `OK` button
inline constexpr Overload ok{
  [](const str1& Text, const str1& Caption = "OK?") -> bool {
    return ::MessageBoxA(0, Text.data(), Caption.data(), MB_ICONEXCLAMATION | MB_OK) == IDOK;
  },
  [](const str2& Text, const str2& Caption = L"OK?") -> bool {
    return ::MessageBoxW(nullptr, Text.c_str(), Caption.c_str(), MB_ICONEXCLAMATION | MB_OK) == IDOK;
  },
  [](const HWND Window, const str1& Text, const str1& Caption = "OK?") -> bool {
    return ::MessageBoxA(Window, Text.c_str(), Caption.c_str(), MB_ICONEXCLAMATION | MB_OK) == IDOK;
  },
  [](const HWND Window, const str2& Text, const str2& Caption = L"OK?") -> bool {
    return ::MessageBoxW(Window, Text.c_str(), Caption.c_str(), MB_ICONEXCLAMATION | MB_OK) == IDOK;
  }
};

/// displays a message box with `Yes` and `No` buttons
/// \param Window (optional) window handle
/// \param Text message text
/// \param Caption (optional) message caption
/// \return `true` if the user clicks the `Yes` button; otherwise, `false`
inline constexpr Overload yes{
  [](const str1& Text, const str1& Caption = "Yes?") -> bool {
    return ::MessageBoxA(nullptr, Text.c_str(), Caption.c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES;
  },
  [](const str2& Text, const str2& Caption = L"Yes?") -> bool {
    return ::MessageBoxW(nullptr, Text.c_str(), Caption.c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES;
  },
  [](const HWND Window, const str1& Text, const str1& Caption = "Yes?") -> bool {
    return ::MessageBoxA(Window, Text.c_str(), Caption.c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES;
  },
  [](const HWND Window, const str2& Text, const str2& Caption = L"Yes?") -> bool {
    return ::MessageBoxW(Window, Text.c_str(), Caption.c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES;
  }
};


/// user-defined procedure to hack the dfault window procedure
/// \return `true` if the message is processed; otherwise, `false`
inline bool (*userproc)(HWND hw, nat4 msg, nat8 wp, int8 lp) = nullptr;

/// list of dropped files
inline Status<Array<Path>> dropped_files{};

/// terminates the system
void terminate();

} // namespace yw


export namespace yw::key {

class Key {
public:
  const bool down;
  const bool changed;
};

/// key status
inline Status<bool> keys[256];

inline auto& lbutton = keys[VK_LBUTTON], &rbutton = keys[VK_RBUTTON], &mbutton = keys[VK_MBUTTON];
inline auto& xbutton1 = keys[VK_XBUTTON1], &xbutton2 = keys[VK_XBUTTON2];
inline auto& win = keys[VK_LWIN], &app = keys[VK_APPS], &back = keys[VK_BACK], &tab = keys[VK_TAB], &enter = keys[VK_RETURN];
inline auto& escape = keys[VK_ESCAPE], &shift = keys[VK_SHIFT], &control = keys[VK_CONTROL], &alt = keys[VK_MENU], &space = keys[VK_SPACE];
inline auto& pageup = keys[VK_PRIOR], &pagedown = keys[VK_NEXT], &end = keys[VK_END], &home = keys[VK_HOME];
inline auto& left = keys[VK_LEFT], &up = keys[VK_UP], &right = keys[VK_RIGHT], &down = keys[VK_DOWN];
inline auto& screenshot = keys[VK_SNAPSHOT], &insert = keys[VK_INSERT], &delete_ = keys[VK_DELETE];
inline auto& n0 = keys['0'], &n1 = keys['1'], &n2 = keys['2'], &n3 = keys['3'], &n4 = keys['4'], &n5 = keys['5'], &n6 = keys['6'], &n7 = keys['7'], &n8 = keys['8'], &n9 = keys['9'];
inline auto& a = keys['A'], &b = keys['B'], &c = keys['C'], &d = keys['D'], &e = keys['E'], &f = keys['F'], &g = keys['G'], &h = keys['H'], &i = keys['I'];
inline auto& j = keys['J'], &k = keys['K'], &l = keys['L'], &m = keys['M'], &n = keys['N'], &o = keys['O'], &p = keys['P'], &q = keys['Q'], &r = keys['R'];
inline auto& s = keys['S'], &t = keys['T'], &u = keys['U'], &v = keys['V'], &w = keys['W'], &x = keys['X'], &y = keys['Y'], &z = keys['Z'];
inline auto& np_0 = keys[VK_NUMPAD0], &np_1 = keys[VK_NUMPAD1], &np_2 = keys[VK_NUMPAD2], &np_3 = keys[VK_NUMPAD3], &np_4 = keys[VK_NUMPAD4];
inline auto& np_5 = keys[VK_NUMPAD5], &np_6 = keys[VK_NUMPAD6], &np_7 = keys[VK_NUMPAD7], &np_8 = keys[VK_NUMPAD8], &np_9 = keys[VK_NUMPAD9];
inline auto& np_mul = keys[VK_MULTIPLY], &np_add = keys[VK_ADD], &np_sub = keys[VK_SUBTRACT], &np_div = keys[VK_DIVIDE], &np_decimal = keys[VK_DECIMAL];
inline auto& f1 = keys[VK_F1], &f2 = keys[VK_F2], &f3 = keys[VK_F3], &f4 = keys[VK_F4], &f5 = keys[VK_F5], &f6 = keys[VK_F6];
inline auto& f7 = keys[VK_F7], &f8 = keys[VK_F8], &f9 = keys[VK_F9], &f10 = keys[VK_F10], &f11 = keys[VK_F11], &f12 = keys[VK_F12];
inline auto& minus = keys[VK_OEM_MINUS], &plus = keys[VK_OEM_PLUS], &comma = keys[VK_OEM_COMMA], &period = keys[VK_OEM_PERIOD];

} // namespace yw::key


export namespace yw::win {

/// window handle type
using HWND = ::HWND;

/// reinterprets `HRESULT` and throws an exception if failed
inline void tiff(HRESULT hr, const Source& _ = {}) {
  if (SUCCEEDED(hr)) return;
  str2 buffer(256, 0);
  ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   buffer.data(), nat4(buffer.size()), nullptr);
  auto text = cvt<cat1>(buffer);
  logger.debug(text, _);
  text = std::format("HRESULT({:08x}) error: {} ({} at {}({},{}))",
                      hr, text, _.func, _.file, _.line, _.column);
  throw std::runtime_error(text.c_str());
}

/// window message type
enum class WindowMessage : nat4 {
  COPYDATA = WM_COPYDATA,
};


/// obtains the text of a window
str2 get_text(HWND hw) {
  str2 r(::GetWindowTextLengthW(hw), 0);
  ::GetWindowTextW(hw, r.data(), int(r.size() + 1));
  return r;
}


/// obtains the handle of a window
HWND get_handle(const str2& WindowTitle) {
  struct Data {
    const str2* WindowTitle;
    HWND hw;
  };
  WNDENUMPROC callback = [](HWND hw, int8 lp) -> BOOL {
    auto d = reinterpret_cast<Data*>(lp);
    return get_text(hw) == *d->WindowTitle ? (d->hw = hw, FALSE) : TRUE;
  };
  Data d{&WindowTitle, nullptr};
  ::EnumWindows(callback, reinterpret_cast<int8>(&d));
  return d.hw;
}


/// sends a message to a window
int8 send_message(HWND hw, WindowMessage msg, nat8 wp, int8 lp) {
  return ::SendMessageW(hw, nat4(msg), wp, lp);
}


/// sends data to a window by `WM_COPYDATA`
int8 send_data(HWND hw, cnt_range auto&& r) {
  ::COPYDATASTRUCT cds{0, nat4(yw::size(r) * sizeof(iter_value<decltype(r)>)), (void*)yw::data(r)};
  return ::SendMessageW(hw, nat4(WindowMessage::COPYDATA), 0, (int8)&cds);
}

} // namespace yw::win
