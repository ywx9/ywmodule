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
#undef FormatMessage
#undef GetCommandLine
#undef GetModuleHandle
#undef GetObject
#undef GetWindowText
#undef GetWindowTextLength
#undef LoadCursor
#undef MessageBox
#undef RegisterClass
#undef SendMessage
#undef SetWindowText

#include "array.hpp"
#include "exception.hpp"
#include "string.hpp"
#include "logger.hpp"
#include "vector.hpp"

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
using POINT = ::POINT;
using RECT = ::RECT;
using WNDCLASS = ::WNDCLASSEXW;
using WNDPROC = ::WNDPROC;

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

inline int4 MessageBox(HWND Window, const cat2* Text, const cat2* Caption, nat4 Type) {
  return ::MessageBoxW(Window, Text, Caption, Type);
}

inline int4 QueryPerformanceCounter(LARGE_INTEGER* PerformanceCount) {
  return ::QueryPerformanceCounter(PerformanceCount);
}

inline int4 QueryPerformanceFrequency(LARGE_INTEGER* Frequency) {
  return ::QueryPerformanceFrequency(Frequency);
}

inline int2 RegisterClass(const WNDCLASS& WndClass) {
  return ::RegisterClassExW(&WndClass);
}

inline void RegisterClass(WNDCLASS&& WndClass) = delete;

inline int4 ScreenToClient(HWND Window, POINT* Point) {
  return ::ScreenToClient(Window, Point);
}

inline int8 SendMessage(HWND Window, nat4 Message, nat8 WParam, int8 LParam) {
  return ::SendMessageW(Window, Message, WParam, LParam);
}

inline HWND SetFocus(HWND Window) {
  return ::SetFocus(Window);
}

inline int4 SetWindowText(HWND Window, const cat2* Text) {
  return ::SetWindowTextW(Window, Text);
}

} // namespace yw::win


export namespace yw {


/// instance handle
inline const win::HINSTANCE INSTANCE_HANDLE = [] {
  if (auto hi = win::GetModuleHandle(nullptr); !hi) {
    logger.fatal("Failed to initialize the instance handle");
    std::cerr << "Failed to initialize the instance handle" << std::endl;
    return win::HINSTANCE{};
  } else return hi;
}();


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
  return std::abs(lf.lfHeight) * cev(96.f / 72.f);
}();


/// command line arguments; argv[0] is the program name
inline const Array<str1> argv = [] {
  int argc;
  auto a = win::CommandLineToArgv(win::GetCommandLine(), &argc);
  Array<str1> r(argc);
  for (; 0 <= --argc;) r[argc] = cvt<cat1>(a[argc]);
  logger.file = Path(r[0]).replace_extension(".log");
  return r;
}();


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
  throw exception(text.c_str(), _);
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
inline Vector get_virtual_screen_size() {
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


namespace _ {
bool _input_alive = false;
struct _input_data_t { const cat2* text; str2* result; };
int8 WINAPI _input_proc(HWND hw, nat4 msg, nat8 wp, int8 lp) {
  static _input_data_t data;
  static HWND hw_text, hw_edit, hw_button;
  if (msg == WM_CREATE) {
    auto* p = reinterpret_cast<CREATESTRUCTW*>(lp);
    data = *reinterpret_cast<_input_data_t*>(p->lpCreateParams);
    auto height = int4(DEFAULT_FONT_SIZE);
    hw_text = win::CreateWindow(0, L"STATIC", data.text, WS_CHILD | WS_BORDER | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
                                2, 2, p->cx - 4, height, hw, nullptr, nullptr, nullptr);
    hw_edit = win::CreateWindow(0, L"EDIT", nullptr, WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE | ES_AUTOHSCROLL,
                                2, 4 + height, p->cx - 4, height, hw, nullptr, nullptr, nullptr);
    hw_button = win::CreateWindow(0, L"BUTTON", L"OK", WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
                                  2, 6 + 2 * height, p->cx - 4, height, hw, nullptr, nullptr, nullptr);
    win::SendMessage(hw_text, WM_SETFONT, (nat8)SYSTEM_FONT_HANDLE, 0);
    win::SendMessage(hw_edit, WM_SETFONT, (nat8)SYSTEM_FONT_HANDLE, 0);
    win::SendMessage(hw_button, WM_SETFONT, (nat8)SYSTEM_FONT_HANDLE, 0);
    win::SetFocus(hw_edit);
    _input_alive = true;
  } else if (msg == WM_COMMAND) {
    if (HIWORD(wp) == BN_CLICKED) {
      *data.result = get_window_text(hw_edit);
      win::DestroyWindow(hw);
      return 0;
    }
  } else if (msg == WM_CLOSE) return win::DestroyWindow(hw), 0;
  else if (msg == WM_DESTROY) return _input_alive = false, 0;
  return win::DefWindowProc(hw, msg, wp, lp);
}
inline str2 _input(const str2& Text, int4 Width) {
  struct Data {
    const cat2* text;
    str2* result;
  };
  const win::WNDCLASS wc{
    sizeof(win::WNDCLASS), 0, _input_proc, 0, 0, INSTANCE_HANDLE, nullptr,
    win::LoadCursor(nullptr, (const cat2*)IDC_ARROW),
    (HBRUSH)win::GetStockObject(BLACK_BRUSH), nullptr, L"YWINPUT", nullptr};
  static const auto atom = win::RegisterClass(wc);
  if (!atom) {
    tiff(HRESULT_FROM_WIN32(::GetLastError()));
    throw std::runtime_error("RegisterClass failed");}
  str2 result;
  Data data{Text.c_str(), &result};
  auto sz = get_virtual_screen_size();
  auto cp = get_cursor_pos();
  std::cout << cp.x << ", " << cp.y << std::endl;
  auto height = int4(DEFAULT_FONT_SIZE) * 3 + 8;
  sz.x = cp.x < sz.x / 2 ? cp.x : cp.x - Width;
  sz.y = cp.y < sz.y / 2 ? cp.y : cp.y - height;
  auto hw = win::CreateWindow(
    0, wc.lpszClassName, Text.c_str(), WS_POPUP,
    int4(sz.x), int4(sz.y), Width + 4, height, nullptr, nullptr, INSTANCE_HANDLE, &data);
  if (!hw) throw std::runtime_error("CreateWindow failed");
  ::ShowWindow(hw, SW_SHOWNORMAL);
  MSG msg;
  while (_input_alive && ::GetMessageW(&msg, nullptr, 0, 0) > 0) {
    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);
  }
  return result;
}

}


/// displays a input window with a label, a text box, and an `OK` button
/// \note The window returns the input text when the user clicks the `OK` button.
inline str2 input(const str2& Text, numeric auto Width) {
  return _::_input(Text, Width);
}

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
