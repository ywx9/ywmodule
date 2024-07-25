// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;

int main() {
  try {
    std::wcout.imbue(std::locale("Japanese"));

    // // std::setlocale(0, "ja_JP");
    std::ofstream ofs("shiftjis.txt");
    // // cat1 mbs[3]{};
    // // cat2 wcs[2]{};
    for (cat2 w = 0x0000; w < 0xffff; ++w) {
      if ((w & 0x00ff) == 0) ofs << "{ ";
      else if ((w & 0x000f) == 0) ofs << "  ";
      const auto sj = win::to_shiftjis(str2{w});
      nat4 code = (nat4(bitcast<nat1>(sj[1])) << 8) | bitcast<nat1>(sj[0]);
      if (code == 0x003f && w != 0x003f) code = 0x8888;
      ofs << std::format("0x{:04x}, ", code);
      if ((w & 255) == 255) ofs << std::format("}}, {:02x}\n\n", w >> 8);
      else if ((w & 15) == 15) ofs << '\n';
    }
    ofs.flush();
    ofs.close();
  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
