#include "shell.h"

int Shell::Directory() {
  std::vector<std::string> fileList = List();
  for (int i = 0; i < fileList.size(); i++) {
    printf("%s/n", fileList[i].c_str());
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
