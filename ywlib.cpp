// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;


int main() {
  std::setlocale(0, ".UTF-8");
  try {

    win::HWND hw = win::get_handle(L"RAPYW");
    if (!hw) throw std::runtime_error("Window not found");
    else win::send_data(hw, str2(44, L'c'));

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
