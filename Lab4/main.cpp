#include "fileSystem.h"

int main(int argc, char* argv) {
  Sdisk diskA("diskA", 256, 128);
  FileSystem fsys("diskA", 256, 128);
  
  fsys.NewFile("file1");
  fsys.NewFile("file2");
  
  
  std::string bfile1;
  for (int i = 1; i <= 1024; i++) {
    bfile1 += "1";
  }
  fsys.AddBlock("file1", bfile1);
  //fsys.RemoveFile("file1");
  //fsys.RemoveFile("file2");
  
  std::string bfile2;
  std::string block;
  


  int blocknumber = 0;

  


  for (int i = 1; i <= 2048; i++) {
    bfile2 += "2";
  }
  blocknumber = fsys.AddBlock("file2", bfile2);

  //fsys.DeleteBlock("file2", blocknumber);
  return 0;
}

