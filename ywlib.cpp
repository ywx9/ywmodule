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

    std::wcout << get_font_family_name(default_text_format) << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
