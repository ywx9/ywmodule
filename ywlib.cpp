// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;

int main() {
  std::wcout.imbue(std::locale("Japanese"));
  std::ofstream ofs("shiftjis.txt");
  for (cat2 w{}; w < 0x1000; ++w) {
    const auto sj = win::to_shiftjis(str2{w});
    nat2 code = sj[1] << 8 | sj[0];
    ofs << std::format("{:04x}, ", code);
    if ((w & 15) == 15) ofs << '\n';
    if ((w & 255) == 255) ofs << '\n';
  }
  ofs.flush();
  ofs.close();
}
