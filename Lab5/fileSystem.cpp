#include "fileSystem.h"

FileSystem::FileSystem(
  std::string diskname, int numberofblocks, int blocksize) :
  Sdisk(diskname, numberofblocks, blocksize),
  rootSize(GetBlockSize() / FILE_NAME_SIZE),
  fatSize(GetNumberOfBlocks()* FILE_ALLOCATION_SIZE / GetBlockSize()) {


  std::string buffer;
  GetBlock(1, buffer);
  if (buffer[1] == '#') {
    //new disk
    for (int i = 0; i < rootSize; i++) {
      //TODO:: preprocessor defines for no file and first block zero, NULL maybe
      filename.push_back("xxxxx");
      firstBlock.push_back(0);
      ///
    }
    for (int i = 0; i < GetNumberOfBlocks(); i++) {
      if (i < fatSize + FAT_MEMORY_POSITION) {
        fat.push_back(0);
      }
      else {
        fat.push_back(i + 1);
      }
    }
    //fat.resize(GetNumberOfBlocks(), -1);
    fat[0] = fatSize + FAT_MEMORY_POSITION;
    fat[GetNumberOfBlocks() - FAT_MEMORY_POSITION] = 0;
    FileSystemSynch();
  }
  else {
    //load disk
    buffer.clear();
    GetBlock(0, buffer);
    std::stringstream diskData(buffer);
    for (unsigned int i = 0; i < rootSize; i++) {
      std::string name;
      int blockNumber;
      diskData >> name >> blockNumber;
      filename.push_back(name);
      firstBlock.push_back(blockNumber);
    }

    diskData.str("");
    diskData.clear();
    for (int i = FAT_MEMORY_POSITION; i < fatSize + FAT_MEMORY_POSITION; i++) {
      GetBlock(i, buffer);
      diskData.str(diskData.str() + buffer);
    }

    for (unsigned int i = 0; i < GetNumberOfBlocks(); i++) {
      int blockNumber;
      diskData >> blockNumber;
      fat.push_back(blockNumber);
    }
  }


}

int FileSystem::FileSystemClose() {
  FileSystemSynch();
  return 1;
}

int FileSystem::FileSystemSynch() {

  std::string syncBuffer;
  for (int i = 0; i < filename.size(); i++) {
    syncBuffer += filename[i] + " " + std::to_string(firstBlock[i]) + " ";
  }
  for (int i = syncBuffer.size(); i < GetBlockSize(); i++) {
    syncBuffer += " ";
  }
  PutBlock(ROOT_MEMORY_POSITION, syncBuffer);

  syncBuffer.clear();
  //TODO:: convert int to hex
  for (unsigned i = 0; i < fat.size(); ++i) {
    syncBuffer += std::to_string(fat[i]) + " ";
  }
  for (int i = syncBuffer.size(); i < GetBlockSize() * fatSize; i++) {
    syncBuffer += " ";
  }
  for (int i = 0; i < fatSize; i++) {
    PutBlock(i + 1, syncBuffer.substr(i * GetBlockSize(), GetBlockSize()));
  }



  return 0;
}

int FileSystem::NewFile(std::string file) {

  for (std::string i : filename) {
    if (i == file)
      return 0; // file exists already
  }

  int freeFileIndex = 0;
  for (int i : firstBlock) {
    if (i == 0) {
      break;
    }
    ++freeFileIndex;
  }
  if (freeFileIndex >= rootSize) return 0;

  //TODO:: fat index following new file needs to = 0
  filename[freeFileIndex] = file;
  firstBlock[freeFileIndex] = fat[FREE_BLOCK];
  fat[FREE_BLOCK] = fat[fat[fat[FREE_BLOCK]]];
  fat[fat[firstBlock[freeFileIndex]]] = 0;



  FileSystemSynch();
  return 1;
}

int FileSystem::RemoveFile(std::string file) {
  std::pair<int, int> firstBlockPair = GetFirstBlock(file);
  if (firstBlockPair.first < fatSize + FAT_MEMORY_POSITION) return 0;

  DeleteBlock(file, firstBlockPair.first);
  filename[firstBlockPair.second] = "xxxxx";
  fat[FREE_BLOCK] = firstBlock[firstBlockPair.second];
  firstBlock[firstBlockPair.second] = 0;
  FileSystemSynch();
  return 1;
}

//first element = firstBlock[index], second element = index
std::pair<int, int> FileSystem::GetFirstBlock(std::string file) {
  std::pair<int, int> firstBlockPair;
  int freeFileIndex = 0;
  for (std::string i : filename) {
    if (i == file)
      break;
    else
      ++freeFileIndex;
  }
  //TODO >= or just >??
  if (freeFileIndex > rootSize) return std::pair<int, int>(0, 0);


  firstBlockPair.first = firstBlock[freeFileIndex];
  firstBlockPair.second = freeFileIndex;
  FileSystemSynch();
  return std::pair<int, int>(firstBlock[freeFileIndex], freeFileIndex);
}

int FileSystem::AddBlock(std::string file, std::string blockBuffer) {
  int endBlock = 0;
  int firstBlock = GetFirstBlock(file).first;
  if (firstBlock < fatSize + FAT_MEMORY_POSITION) return 0;
  for (int i = firstBlock; i < fat.size(); i = fat[i]) {
    if (fat[i] == 0) {
      endBlock = i;
      break;
    }
  }
  PutBlock(endBlock, blockBuffer);
  fat[endBlock] = fat[FREE_BLOCK];
  fat[FREE_BLOCK] = fat[fat[FREE_BLOCK]];
  fat[fat[endBlock]] = 0;
  FileSystemSynch();
  return 0;
}

//TODO:: change to only remove that block number in file not all files following
int FileSystem::DeleteBlock(std::string file, int blocknumber) {
  int startBlock = GetFirstBlock(file).first;
  for (int i = startBlock; i < fat.size(); i = fat[i]) {
    if (i == blocknumber) {
      startBlock = blocknumber;
      break;
    }
    if (fat[i] == 0) {
      return 0;
    }
  }
  for (int i = startBlock; i < fat.size(); i = fat[i]) {
    if (fat[i] == 0) {
      fat[i] = fat[FREE_BLOCK];
      fat[FREE_BLOCK] = fat[startBlock];
      return 1;
    }
  }
  return 0;
}

int FileSystem::ReadBlock(std::string file, int blocknumber, std::string& blockBuffer) {
  int startBlock = GetFirstBlock(file).first;
  for (int i = startBlock; i < fat.size(); i = fat[i]) {
    if (i == blocknumber) {
      GetBlock(blocknumber, blockBuffer);
      return 1;
    }
  }
  return 0;
}



int FileSystem::WriteBlock(std::string file, int blocknumber, std::string blockBuffer) {
  int startBlock = GetFirstBlock(file).first;
  for (int i = startBlock; i < fat.size(); i = fat[i]) {
    if (i == blocknumber) {
      PutBlock(blocknumber, blockBuffer);
      return 1;
    }
  }
  return 0;
}

int FileSystem::NextBlock(std::string file, int blocknumber) {
  if (CheckBlock(file, blocknumber) == 1) {
    return fat[blocknumber];
  }
  return 0;
}

int FileSystem::CheckBlock(std::string file, int blocknumber) {
  int startBlock = GetFirstBlock(file).first;
  if (startBlock > 0) {
    for (int i = startBlock; i < fat.size(); i = fat[i]) {
      if (fat[i] == blocknumber)
        return 1;
    }
  }

  return 0;
}

std::vector<std::string> FileSystem::List() {
  std::vector<std::string> fileList;
  for (int i = 0; i < filename.size(); i++) {
    if (filename[i] != "xxxxx") {
      fileList.push_back(filename[i]);
    }
  }
  return fileList;
}

int FileSystem::FreeBlock() {
  int index = 0;
  for (int i : fat) {
    if (i == -1) {
      return index;
    }
    ++index;
  }
  return 0;
}
