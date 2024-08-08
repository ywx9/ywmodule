/// \file main.hpp

#pragma once

#include "directx.hpp"
#include "logger.hpp"
#include "windows.hpp"

export namespace yw::main {

/// instance handle
inline const win::HINSTANCE INSTANCE_HANDLE = win::GetModuleHandle(nullptr);

/// user procedure for main window
/// \note Return `true` to cancel the default procedure.
inline bool (*userproc)(win::HWND hw, nat4 msg, nat8 wp, int8 lp) = nullptr;

} // namespace yw::main ////////////////////////////////////////////////////////

namespace yw::_ {

/// procedure for main window
int8 WINAPI _main_proc(win::HWND hw, nat4 msg, nat8 wp, int8 lp) {
  if (main::userproc && main::userproc(hw, msg, wp, lp)) return 0;
  else return win::DefWindowProc(hw, msg, wp, lp);
}

export inline const win::WNDCLASS _main_wc{
  sizeof(win::WNDCLASS), 0, _main_proc, 0, 0,
  INSTANCE_HANDLE, nullptr, win::LoadCursor(nullptr, (const cat2*)IDC_ARROW),
  (win::HBRUSH)win::GetStockObject(BLACK_BRUSH), nullptr, L"YWLIB", nullptr};

export inline const auto _main_atom = win::RegisterClass(_main_wc);

} // namespace yw::_ ///////////////////////////////////////////////////////////

namespace yw::main {

/// initializes the main window
/// \param Title title of the window
/// \param Rect rectangle of the window; `{left, top, right, bottom}`
/// \param Style style of the window
bool init(
  const str2& Title,
  const Vector& Rect,
  nat4 Style = WS_POPUP) {
  // initialize window
  win::RECT r{int4(Rect.x), int4(Rect.y), int4(Rect.z), int4(Rect.w)};
  win::AdjustWindowRect(&r, Style, false, 0);
  if (!_::_main_atom) return logger.error("Failed to register the main window class."), false;
  auto hw = win::CreateWindow(
    WS_EX_ACCEPTFILES, (const cat2*)_::_main_atom, Title.c_str(), Style,
    r.left, r.top, r.right - r.left, r.bottom - r.top,
    nullptr, nullptr, INSTANCE_HANDLE, nullptr);
  if (!hw) return logger.error("Failed to create the main window."), false;
  win::ShowWindow(hw, SW_SHOW);
  win::UpdateWindow(hw);
  // initialize direct3d

}

} // namespace yw::main ////////////////////////////////////////////////////////
