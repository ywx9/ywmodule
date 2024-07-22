// for test

#ifndef ywlib_module
#include "ywlib.hpp"
#else
import ywlib;
#endif

int main() {
  yw::hello_world();
  std::print("Hello, World from {}!\n", __FILE__);
  // std::operator""sv("Hello, World from {}!", 12);
}