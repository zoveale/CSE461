#pragma once
#ifndef SDISK_H
#define SDISK_H

#include <string>
#include <fstream>
#include <cstdio>

class Sdisk {
public:
  Sdisk(std::string diskname, int numberofblocks, int blocksize) :
    diskname(diskname), numberofblocks(numberofblocks), blocksize(blocksize) {
    if (!LoadDisk()) {
      CreateDisk();
    }
  }
  int GetBlock(int blocknumber, std::string& buffer) {
    std::ifstream file(diskname.c_str(), std::fstream::binary || std::fstream::app);
    file.seekg((__int64)blocknumber * (__int64)blocksize);

    char* block = new char[blocksize + 1];
    block[blocksize] = '\0';
    file.read(block, blocksize);
    buffer = std::string(block);
    delete[] block;

    if (!file.good())
      return 0;

    return 1;
  }
  int PutBlock(int blocknumber, const std::string& buffer) {
    std::ofstream file(diskname.c_str(), std::fstream::binary || std::fstream::out);
    file.seekp((__int64)blocknumber * (__int64)blocksize);
    file.write(buffer.c_str(), blocksize);

    if (!file.good())
      return 0;

    return 1;
  }
  int GetNumberOfBlocks() { return numberofblocks; }
  int GetBlockSize() { return blocksize; }
private:
  std::ifstream inFile;
  std::string diskname;        // file name of software-disk
  int numberofblocks;     // number of blocks on disk
  int blocksize;          // block size in bytes
  bool LoadDisk() {
    inFile.open(diskname.c_str(), std::fstream::binary);
    return inFile.is_open();
  }
  void CreateDisk() {
    std::ofstream file(diskname.c_str(), std::fstream::binary);
    std::string buffer(numberofblocks * blocksize, '#');
    //TODO:: add \n after each block write ?
    //for(int i = 0; i < numberofblocks; i++)
    //no need because memory is contiguous?
    file.write(buffer.c_str(), (__int64)numberofblocks * (__int64)blocksize);

    if (!file.good())
      printf("File did not write!\n");

  }
};
#endif // !SDISK_H
