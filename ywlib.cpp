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

    auto a = sha256("The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog");
    std::cout << a << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
