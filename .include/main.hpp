/// \file main.hpp

#pragma once

#include "status.hpp"

#include "directx.hpp"
#include "dwrite.hpp"
#include "windows.hpp"

export namespace yw::main {


/// instance handle
inline const win::HINSTANCE INSTANCE_HANDLE = win::GetModuleHandle(nullptr);


/// command line arguments; ARGV[0] is the executable path
inline const Array<str1> ARGV = [] {
  int argc;
  auto a = win::CommandLineToArgv(win::GetCommandLine(), &argc);
  Array<str1> r(argc);
  for (; 0 <= --argc;) r[argc] = cvt<cat1>(a[argc]);
  logger.file = Path(r[0]).replace_extension(".log");
  return r;
}();


/// user procedure for main window
/// \note Return `true` to cancel the default procedure.
inline bool (*userproc)(win::HWND hw, nat4 msg, nat8 wp, int8 lp) = nullptr;


/// main window procedure
int8 WINAPI wndproc(win::HWND hw, nat4 msg, nat8 wp, int8 lp) {
  if (userproc && userproc(hw, msg, wp, lp)) return 0;
  else return win::DefWindowProc(hw, msg, wp, lp);
}


/// window class of the main window
inline const win::WNDCLASS wndclass{
  sizeof(win::WNDCLASS), 0, wndproc, 0, 0,
  INSTANCE_HANDLE, nullptr, win::LoadCursor(nullptr, (const cat2*)IDC_ARROW),
  (win::HBRUSH)win::GetStockObject(BLACK_BRUSH), nullptr, L"YWLIB", nullptr};


/// atom of the main window class
export inline const nat2 wndclass_atom = win::RegisterClass(&wndclass);


/// main window handle
inline const HWND hwnd = []{
  if (!wndclass_atom) {
    std::cerr << "Failed to register the main window class." << std::endl;
    logger.error("Failed to register the main window class.");
    return HWND{};
  }
  auto hw = win::CreateWindow(
    WS_EX_ACCEPTFILES, (const cat2*)wndclass_atom, L"YWLIB",
    WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
    0, 0, 0, 0, nullptr, nullptr, INSTANCE_HANDLE, nullptr);
  if (!hw) {
    std::cerr << "Failed to create the main window." << std::endl;
    logger.error("Failed to create the main window.");
    return HWND{};
  }
  return hw;
}();


/// mouse position and wheel
struct mouse {

  /// mouse position
  inline static Status<Vector2> pos{};

  /// mouse wheel
  inline static Status<fat4> wheel{};

  /// sets the mouse position
  mouse(const Vector2& Pos) noexcept {
    win::POINT p{int4(Pos.x), int4(Pos.y)};
    win::ClientToScreen(hwnd, &p);
    win::SetCursorPos(p.x, p.y);
  }

}; ///////////////////////////////////////////////////////////////////////////// struct mouse


namespace key {

} ////////////////////////////////////////////////////////////////////////////// namespace key


} ////////////////////////////////////////////////////////////////////////////// namespace yw::main
