#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#include <wingdi.h>
#pragma comment(lib, "gdi32.lib")
#undef max
#undef min
#undef ERROR
#undef CreateWindow
#undef DefWindowProc
#undef DispatchMessage
#undef FormatMessage
#undef GetCommandLine
#undef GetMessage
#undef GetModuleHandle
#undef GetObject
#undef GetWindowText
#undef GetWindowTextLength
#undef LoadCursor
#undef PeekMessage
#undef PostMessage
#undef MessageBox
#undef RegisterClass
#undef SendMessage
#undef SetWindowText

#include "array.hpp"
#include "exception.hpp"
#include "string.hpp"
#include "logger.hpp"
#include "vector.hpp"
#include "file.hpp"

/// namespace for Windows API
export namespace yw::win {

using HBRUSH = ::HBRUSH;
using HCURSOR = ::HCURSOR;
using HGDIOBJ = ::HGDIOBJ;
using HFONT = ::HFONT;
using HICON = ::HICON;
using HINSTANCE = ::HINSTANCE;
using HMENU = ::HMENU;
using HWND = ::HWND;

using LARGE_INTEGER = ::LARGE_INTEGER;
using LOGFONT = ::LOGFONTW;
using MSG = ::MSG;
using POINT = ::POINT;
using RECT = ::RECT;
using WNDCLASS = ::WNDCLASSEXW;
using WNDPROC = ::WNDPROC;

inline int4 AdjustWindowRect(RECT* Rect, nat4 Style, int4 Menu, nat4 ExStyle) {
  return ::AdjustWindowRectEx(Rect, Style, Menu, ExStyle);
}

inline int4 ClientToScreen(HWND Window, POINT* Point) {
  return ::ClientToScreen(Window, Point);
}

inline int4 ClipCursor(const RECT* Rect = nullptr) {
  return ::ClipCursor(Rect);
}

inline cat2** CommandLineToArgv(const cat2* CommandLine, int* argc) {
  return ::CommandLineToArgvW(CommandLine, argc);
}

inline HWND CreateWindow(
  nat4 ExStyle, const cat2* ClassName, const cat2* WindowName, nat4 Style,
  int4 X, int4 Y, int4 Width, int4 Height,
  HWND Parent, HMENU Menu, HINSTANCE Instance, void* Param) {
  return ::CreateWindowExW(
    ExStyle, ClassName, WindowName, Style,
    X, Y, Width, Height, Parent, Menu, Instance, Param);
}

inline int8 DefWindowProc(HWND Window, nat4 Message, nat8 WParam, int8 LParam) {
  return ::DefWindowProcW(Window, Message, WParam, LParam);
}

inline int4 DestroyWindow(HWND Window) {
  return ::DestroyWindow(Window);
}

inline int8 DispatchMessage(const MSG* Message) {
  return ::DispatchMessageW(Message);
}

inline nat4 FormatMessage(
  nat4 Flags, const void* Source, nat4 MessageId, nat4 LanguageId,
  cat2* Buffer, nat4 Size, va_list* Arguments) {
  return ::FormatMessageW(Flags, Source, MessageId, LanguageId, Buffer, Size, Arguments);
}

inline const cat2* GetCommandLine() {
  return ::GetCommandLineW();
}

inline int4 GetCursorPos(POINT* Point) {
  return ::GetCursorPos(Point);
}

inline HWND GetDesktopWindow() {
  return ::GetDesktopWindow();
}

inline nat4 GetLastError() {
  return ::GetLastError();
}

inline int4 GetMessage(MSG* Message, HWND Window, nat4 Min, nat4 Max) {
  return ::GetMessageW(Message, Window, Min, Max);
}

inline HINSTANCE GetModuleHandle(const cat2* ModuleName) {
  return ::GetModuleHandleW(ModuleName);
}

inline int4 GetObject(HGDIOBJ Object, int4 BufferSize, void* Buffer) {
  return ::GetObjectW(Object, BufferSize, Buffer);
}

inline HGDIOBJ GetStockObject(int4 Object) {
  return ::GetStockObject(Object);
}

inline int4 GetSystemMetrics(int4 Index) {
  return ::GetSystemMetrics(Index);
}

inline int4 GetWindowRect(HWND Window, RECT* Rect) {
  return ::GetWindowRect(Window, Rect);
}

inline int4 GetWindowText(HWND Window, cat2* Buffer, int4 Length) {
  return ::GetWindowTextW(Window, Buffer, Length);
}

inline int4 GetWindowTextLength(HWND Window) {
  return ::GetWindowTextLengthW(Window);
}

inline HCURSOR LoadCursor(HINSTANCE Instance, const cat2* CursorName) {
  return ::LoadCursorW(Instance, CursorName);
}

inline int4 PeekMessage(MSG* Message, HWND Window, nat4 Min, nat4 Max, nat4 Remove) {
  return ::PeekMessageW(Message, Window, Min, Max, Remove);
}

inline int4 MessageBox(HWND Window, const cat2* Text, const cat2* Caption, nat4 Type) {
  return ::MessageBoxW(Window, Text, Caption, Type);
}

inline int4 PostMessage(HWND Window, nat4 Message, nat8 WParam, int8 LParam) {
  return ::PostMessageW(Window, Message, WParam, LParam);
}

inline void PostQuitMessage(int4 ExitCode) {
  ::PostQuitMessage(ExitCode);
}

inline int4 QueryPerformanceCounter(LARGE_INTEGER* PerformanceCount) {
  return ::QueryPerformanceCounter(PerformanceCount);
}

inline int4 QueryPerformanceFrequency(LARGE_INTEGER* Frequency) {
  return ::QueryPerformanceFrequency(Frequency);
}

inline nat2 RegisterClass(const WNDCLASS* WndClass) {
  return ::RegisterClassExW(WndClass);
}

inline int4 ScreenToClient(HWND Window, POINT* Point) {
  return ::ScreenToClient(Window, Point);
}

inline int8 SendMessage(HWND Window, nat4 Message, nat8 WParam, int8 LParam) {
  return ::SendMessageW(Window, Message, WParam, LParam);
}

inline int4 SetCursorPos(int4 X, int4 Y) {
  return ::SetCursorPos(X, Y);
}

inline HWND SetFocus(HWND Window) {
  return ::SetFocus(Window);
}

inline int4 SetWindowText(HWND Window, const cat2* Text) {
  return ::SetWindowTextW(Window, Text);
}

inline int4 ShowWindow(HWND Window, int4 Command) {
  return ::ShowWindow(Window, Command);
}

inline int4 TranslateMessage(const MSG* Message) {
  return ::TranslateMessage(Message);
}

inline int4 UpdateWindow(HWND Window) {
  return ::UpdateWindow(Window);
}

} // namespace yw::win /////////////////////////////////////////////////////////


export namespace yw {


/// instance handle
// inline const win::HINSTANCE INSTANCE_HANDLE = [] {
//   if (auto hi = win::GetModuleHandle(nullptr); !hi) {
//     logger.fatal("Failed to initialize the instance handle");
//     std::cerr << "Failed to initialize the instance handle" << std::endl;
//     return win::HINSTANCE{};
//   } else return hi;
// }();


/// default font handle
inline const win::HFONT SYSTEM_FONT_HANDLE = [] {
  if (auto hf = (win::HFONT)win::GetStockObject(SYSTEM_FONT); !hf) {
    logger.fatal("Failed to initialize the default font handle");
    std::cerr << "Failed to initialize the default font handle" << std::endl;
    return win::HFONT{};
  } else return hf;
}();


/// default font size in pixels
inline const fat4 DEFAULT_FONT_SIZE = [] {
  win::LOGFONT lf;
  win::GetObject(SYSTEM_FONT_HANDLE, sizeof(lf), &lf);
  return std::abs(lf.lfHeight) * cev(96.f / 72.f) + 2;
}();


/// obtains `HRESULT` message
inline str2 hresult_message(HRESULT hr) noexcept {
  str2 buffer(256, 0);
  win::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     buffer.data(), nat4(buffer.size()), nullptr);
  buffer.resize(strlen(buffer.data()));
  return buffer;
}


/// reinterprets `HRESULT` and throws an exception if failed
inline void tiff(HRESULT hr, const Source& _ = {}) {
  if (SUCCEEDED(hr)) return;
  str2 buffer(256, 0);
  win::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                     nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                     buffer.data(), nat4(buffer.size()), nullptr);
  auto text = cvt<cat1>(buffer);
  text = std::format("{} (0x{:08x})", text, nat4(hr));
  logger.debug(text, _);
  throw Exception(text.c_str(), _);
}


/// stopwatch class
class Stopwatch {

  inline static const fat8 FREQ = [](LARGE_INTEGER _) {
    return win::QueryPerformanceFrequency(&_), fat8(_.QuadPart); }({});

  mutable LARGE_INTEGER li;
  int8 last;

public:

  /// default constructor; initializes the stopwatch
  Stopwatch() noexcept : li{}, last{(win::QueryPerformanceCounter(&li), li.QuadPart)} {}

  /// function call operator which returns the elapsed time in seconds
  fat8 operator()() const noexcept { return *this; }

  /// conversion operator which returns the elapsed time in seconds
  operator fat8() const noexcept {
    return win::QueryPerformanceCounter(&li), (li.QuadPart - last) / FREQ;
  }

  /// conversion operator to `fat4` which is the elapsed time in seconds
  explicit operator fat4() const noexcept { return fat4(operator fat8()); }

  /// resets the stopwatch to zero
  void reset() noexcept { win::QueryPerformanceCounter(&li), last = li.QuadPart; }

  /// returns the elapsed time in seconds and resets the stopwatch to zero
  fat8 split() noexcept {
    win::QueryPerformanceCounter(&li);
    return (li.QuadPart - exchange(last, li.QuadPart)) / FREQ;
  }
};


/// displays a message box with a `OK` button
/// \param Text message text
/// \param Caption (optional) message caption
/// \return `true` if the user clicks the `OK` button
constexpr bool ok(stringable auto&& Text, stringable auto&& Caption) {
  auto text = cvt<cat2>(Text), caption = cvt<cat2>(Caption);
  return win::MessageBox(nullptr, text.c_str(), caption.c_str(), MB_ICONEXCLAMATION | MB_OK) == IDOK;
}

/// displays a message box with a `OK` button
/// \param Text message text
/// \return `true` if the user clicks the `OK` button
constexpr bool ok(stringable auto&& Text) { return ok(Text, "OK?"); }


/// displays a message box with `Yes` and `No` buttons
/// \param Text message text
/// \param Caption (optional) message caption
/// \return `true` if the user clicks the `Yes` button; otherwise, `false`
constexpr bool yes(stringable auto&& Text, stringable auto&& Caption) {
  auto text = cvt<cat2>(Text), caption = cvt<cat2>(Caption);
  return win::MessageBox(nullptr, text.c_str(), caption.c_str(), MB_ICONQUESTION | MB_YESNO) == IDYES;
}

/// displays a message box with `Yes` and `No` buttons
/// \param Text message text
/// \return `true` if the user clicks the `Yes` button; otherwise, `false`
constexpr bool yes(stringable auto&& Text) { return yes(Text, "Yes?"); }


/// obtains the cursor position
inline Vector get_cursor_pos(HWND hw = nullptr) {
  POINT pt;
  win::GetCursorPos(&pt);
  if (hw) win::ScreenToClient(hw, &pt);
  return {pt.x, pt.y};
}


/// obtains the font information (Windows API)
inline win::LOGFONT get_logfont(HFONT hf) {
  win::LOGFONT lf;
  auto n = win::GetObject(hf, sizeof(lf), &lf);
  if (!n) tiff(HRESULT_FROM_WIN32(win::GetLastError()));
  return lf;
}


/// obtains the size of the virtaul screen
inline Vector get_window_size() {
  return {win::GetSystemMetrics(SM_CXVIRTUALSCREEN),
          win::GetSystemMetrics(SM_CYVIRTUALSCREEN)};
}


/// obtains the size of a window
/// \param hw window handle
/// \return `{width, height, 0, 0}`
inline Vector get_window_size(HWND hw) {
  win::RECT r;
  win::GetWindowRect(hw, &r);
  return {r.right - r.left, r.bottom - r.top};
}


/// obtains the text of a window
str2 get_window_text(HWND hw) {
  str2 r(win::GetWindowTextLength(hw), 0);
  win::GetWindowText(hw, r.data(), int(r.size() + 1));
  return r;
}



/// user-defined procedure to hack the dfault window procedure
/// \return `true` if the message is processed; otherwise, `false`
// inline bool (*userproc)(HWND hw, nat4 msg, nat8 wp, int8 lp) = nullptr;

/// list of dropped files
// inline Status<Array<Path>> dropped_files{};

/// terminates the system
// void terminate();

} // namespace yw


export namespace yw::key {

class Key {
public:
  const bool down;
  const bool changed;
};

/// key status
// inline Status<bool> keys[256];

// inline auto& lbutton = keys[VK_LBUTTON], &rbutton = keys[VK_RBUTTON], &mbutton = keys[VK_MBUTTON];
// inline auto& xbutton1 = keys[VK_XBUTTON1], &xbutton2 = keys[VK_XBUTTON2];
// inline auto& win = keys[VK_LWIN], &app = keys[VK_APPS], &back = keys[VK_BACK], &tab = keys[VK_TAB], &enter = keys[VK_RETURN];
// inline auto& escape = keys[VK_ESCAPE], &shift = keys[VK_SHIFT], &control = keys[VK_CONTROL], &alt = keys[VK_MENU], &space = keys[VK_SPACE];
// inline auto& pageup = keys[VK_PRIOR], &pagedown = keys[VK_NEXT], &end = keys[VK_END], &home = keys[VK_HOME];
// inline auto& left = keys[VK_LEFT], &up = keys[VK_UP], &right = keys[VK_RIGHT], &down = keys[VK_DOWN];
// inline auto& screenshot = keys[VK_SNAPSHOT], &insert = keys[VK_INSERT], &delete_ = keys[VK_DELETE];
// inline auto& n0 = keys['0'], &n1 = keys['1'], &n2 = keys['2'], &n3 = keys['3'], &n4 = keys['4'], &n5 = keys['5'], &n6 = keys['6'], &n7 = keys['7'], &n8 = keys['8'], &n9 = keys['9'];
// inline auto& a = keys['A'], &b = keys['B'], &c = keys['C'], &d = keys['D'], &e = keys['E'], &f = keys['F'], &g = keys['G'], &h = keys['H'], &i = keys['I'];
// inline auto& j = keys['J'], &k = keys['K'], &l = keys['L'], &m = keys['M'], &n = keys['N'], &o = keys['O'], &p = keys['P'], &q = keys['Q'], &r = keys['R'];
// inline auto& s = keys['S'], &t = keys['T'], &u = keys['U'], &v = keys['V'], &w = keys['W'], &x = keys['X'], &y = keys['Y'], &z = keys['Z'];
// inline auto& np_0 = keys[VK_NUMPAD0], &np_1 = keys[VK_NUMPAD1], &np_2 = keys[VK_NUMPAD2], &np_3 = keys[VK_NUMPAD3], &np_4 = keys[VK_NUMPAD4];
// inline auto& np_5 = keys[VK_NUMPAD5], &np_6 = keys[VK_NUMPAD6], &np_7 = keys[VK_NUMPAD7], &np_8 = keys[VK_NUMPAD8], &np_9 = keys[VK_NUMPAD9];
// inline auto& np_mul = keys[VK_MULTIPLY], &np_add = keys[VK_ADD], &np_sub = keys[VK_SUBTRACT], &np_div = keys[VK_DIVIDE], &np_decimal = keys[VK_DECIMAL];
// inline auto& f1 = keys[VK_F1], &f2 = keys[VK_F2], &f3 = keys[VK_F3], &f4 = keys[VK_F4], &f5 = keys[VK_F5], &f6 = keys[VK_F6];
// inline auto& f7 = keys[VK_F7], &f8 = keys[VK_F8], &f9 = keys[VK_F9], &f10 = keys[VK_F10], &f11 = keys[VK_F11], &f12 = keys[VK_F12];
// inline auto& minus = keys[VK_OEM_MINUS], &plus = keys[VK_OEM_PLUS], &comma = keys[VK_OEM_COMMA], &period = keys[VK_OEM_PERIOD];

} // namespace yw::key


export namespace yw::win {

/// window handle type
using HWND = ::HWND;


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
