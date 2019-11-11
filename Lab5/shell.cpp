#include "shell.h"

Shell::Shell(std::string filename, int numberofblocks, int blocksize) :
  FileSystem(filename, numberofblocks, blocksize){
}

int Shell::Directory() {
  std::vector<std::string> fileList = List();
  for (unsigned int i = 0; i < fileList.size(); i++) {
    printf("%s\n", fileList[i].c_str());
  }
  return 0;
}

int Shell::Add(std::string file) {
  if (NewFile(file))
    return 1;

  printf("file already exsits\n");
  return 0;
}

int Shell::Delete(std::string file) {
  RemoveFile(file);
  return 0;
}

int Shell::Type(std::string file) {
  int firstBlock = GetFirstBlock(file).first;
  if (firstBlock == 0) { 
    printf("file does not exist\n");
    return 0;
  }

  std::string buffer;
 
  for (int i = firstBlock; i != 0; i = NextBlock(file, i)) {
    ReadBlock(file, i, buffer);
    printf("%s", buffer.c_str());
  }
  printf("\n");

   return 1;
}

int Shell::Copy(std::string file1, std::string file2) {

  int firstBlock = GetFirstBlock(file1).first;
  if (firstBlock == 0) {
    printf("file does not exist\n");
    return 0;
  }
  std::string buffer;
  std::string bufferCopy{ "" };
  int k{ 0 };
  for (int i = firstBlock; i != 0; i = NextBlock(file1, i)) {
    ReadBlock(file1, i, buffer);
    bufferCopy += buffer;
    ++k; //number of blocks needed
  }
  int bufferSize{ buffer.size() };
  int j{ 0 };
  firstBlock = GetFirstBlock(file2).first;
  if (firstBlock == 0) {
    Add(file2);
    firstBlock = GetFirstBlock(file2).first; 
  }
  
  for (int i = firstBlock; j != k; i = NextBlock(file2, i)) {
    if (i == 0)
      AddBlock(file2, bufferCopy.substr(bufferSize * j, bufferSize * (j + 1)));
    else
      WriteBlock(file2, i, bufferCopy.substr(bufferSize * j, bufferSize * (j + 1)));
    ++j;
  }
  return 0;
}
