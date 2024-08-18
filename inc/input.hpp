/// \file input.hpp
/// \brief defines `input` function which enables user to create a input window easily

#pragma once

#include "main.hpp"

namespace yw::_ {

/// procedure for input window
int8 WINAPI _input_proc(HWND hw, nat4 msg, nat8 wp, int8 lp) {
  if (msg == WM_COMMAND && HIWORD(wp) == BN_CLICKED)
    return win::PostMessage(hw, WM_USER, 0, 0), 0;
  else return win::DefWindowProc(hw, msg, wp, lp);
}

/// `WNDCLASS` for input window
export inline const win::WNDCLASS _input_wc{
  sizeof(win::WNDCLASS), 0, _input_proc, 0, 0, INSTANCE_HANDLE, nullptr,
  win::LoadCursor(nullptr, (const cat2*)IDC_ARROW),
  (HBRUSH)win::GetStockObject(BLACK_BRUSH), nullptr, L"YWINPUT", nullptr};

/// atom for input window
export inline const nat4 _input_atom = win::RegisterClass(&_input_wc);

/// internal function to create input window
inline str2 _input(const str2& Text, int4 Width) {
  if (!_input_atom) return tiff(HRESULT_FROM_WIN32(::GetLastError())), str2{};
  str2 result;
  auto virtual_screen_size = get_window_size();
  auto cursor_position = get_cursor_pos();
  auto height = int4(DEFAULT_FONT_SIZE) * 3 + 8;
  virtual_screen_size.x = cursor_position.x < virtual_screen_size.x / 2 ?
                          cursor_position.x : cursor_position.x - Width;
  virtual_screen_size.y = cursor_position.y < virtual_screen_size.y / 2 ?
                          cursor_position.y : cursor_position.y - height;
  auto hw = win::CreateWindow(
    0, _input_wc.lpszClassName, Text.c_str(), WS_POPUP,
    int4(virtual_screen_size.x), int4(virtual_screen_size.y),
    Width + 4, height, nullptr, nullptr, INSTANCE_HANDLE, nullptr);
  if (!hw) throw Exception("Failed to create input window");
  auto style = WS_CHILD | WS_BORDER | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE;
  auto hw_text = win::CreateWindow(
    0, L"STATIC", Text.c_str(), style, 2, 2, Width,
    int4(DEFAULT_FONT_SIZE), hw, nullptr, INSTANCE_HANDLE, nullptr);
  style = WS_CHILD | WS_BORDER | WS_VISIBLE | ES_AUTOHSCROLL;
  auto hw_edit = win::CreateWindow(
    0, L"EDIT", nullptr, style, 2, 4 + int4(DEFAULT_FONT_SIZE), Width,
    int4(DEFAULT_FONT_SIZE), hw, nullptr, INSTANCE_HANDLE, nullptr);
  style = WS_CHILD | WS_BORDER | WS_VISIBLE | BS_DEFPUSHBUTTON;
  auto hw_button = win::CreateWindow(
    0, L"BUTTON", L"OK", style, 2, 6 + 2 * int4(DEFAULT_FONT_SIZE), Width,
    int4(DEFAULT_FONT_SIZE), hw, nullptr, INSTANCE_HANDLE, nullptr);
  win::SendMessage(hw_text, WM_SETFONT, (nat8)SYSTEM_FONT_HANDLE, 0);
  win::SendMessage(hw_edit, WM_SETFONT, (nat8)SYSTEM_FONT_HANDLE, 0);
  win::SendMessage(hw_button, WM_SETFONT, (nat8)SYSTEM_FONT_HANDLE, 0);
  win::SetFocus(hw_edit);
  win::ShowWindow(hw, SW_SHOWNORMAL);
  MSG msg;
  while (bool(hw) && win::GetMessage(&msg, 0, 0, 0) > 0) {
    if (msg.message == WM_USER) {
      result = get_window_text(hw_edit);
      win::DestroyWindow(hw);
      return result;
    } else if (msg.message == WM_KEYDOWN) {
      if (msg.wParam == VK_TAB) {
        if (msg.hwnd == hw_edit) win::SetFocus(hw_button);
        else win::SetFocus(hw_edit);
      } else if (msg.wParam == VK_RETURN) {
        if (msg.hwnd == hw_button) win::PostMessage(hw, WM_USER, 0, 0);
        else win::SetFocus(hw_button);
      } else win::TranslateMessage(&msg), win::DispatchMessage(&msg);
    } else win::TranslateMessage(&msg), win::DispatchMessage(&msg);
  }
  return result;
}

} ////////////////////////////////////////////////////////////////////////////// namespace yw::_


export namespace yw {


/// displays a input window with a label, a text box, and an `OK` button
/// \param Text label text to inform the user what to input
/// \param Width width of the input window
/// \return text input by the user
/// \note The window returns the input text when the user clicks the `OK` button.
inline str2 input(const str2& Text, numeric auto&& Width) {
  return _::_input(Text, int4(Width));
}


} ////////////////////////////////////////////////////////////////////////////// namespace yw
