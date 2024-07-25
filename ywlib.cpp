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

    const str2 wcs = LR"(テレビ朝日で23日午後10時半～深夜0時までの約1時間半、CMが放送できないトラブルが発生した。翌24日朝の「グッド！モーニング」でもCMが一部欠落した。

　23日夜の「報道ステーション」は約1時間20分の放送で一度もCMが流れなかった。同番組は通常「約50本のCMがある」（同局関係者）という。午後11時26分からの「世界の車窓から」の放送前に、画面に「機器の不具合によりコマーシャルが送出できない状況となっております」とのテロップとともに夜景が表示された。)";
    std::cout << utf16_to_shiftjis(wcs) << std::endl;
  } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }
}
