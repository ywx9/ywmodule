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

    auto a = file::list("/", true);
    for (const auto& e : a) std::cout << e << std::endl;
    std::cout << std::endl;

    auto b = file::read("LICENSE");
    std::cout << b << std::endl;
    std::cout << std::endl;

    std::cout << "あいうえお" << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
