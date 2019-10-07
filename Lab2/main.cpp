#include <iostream>
#include "sDisk.h"

int main(int argc, char* argv) {
  Sdisk disk1("test1", 16, 32);
  std::string block1, block2, block3, block4;
  for (int i = 1; i <= 32; i++) block1 = block1 + "1";
  for (int i = 1; i <= 32; i++) block2 = block2 + "2";
  disk1.PutBlock(4, block1);
  disk1.GetBlock(4, block3);
  std::cout << "Should be 32 1s : ";
  std::cout << block3 << std::endl;
  disk1.PutBlock(8, block2);
  disk1.GetBlock(8, block4);
  std::cout << "Should be 32 2s : ";
  std::cout << block4 << std::endl;;
  return 0;
}