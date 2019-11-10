#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <sstream>
#include <iomanip> 

#include "sDisk.h"

class FileSystem : public Sdisk {
public:
  FileSystem(std::string diskname, int numberofblocks, int blocksize);
  int FileSystemClose();
  int FileSystemSynch();
  int NewFile(std::string file);
  int RemoveFile(std::string file);
  int GetFirstBlock(std::string file);
  int AddBlock(std::string file, std::string block);
  int DeleteBlock(std::string file, int blocknumber);
  int ReadBlock(std::string file, int blocknumber, std::string& buffer);
  int WriteBlock(std::string file, int blocknumber, std::string buffer);
  int NextBlock(std::string file, int blocknumber);
private:
  const int FILE_ALLOCATION_SIZE = 4;
  const int FILE_NAME_SIZE = 7;
  const int FAT_MEMORY_POSITION = 1;
  const int ROOT_MEMORY_POSITION = 0;
  int rootSize;           // maximum number of entries in ROOT
  int fatSize;            // number of blocks occupied by FAT
  std::vector<std::string> filename;   // filenames in ROOT
  std::vector<int> firstBlock; // firstblocks in ROOT
  std::vector<int> fat;             // FAT
};
#endif // !FILESYSTEM_H

