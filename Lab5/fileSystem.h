#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <sstream>
#include <iomanip> 
#include <utility>  

#include "sDisk.h"

class FileSystem : public Sdisk {
public:
  FileSystem(std::string diskname, int numberofblocks, int blocksize);
  int FileSystemClose();
  int FileSystemSynch();
  int NewFile(std::string file);
  int RemoveFile(std::string file);
  std::pair<int, int> GetFirstBlock(std::string file);
  int AddBlock(std::string file, std::string blockBuffer);
  int DeleteBlock(std::string file, int blocknumber);
  int ReadBlock(std::string file, int blocknumber, std::string& buffer);
  int WriteBlock(std::string file, int blocknumber, std::string buffer);
  int NextBlock(std::string file, int blocknumber);
  int CheckBlock(std::string file, int blocknumber);
  std::vector<std::string> List();
  //std::vector<std::string> Block(std::string buffer, int blockSize);
private:
  const int FILE_ALLOCATION_SIZE = 4;
  const int FILE_NAME_SIZE = 10;
  const int FAT_MEMORY_POSITION = 1;
  const int ROOT_MEMORY_POSITION = 0;
  const int FREE_BLOCK = 0;
  int FreeBlock();
  int rootSize;           // maximum number of entries in ROOT
  int fatSize;            // number of blocks occupied by FAT
  std::vector<std::string> filename;   // filenames in ROOT
  std::vector<int> firstBlock; // firstblocks in ROOT
  std::vector<int> fat;             // FAT
  
};
#endif // !FILESYSTEM_H

