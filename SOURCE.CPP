// for test

#ifndef YWLIB
#include "inc/ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;


constexpr List test{1_n1, 2_i2, 3_f4, none, none};
constexpr auto a = yw::get<0>(test);
constexpr auto b = yw::get<1>(test);
constexpr auto c = yw::get<2>(test);


int main() {
  std::setlocale(0, ".UTF-8");
  try {
    std::cout << a << ", " << b << ", " << c << std::endl;

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
