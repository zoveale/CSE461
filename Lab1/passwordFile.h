#ifndef PASSWORDFILE_H
#define PASSWORDFILE_H

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>  

class PasswordFile {
public:

  PasswordFile(std::string filename);// opens the file and reads the names/passwords in the vectors user and password.
  void AddPassword(std::string newuser, std::string newpassword); //this adds a new user/password to the vectors and writes the vectors to the file filename
  bool CheckPassword(std::string user, std::string passwd); // returns true if user exists and password matches
  static void NewSalt(int ns);
  void PrintFile();

private:
  
  std::string filename; // the file that contains password information
  std::vector<std::string> user; // the list of usernames
  std::vector<std::string> password; // the list of passwords
  void SynchFile();// writes the user / password vectors to the password file
  static int salt;
  
  std::string Encrypt(std::string s);
  //std::string Decrypt(std::string s);
};

#endif // !PASSWORDFILE_H
