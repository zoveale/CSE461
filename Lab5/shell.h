#include "fileSystem.h"

class Shell : public FileSystem {
public:
  Shell(std::string filename, int numberofblocks, int blocksize);
  int Directory();// lists all files
  int Add(std::string file);// add a new file using input from the keyboard
  int Delete(std::string file);// deletes the file
  int Type(std::string file);//lists the contents of file
  int Copy(std::string file1, std::string file2);//copies file1 to file2
private:
  const int FILENAME_SIZE = 5;
};
