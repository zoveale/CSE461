#include "passwordFile.h"

int main(int argc, char* argv) {

  PasswordFile passfile("password.txt");
  passfile.AddPassword("dbotting", "123qwe");
  passfile.AddPassword("egomez", "qwerty");
  passfile.AddPassword("tongyu", "liberty");
  passfile.AddPassword("dbotting", "DOH!");
  passfile.PrintFile();
  
  if (passfile.CheckPassword("dbotting", "123qwe")) {
    printf("works\n");
  }

  return 0;
}