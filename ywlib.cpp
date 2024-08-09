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

    for (auto s = input(L"設定したいマウス位置を入力", 200); !s.empty();
              s = input(L"設定したいマウス位置を入力", 200)) {
      StringView sv = s;
      auto x = stov<fat4>(sv);
      auto y = stov<fat4>(sv);
      std::cout << x << ", " << y << std::endl;
      main::mouse({x, y});
    }

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
