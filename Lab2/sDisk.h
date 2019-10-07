#ifndef SDISK_H
#define SDISK_H

#include <string>
#include <fstream>
#include <cstdio>

class Sdisk {
public:
  Sdisk(std::string diskname, int numberofblocks, int blocksize) :
    diskname(diskname), numberofblocks(numberofblocks), blocksize(blocksize) {
    std::ifstream checkFile(diskname.c_str());

    if (!checkFile.is_open()) {
      std::ofstream file(diskname.c_str(), std::fstream::binary);
      std::string buffer(numberofblocks * blocksize, '#');
      file.write(buffer.c_str(), numberofblocks * blocksize);

      if (!file.good())
        printf("File did not write!\n");
    }

  }
  int GetBlock(int blocknumber, std::string& buffer) {
    std::ifstream file(diskname.c_str(), std::fstream::binary || std::fstream::app);
    file.seekg(blocknumber * blocksize);

    char* block = new char[blocksize + 1];
    block[blocksize] = '\0';
    file.read(block, blocksize);
    buffer = std::string(block);
    delete[] block;

    if (!file.good())
      return 0;

    return 1;
  }
  int PutBlock(int blocknumber, std::string buffer) {
    std::ofstream file(diskname.c_str(), std::fstream::binary || std::fstream::out);
    file.seekp(blocknumber * blocksize);
    file.write(buffer.c_str(), blocksize);

    if (!file.good())
      return 0;

    return 1;
  }
  int GetNumberOfBlocks() { return numberofblocks; }
  int GetBlockSize() { return blocksize; }
private:
  std::string diskname;        // file name of software-disk
  int numberofblocks;     // number of blocks on disk
  int blocksize;          // block size in bytes
};
#endif // !SDISK_H
