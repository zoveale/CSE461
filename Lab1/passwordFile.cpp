#include "passwordFile.h"

int PasswordFile::salt;

PasswordFile::PasswordFile(std::string filename) {
  salt = 1;

  std::fstream inFile;
  this->filename = filename;
  inFile.open(this->filename, std::ios::in | std::ios::out);

  std::string inData;


  while (!inFile.eof()) {
    inFile >> inData;
    user.push_back(inData);
    inFile >> inData;
    password.push_back(inData);

  }
  user.pop_back();
  password.pop_back();
  inFile.close();
}

void PasswordFile::AddPassword(std::string newuser, std::string newpassword) {
  
  user.push_back(newuser);
  password.push_back(newpassword);
  SynchFile();
}

bool PasswordFile::CheckPassword(std::string user, std::string passwd) {
  for (int i = 0; i < user.size(); i++) {
    if (this->user[i].c_str() == user && this->password[i].c_str() == passwd) {
      return true;
    }
  }
  return false;
}

void PasswordFile::PrintFile() {
  for (int i = 0; i < user.size(); i++) {
    printf("user: %s, pass: %s\n", user[i].c_str(), password[i].c_str());
  }
}

void PasswordFile::NewSalt(int ns) {
  salt = ns;
}

std::string PasswordFile::Encrypt(std::string s) {
  for (int i = 0; i < s.size(); i++) {
    s[i] += salt;
  }
  return s;
}

void PasswordFile::SynchFile() {
  std::fstream inFile;
  inFile.open(filename, std::ios::out);

  for (int i = 0; i < user.size(); i++) {
    inFile << user[i].c_str() << " ";
    inFile << password[i].c_str() << " \n";
  }
  inFile.close();
}
