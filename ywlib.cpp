// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;

int main() {
  // using t = Typepack<int, double, char, float, short>;
  // std::cout << t::count << std::endl;
  using t = Sequence<0, 1, 2, 3, 4, 5>;
  // using t = Sequence<>;
  std::cout << t::count << std::endl;
  std::cout << t::at<2> << std::endl;
  // std::wcout.imbue(std::locale("Japanese"));
  // cat1 s2[2]{};
  // // 0x00 ~ 0x7f
  // for (cat1 c = 0; c < 0x80; ++c) {
  //   s2[0] = c;
  //   auto w = win::from_shiftjis(s2);
  //   std::wcout << std::format(L"0x00{:02x} -> 0x{:04x} ({})\n", nat1(c), nat2(w[0]), w);
  // }
}
