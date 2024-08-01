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

export namespace yw::win {


/// window handle type
using HWND = ::HWND;


/// command line arguments; argv[0] is the program name
inline Array<str1> argv = [] {
  int argc;
  auto a = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  Array<str1> r(argc);
  for (; 0 <= --argc;) r[argc] = cvt<cat1>(a[argc]);
  return r;
}();


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
