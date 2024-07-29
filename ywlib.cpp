// for test

#ifndef YWLIB
#include "ywlib.hpp"
#else
import ywlib;
#endif
using namespace yw;

int main() {
  // std::cout.imbue(std::locale("Japanese"));
  std::wcout.imbue(std::locale(""));
  try {

    stv1 is = " sadfa -873252sadfsa";
    stv1 ns = " fsageaaa  asf 1236789fasga";
    stv1 fs = " sajfdl -333.55555";

    std::wcout << L"あいうえお\n";
    // std::cout << _::_stoi(is) << std::endl;
    // std::cout << _::_ston(ns) << std::endl;
    // std::cout << _::_stof(fs) << std::endl;
    std::cout << stov<fat4>(is) << " --- " << is << std::endl;
    std::cout << stov<nat8>(ns) << " --- " << ns << std::endl;
    std::cout << stov<fat8>(fs) << " --- " << fs << std::endl;

  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
