// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;

int main() {
  std::cout.imbue(std::locale("Japanese"));
  std::wcout.imbue(std::locale("Japanese"));
  try {

    constexpr stv1 is = " sadfa -873252sadfsa";
    constexpr stv1 ns = " fsageaaa  asf 1236789fasga";
    constexpr stv1 fs = " sajfdl -333.55555";

    return stov<int8>(is);
    // std::cout << stov<nat8>(ns) << std::endl;
    // std::cout << stov<fat8>(fs) << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
