// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;

int main() {
  std::cout.imbue(std::locale(""));
  std::wcout.imbue(std::locale(""));
  try {

    nat a = 1236254;
    std::cout << a << std::endl;
    std::cout << yw::_::_ntos<cat1>(a) << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
