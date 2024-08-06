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

    auto r = get_virtual_screen_size();
    std::cout << "Desktop window size: " << r.x << "x" << r.y << std::endl;

    str2 s{};
    while ((s = input(L"Enter a string: ", 100), !s.empty())) {
      std::cout << "SHA-256: " << sha256(s) << std::endl;
    }

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
