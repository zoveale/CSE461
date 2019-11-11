#include <iostream>
#include "shell.h"


int main(int argc, char* argv) {
  //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
  Sdisk diskA("diskA", 256, 128);
  FileSystem fsys("diskA", 256, 128);
  Shell shell("diskA", 256, 128);

  std::string s;
  std::string command = "go";
  std::string op1, op2;

  while (command != "quit") {
    command.clear();
    op1.clear();
    op2.clear();
    std::cout << "$";
    std::getline(std::cin, s);
    int firstblank = s.find(' ');
    if (firstblank < s.length()) s[firstblank] = '#';
    int secondblank = s.find(' ');
    command = s.substr(0, firstblank);
    if (firstblank < s.length())
      op1 = s.substr(firstblank + 1, secondblank - firstblank - 1);
    if (secondblank < s.length())
      op2 = s.substr(secondblank + 1);
    if (command == "dir") {
      // use the ls function
      shell.Directory();
    }
    if (command == "add") {
      // The variable op1 is the new file
      shell.Add(op1);
    }
    if (command == "del") {
      // The variable op1 is the file
      shell.Delete(op1);
    }
    if (command == "type") {
      // The variable op1 is the file
      shell.Type(op1);
    }
    if (command == "copy") {
      // The variable op1 is the source file and the variable op2 is the destination file.
      shell.Copy(op1, op2);
    }

  }

  return 0;
}

