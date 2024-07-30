// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;


int main() {
  // std::cout.imbue(std::locale(""));
  // std::wcout.imbue(std::locale(""));
  try {

    std::cout << vtos<cat1>(-0.0000022222224343251) << std::endl;
    std::cout << vtos<cat1>(-0.00002) << std::endl;
    std::cout << vtos<cat1>(0.000002) << std::endl;
    std::cout << vtos<cat1>(0.00000000000000000000000000167262192369) << std::endl;

    std::cout << vtos<cat1>(-0.23456) << std::endl;
    std::cout << vtos<cat1>(8754324.45323) << std::endl;

    std::cout << vtos<cat1>(-589234089051111111128032.444) << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
